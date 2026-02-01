/*----------------------------------------------------------------------

  ssdcontrl.h - Controller class header - manages notification, shutdown, etc.

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


#ifndef __SSDCTRL_H
#define __SSDCTRL_H


#include "simpleups.h"

extern "C" {
#include <sys/types.h>
}

//class Timer;
class Broadcast;
class SSDSyslog;


class Controller {

  public:
    Controller(char *aPortName, int aMaxBatRunTime);
    ~Controller();
    virtual int Init();
    virtual void Start();
    virtual void Stop();

  private:
    Controller(const Controller &);                    // not implemented
	const Controller &operator=(const Controller &);   // ditto

  private:
    char *msg_buffer;
    int theMaxBatRunTime;
    time_t theCounterStartTime;
    SimpleUPS theUPS;
    Broadcast *theNotifier;
    SSDSyslog *theSysLogger;

    virtual void shutdownSystem(const char *syslog_msg);
    char *buildPowerFailMsg(int atime);
};


#endif


    
