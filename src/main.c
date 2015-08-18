/* Troutfin src/main.c
 * Copyright (C) 2015- Janice "janicez" Johnson (janicez@umbrellix.tk)
 *
 * See file AUTHORS in IRC package (reproduced in CREDITS at root)
 * for additional names of the programmers and inspiration for Troutfin.
 *
 * This program is free softwmare; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 1, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

// Lots of code here is taken from IRCd-Rainbow-Trout.
#include "sys.h"
#include "struct.h"

#ifndef TF_PID
#define TF_PID "./troutfin.pid"
#endif

#ifndef TF_EXENAME
#define TF_EXENAME "troutfin"
#endif

int cellared = 0;

// This function should return 0 if the servicesd has climed into the cellar.
int climb(char *pf) {
  int pid, fd = 0;
  char buf[24];

  if (!(*pf) && isatty(0)) {
    printf("Did NOT climb into cellar; running in FOREGROUND now (No PID file specified)\n");
  } else {
    if (!isatty(0)) return 2; // 2 == We already forked
    if ((pid = fork()) <= 0) {
      printf("Did NOT climb into cellar; running in FOREGROUND now (fork() returned %d, google it then report to the developer)\n", pid);
    } else {
      if ((fd = open(pf, O_CREAT | O_WRONLY, 0600)) >= 0) {
        snprintf(buf, 23, "%7d", pid);
        if (write(fd, buf, strlen(buf)) == -1) {
          printf("Climbed into cellar but PID file not written, running in BACKGROUND now (pid: %s) - try ps x | grep %s\n", buf, TF_EXENAME);
        } else {
          printf("Climbed into cellar and PID file written, running in BACKGROUND now (pid: %s)\n", buf);
        }
        close(fd);
      } else {
        printf("Climbed into cellar but PID file not written, running in BACKGROUND now (pid: %s) - try ps x | grep %s\n", buf, TF_EXENAME);
      }
      close(0);
      exit(0); // Finally
    }
  }
  return 1; // We didn't fork.
}

int main(int parc, char *parv[]) {
  int cellar;

  climb(TF_PID);

  for(;;) ;

  return 0;
}
