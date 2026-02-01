/*----------------------------------------------------------------------

  ssdsyslog.cxx - SSDSyslog class - interface to syslog(3)

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

#include "ssdsyslog.h"

extern "C" {
#include <syslog.h>
}



SSDSyslog::SSDSyslog(const char *aName)
{
    theAppName = aName;
}


SSDSyslog::~SSDSyslog()
{
}


void SSDSyslog::SyslogAlert(const char *aMessage)
{
    openlog(theAppName, LOG_PID, LOG_DAEMON);
    syslog(LOG_ALERT, "%s", aMessage);
    closelog();
}


void SSDSyslog::SyslogError(const char *aMessage)
{
    openlog(theAppName, LOG_PID, LOG_DAEMON);
    syslog(LOG_ERR, "%s", aMessage);
    closelog();
}


void SSDSyslog::SyslogWarning(const char *aMessage)
{
    openlog(theAppName, LOG_PID, LOG_DAEMON);
    syslog(LOG_WARNING, "%s", aMessage);
    closelog();
}


void SSDSyslog::SyslogNotice(const char *aMessage)
{
    openlog(theAppName, LOG_PID, LOG_DAEMON);
    syslog(LOG_NOTICE, "%s", aMessage);
    closelog();
}


void SSDSyslog::SyslogInfo(const char *aMessage)
{
    openlog(theAppName, LOG_PID, LOG_DAEMON);
    syslog(LOG_INFO, "%s", aMessage);
    closelog();
}
