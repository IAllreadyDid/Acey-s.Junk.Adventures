/*----------------------------------------------------------------------

  ssdbroadcast.cxx - Broadcast class - Handles wall(1) messages

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


extern "C" {
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
}

#include "ssdbroadcast.h"


Broadcast::Broadcast()
{
}


Broadcast::~Broadcast() {}


void Broadcast::BroadcastMsg(const char *aMsg)
{
#define kMsgFile SSD_DIR "/ssd.msg"
#define kWallScript SSD_DIR "/ssdwall.sh"
    
    int fd = open(kMsgFile,
                  O_RDWR|O_TRUNC|O_CREAT,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
    
    if (fd < 0) {
#ifdef AJR_DEBUG
        perror("broadcast open error ");
#endif
    }
    else {

        if (write(fd, aMsg, strlen(aMsg)) <0) {
#ifdef AJR_DEBUG
            perror("broadcast write error ");
#endif
        }
        close(fd);
        callScript(kWallScript, kMsgFile);
    }
}


void Broadcast::callScript(const char *aScript, const char *anArg1)
{
    int pid = fork();

    if (pid == 0) {
        char *argv[3];
        argv[0] = (char *)aScript;
        argv[1] = (char *)anArg1;
        argv[2] = 0;
        execv(aScript, argv);
//          char *argv[5];
//          argv[0] = "sh";
//          argv[1] = "-c";
//          argv[2] = (char *)aScript;
//          argv[3] = (char *)anArg1;
//          argv[4] = 0;
//          execv("/bin/sh", argv);
#ifdef AJR_DEBUG
        perror("execv failed ");
#endif
        exit(127);
    }
    else {
        int status;
        
        int err = waitpid(pid, &status, 0);

#ifdef AJR_DEBUG
        if (err < 0) {
            perror("waiting on ssdwall.sh ");
        }
#endif
    }
}

