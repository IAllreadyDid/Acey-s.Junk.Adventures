/*----------------------------------------------------------------------

  simpleups.h - SimpleUPS class header - Models simple-signalling UPS

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


#ifndef __SIMPLEUPS_H
#define __SIMPLEUPS_H


extern "C" {
#include <termios.h>
}


enum tPortState {
    ePortOpen,
    ePortClosed
};


enum tUPSState
{
    eStateUndefined = 0,
    eStateLineOK,
    eStateOnBattery,
    eStateLowBattery,
    eStateBatteryDischarged
};


class SimpleUPS
{
  public:
    SimpleUPS(const char *aPortName);
    virtual ~SimpleUPS();
    virtual int OpenPort();
    virtual int ClosePort();
    tUPSState GetUPSState();
    tUPSState WaitForStateChange(tUPSState aBaseState);
    inline const char *GetPortName() { return thePortName;}
    inline tPortState GetPortState() { return thePortState;}
    inline int GetFD() { return theFD;}

  private:
    const char *thePortName;
    tPortState thePortState;
    int theFD;
    struct termios theOriginalTerm;

    void initPort();
};



#endif

