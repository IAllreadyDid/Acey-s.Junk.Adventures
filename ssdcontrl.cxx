/*----------------------------------------------------------------------

  ssdcontrl.cxx - Controller class - manages notification, shutdown, etc.

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


#include "ssdcontrl.h"
#include "ssdbroadcast.h"
#include "simpleups.h"
#include "ssdsyslog.h"
#include "ssdmsg.h"


extern "C" {
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
}


const int cMaxMessageLen = 160;  // up to 2 lines of info.;



Controller::Controller(char *aPortName, int aMaxBatRunTime)
    : theUPS(aPortName),
      theMaxBatRunTime(aMaxBatRunTime),
      theCounterStartTime(0)
{
    theNotifier = new Broadcast;
    theSysLogger = new SSDSyslog("ssd");

    msg_buffer = new char[cMaxMessageLen];
}


Controller::~Controller()
{
    Stop();
    
    delete theNotifier;
    theNotifier = NULL;

    delete theSysLogger;
    theSysLogger = NULL;

    delete[] msg_buffer;
    msg_buffer = NULL;
}


int Controller::Init()
{
    int ret = theUPS.OpenPort();

    if (ret < 0) {
        theSysLogger->SyslogError((char*)cOpenPortErrMsg);
    }
    else {
        ret = 0;
    }
    return ret;
}


void Controller::Start()
{
    tUPSState ups_state = eStateUndefined;
    int continue_flag = 1;
    int msg_sent = 0;
    int min_count=1;

    while (continue_flag) {

        if (ups_state != eStateOnBattery) {
            ups_state = theUPS.WaitForStateChange(ups_state);
        }
        else {
            sleep(2);
            ups_state = theUPS.GetUPSState();
        }
#ifdef AJR_DEBUG
        printf("state is [%d]\n", ups_state);
#endif
        
        switch (ups_state) {

          case eStateLineOK:

            if (msg_sent) {
                theNotifier->BroadcastMsg(cPowerRestoredMsg);	    
                theSysLogger->SyslogNotice((char*)cPowerRestoredMsg);
                msg_sent = 0;
            }
            break;
            
          case eStateLowBattery:

            if (!msg_sent) {
                theNotifier->BroadcastMsg(cOnBatteryMsg);
                theSysLogger->SyslogWarning((char*)cOnBatteryMsg);
                msg_sent = 1;
            }
            continue_flag = 0;
            theNotifier->BroadcastMsg(cLowBatteryMsg);
            sleep(5);
            shutdownSystem(cLowBatteryShutdownMsg);
            break;
            
          case eStateOnBattery:
            {
                if (!msg_sent) {
                    min_count = 1;
                    theCounterStartTime = time(NULL);
                        
                    theNotifier->BroadcastMsg(cOnBatteryMsg);
                    theNotifier->BroadcastMsg(buildPowerFailMsg(theMaxBatRunTime));
                    theSysLogger->SyslogWarning((char*)cOnBatteryMsg);
                    msg_sent = 1;
                }
                int elapsed_t = (int) (time(NULL) - theCounterStartTime);

                if ((elapsed_t / (min_count*60)) >=  1) {
                    int t_left = theMaxBatRunTime - elapsed_t;
                    
                    if (t_left <= 0) {
                        theNotifier->BroadcastMsg(cRuntimeExpiredMsg);
                        shutdownSystem(cRuntimeExpiredShutdownMsg);
                        continue_flag = 0;
                    }
                    else {
                        theNotifier->BroadcastMsg(buildPowerFailMsg(t_left));
                    }
#ifdef TIME_DEBUG
                    printf("elapsed_t = %d\n", elapsed_t);
                    printf("t_left=%d\n", t_left);
#endif
                    min_count++;
                }
            }
            break;

          case eStateBatteryDischarged:
            // TODO
            break;
            
          default:
            // no action
            break;
        }
    }
    theUPS.ClosePort();
}


void Controller::Stop()
{
    theUPS.ClosePort();
}


void Controller::shutdownSystem(const char *syslog_msg)
{
#define kShutdownScript "\"" SSD_DIR "/ssdshut.sh" "\""

    theSysLogger->SyslogAlert((char*)syslog_msg);
    system(kShutdownScript);
}


char *Controller::buildPowerFailMsg(int aTime)
{
#warning TODO: Get rid of msg_buffer
    
    if (aTime < 60) {
        strcpy(msg_buffer, cShortCountdownMsg);
    }
    else {
        memset(msg_buffer, '\0', cMaxMessageLen);
        sprintf(msg_buffer, cCountdownMsg, (int)(aTime/60), (int)(aTime%60));
    }
    return msg_buffer;
}
