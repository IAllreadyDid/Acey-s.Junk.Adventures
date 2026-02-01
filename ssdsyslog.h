/*----------------------------------------------------------------------

  ssdsyslog.h - SSDSyslog class header - interface to syslog(3)

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


#ifndef _SSDSYSLOG_H
#define _SSDSYSLOG_H


class SSDSyslog
{
  private:
    const char *theAppName;
  
  public:
    SSDSyslog(const char *aName);
    virtual ~SSDSyslog();
    void SyslogAlert(const char *aMsg);   // requires immediate attn
    void SyslogError(const char *aMsg);   // Error condition
    void SyslogWarning(const char *aMsg); // Warning condition
    void SyslogNotice(const char *aMsg);  // Normal, but significant, condition
    void SyslogInfo(const char *aMsg);    // informational
};

#endif

  



