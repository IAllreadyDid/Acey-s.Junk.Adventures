/*----------------------------------------------------------------------

  ssdmsg.h - Broadcast and syslog messages

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


#ifndef __SSDMSG_H
#define __SSDMSG_H


const char cOnBatteryMsg[] = "UPS On Battery due to Power Failure\n";

const char cLowBatteryMsg[] = "UPS Battery Depleted: System will Shut Down immediately\n";

const char cCountdownMsg[] = "System Shutting Down due to Power Failure in %d minute(s) and %d second(s)\n";

const char cShortCountdownMsg[] = "System Shutting Down due to Power Failure in less than one minute\n";

const char cLowBatteryShutdownMsg[] = "System Shutdown due to Low Battery";

const char cRuntimeExpiredMsg[] = "Battery Runtime Expired! Shutting system down immediately\n";

const char cRuntimeExpiredShutdownMsg[] = "System Shutdown: battery run time expired";

const char cPowerRestoredMsg[] = "Shutdown Cancelled: Normal Power Restored\n";


const char cOpenPortErrMsg[] = "Error opening port";

#endif



