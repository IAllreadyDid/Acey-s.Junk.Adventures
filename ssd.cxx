/*----------------------------------------------------------------------

  ssd.cxx - Reads config file, starts daemon - location of main()

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
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
}

#include "ssdcontrl.h"

#define kPIDFile SSD_DIR "/ssd.pid"


static Controller *_theController = (Controller *)0;


/* Prototypes */
void WritePidFile();
void ParseIniFile(char *file, int *mbrt, char *ttyname);
void sig_handler(int signo);
void exit_handler(void);


int main(int argc, char *argv[])
{
    char ttyName[30];
    int  maxBatRunTime;
    struct stat statbuf;

    if (stat(kPIDFile, &statbuf) == 0) {
        fprintf(stderr, "Only one copy of ssd may be running at a time\n");
        exit(-1);
    }

    if (argc > 1) {
        ParseIniFile(argv[1], &maxBatRunTime, ttyName);
    }
    else {
        fprintf(stderr,"ssd requires an initialization file as an argument\n");
        exit(-1);
    }

    _theController = new Controller(ttyName, maxBatRunTime);

    int err = _theController->Init();

    if (err < 0) {
        perror("initializing port");

        return(err);
    }

#ifdef AJR_DEBUG
    int spawn = 0;
#else
    int spawn = 1;
#endif

    if (spawn) {	
        // Daemons should spawn off a child, then kill the parent.  The child ;
        // should then become the leader of its own session.  This causes the;
        // daemon to have no controlling terminals, and to become adopted by ;
        // the init proccess.  This is not only a recommended practice, it is;
        // in that certain OSs kill off the parents of daemons when they are;
        // launched. ;
        // _NOSPAWN should be used only for debugging.;
	
        pid_t pid;

        if ((pid = fork()) < 0) {
            perror("Error forking process ");
            exit (-1);
        }
        else if (pid != 0) {
            exit (0);  // Adios to mommy and daddy...;
        }
        setsid();  // Become session leader;

        // process has no controlling terminals, cannot write to stdout,;
        // or stderr. The closes are only pedantic, the fork and setsid;
        // are sufficient to ensure process is isolated;

        fclose(stdout);
        fclose(stdin);
        fclose(stderr);
    }    
    WritePidFile();

    if (atexit(exit_handler) != 0) {
#ifdef AJR_DEBUG
        perror("Error: unable to register exit handler ");
#endif
    }
    signal(SIGTERM, sig_handler);
    signal(SIGINT, sig_handler);
    /*signal(SIGPWR, sig_handler);*/

    _theController->Start();

    return 0;
}


void WritePidFile()
{
    pid_t  pid = getpid();
    int fd;
    char *file = kPIDFile;
    
    if ((fd = open(file, O_RDWR|O_TRUNC|O_CREAT,
                   S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)) < 0) {
#ifdef AJR_DEBUG
        printf("kPIDFile = [%s]\n", kPIDFile);
        perror("Error opening ssd.pid ");
#endif
        exit(-1);
    }

    char buffer[10];
    sprintf(buffer,"%d",(int)pid);

    if (write(fd,buffer,strlen(buffer)) <0) {
#ifdef AJR_DEBUG
        perror("Error writing ssd.pid ");
#endif
    }
}


void ParseIniFile(char *file,int *mbrt, char *ttyname)
{
    FILE *fptr;

    if ((fptr = fopen(file, "r")) == NULL) {
        perror("Error opening ini file ");
        exit(-1);
    }
    char buffer[81];
    char *token;

    while (fgets(buffer, 81, fptr) != NULL) {
        token = strtok(buffer,"= ");

        while (token != NULL) {

            if (strcmp(token,"TTY") == 0) {
                token = strtok(NULL,"= ");
                strcpy(ttyname,token);
                ttyname[strlen(ttyname)-1] = '\0';
            }
            else if (strcmp(token,"MaxBatRT") == 0){
                token = strtok(NULL,"= ");
                *mbrt = atoi(token);
            }
            else {
                token = strtok(NULL,"= ");
            }
        }
    }

}


void sig_handler(int signo)
{
    switch (signo) {
      /*case SIGPWR:
        break;*/

      case SIGINT:
        exit(-1);
        break;

      case SIGTERM:
        exit(-1);
        break;
    }
}


void exit_handler(void)
{
    delete _theController;
    unlink(kPIDFile);
}
