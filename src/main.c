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

struct delimfile *configfile = NULL;

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
      close(1);
      close(2);
      exit(0); // Finally
    }
  }
  return 1; // We didn't fork.
}

int main(int parc, char *parv[]) {
  int cellar;

  slurp_file("./" TF_EXENAME ".conf");

  //climb(TF_PID);

  load_module("m_launch");

  return 0;
}

struct delimfile *slurp_line(int ln, char *str) {
  struct delimfile *ll = (struct delimfile*)malloc(sizeof(struct delimfile));
  char **valu;
  int i = 0, j = 0;
  if (*str == '\0') return NULL;
  char *stripnl = str;
  for (; *stripnl; *stripnl++) if (*stripnl == '\r' || *stripnl == '\n') *stripnl = '\0';
  (ll)->c  = ircexplode(str, &i, &j);
  (ll)->hc = j;
  (ll)->n  = ln;
  return ll;
}

int slurp_file(char *filename) {
  struct delimfile *lln = NULL;
  char *str = malloc(sizeof(char)*2049);
  char **valu;
  int i = 0, ln = 0, j = 0;
  FILE *fp;
  fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("fopen failed on %s, crashing!", filename);
    exit(72);
  }
  while (fgets(str, 2048, fp) != NULL) {
    ln++;
    lln = slurp_line(ln, strdup(str));
    if (lln) HASH_ADD_INT(configfile, n, lln);
  }
  free(str);
  return 0;
};

static int count_explode_elems(char *explode, char *string) {
  int i = 0;
  // Unavoidably O(len(explode)^len(string)) or something
  for (; *explode; *explode++) for (; *string; *string++) if (*string == *explode) i++;
  return i;
};

static int count_ircexplode_elems(char *string) {
  int i = 0;
  for (; *string; *string++) {
    if (*string == ' ') i++;
    if (*(string+1) == ':') break;
  }
  return i;
};

// Numel is passed by reference
char **explode(char *explode, char *string, int *numel) {
  *numel = count_explode_elems(explode, string);
  int i = 0;

  char *exploder;
  char **exploded = malloc(sizeof(char)*(*numel+1)*(7681));

  for (exploder = strtok(string, explode); NULL != exploder && i < *numel+1; i++, exploder = strtok(NULL, explode))
    exploded[i] = strdup(exploder, 7680);
  exploded[i+1] = NULL;
  return exploded;
};

char **ircexplode(char *string, int *numel, int *hascolon) {
  int i = 0;
  char *exploder;

  if (string[0] == ':') {
    *string++;
    *hascolon = 1;
  }

  char *stripnl = string;
  for (; *stripnl; *stripnl++) if (*stripnl == '\r' || *stripnl == '\n') *stripnl = '\0';

  *numel = count_ircexplode_elems(string);
  char **exploded = malloc(sizeof(char)*(*numel+1)*(7681));

  for (exploder = strtok(string, " "); NULL != exploder && i < *numel+1; i++, exploder = strtok(NULL, " ")) {
    if (exploder[0] == ':') {
      exploded[i] = strndup(string, 7680);
      break;
    }
    exploded[i] = strndup(exploder, 7680);
  }
  exploded[i+1] = NULL;
  return exploded;
};
