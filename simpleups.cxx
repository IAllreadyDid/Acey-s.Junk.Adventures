/*----------------------------------------------------------------------

  simpleups.cxx - SimpleUPS class - Models simple-signalling UPS

  Copyright (C) 1999, American Power Conversion, Inc., All Rights Reserved

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

-----------------------------------------------------------------------*/


#if HAVE_CONFIG_H
#  include "config.h"
#endif


#include "simpleups.h"

extern "C" {
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
}


SimpleUPS::SimpleUPS(const char *aPortName)
{ 
    thePortName = aPortName;
    thePortState = ePortClosed;
    theFD = -1;
}


SimpleUPS::~SimpleUPS()
{
    ClosePort();
}


// ------------------------------------------------------------------------;
// Opens the device with the given mode.;
// ------------------------------------------------------------------------;
int SimpleUPS::OpenPort()
{
    int ret = -1;
    
    if (thePortState == ePortOpen) {
        ClosePort();
    }

    if ((ret = open(GetPortName(), O_RDWR|O_NOCTTY|O_NONBLOCK)) < 0) {
#ifdef AJR_DEBUG
        perror("Unable to open port ");
        printf("portname = [%s]\n", GetPortName());
#endif
    }
    else {
        theFD = ret;
        thePortState = ePortOpen;
        tcgetattr(theFD, &theOriginalTerm);

        initPort();
    }
    return ret;
}


int SimpleUPS::ClosePort()
{
    int ret = -1;

    if (thePortState == ePortOpen) {
        tcsetattr(theFD, TCSANOW, &theOriginalTerm);

        ret = close(theFD);

        thePortState = ePortClosed;
        theFD = -1;
    }
    return ret;
}


tUPSState SimpleUPS::GetUPSState()
{
    tUPSState ret = eStateUndefined;

    int flags = 0;
    
    if (ioctl(theFD, TIOCMGET, &flags) < 0) {
#ifdef AJR_DEBUG
        perror("Unable to get modem control lines ");
#endif
    }
    else {
        // State Map:
        //
        // Back-UPS
        //    CD   CTS  State
        //    ---  ---  -----
        //     0    0   On Line
        //     0    1   On Battery
        //     1    0   (undefined)
        //     1    1   Low Battery
        //
        // Smart-UPS (and Accessories)
        //    CD   CTS  State
        //    ---  ---  -----
        //     0    0   On Line
        //     0    1   On Battery
        //     1    0   Battery Discharged
        //     1    1   Low Battery
        
        if ((flags & TIOCM_CD) && (flags & TIOCM_CTS)) {
            ret = eStateLowBattery;
        }
        else if (flags & TIOCM_CTS) {
            ret = eStateOnBattery;
        }
        else {
            ret = eStateLineOK;
        }
#ifdef AJR_DEBUG

        if (flags & TIOCM_DTR) {
            printf("DTR is set!\n");
        }
#endif
#warning TODO: Battery Discharged - understand different UPS types
    }
    return ret;
}


tUPSState SimpleUPS::WaitForStateChange(tUPSState aBaseState)
{
    tUPSState cur_state = GetUPSState();
    
    while (cur_state == aBaseState) {
#ifdef TIOCMIWAIT
        int flags = TIOCM_CD | TIOCM_CTS;
        
        ioctl(GetFD(), TIOCMIWAIT, (char *)flags);

#ifdef AJR_DEBUG
        printf("ioctl broke\n");

        int tmp = 0;

        ioctl(theFD, TIOCMGET, &tmp);
        printf("bits = [0x%0x]\n", tmp);

#endif
#else
        sleep(2);
#endif
        cur_state = GetUPSState();
    }

    return cur_state;
}


// ----------------------------------------------------------------------;
// Sets up port characteristics for UPS communications;
//
//  Assumes that theFD and theOriginalTerm are properly set up (and theFD
//  is open to a serial port).
// ----------------------------------------------------------------------;
void SimpleUPS::initPort()
{
    if (thePortState != ePortOpen) {
        return;
    }
    struct termios term = theOriginalTerm;

    cfmakeraw(&term);
      
    term.c_cflag |= (CLOCAL|CS8);
      
    tcflush(theFD, TCIOFLUSH);
      
    if (tcsetattr(theFD, TCSANOW, &term) < 0) {
#ifdef AJR_DEBUG
        perror("error setting port attributes: ");
#endif
    }

    // we need to clear the shutdown bit when we open the port
    int flags = TIOCM_DTR;

    if (ioctl(theFD, TIOCMBIC, &flags) < 0) {
#ifdef AJR_DEBUG
        perror("error clearing shutdown bit");
#endif
    }

    // need to set RTS high for some reason
    flags = TIOCM_RTS;

    if (ioctl(theFD, TIOCMBIS, &flags) < 0) {
#ifdef AJR_DEBUG
        perror("error setting RTS high");
#endif
    }
    tcflush(theFD, TCIOFLUSH);
}
