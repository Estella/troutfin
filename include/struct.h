/* Troutfin include/struct.h
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

#include "uthash.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#if defined(SOL20)
#include <sys/filio.h>
#include <sys/select.h>
#include <unistd.h>
#endif

struct mod_function {
  char *cb; // Stores the "callback"
  int (*func)(); // Stores the callback's function
  UT_hash_handle hh;
};

struct loaded_module {
  char *name;
  void *hand; // Stores the dlopen handle
  struct mod_function *funcs;
  UT_hash_handle hh;
};

#define Module struct loaded_module *
#define ModFunc struct mod_function *

extern struct mod_function *get_cb(char *, char *);
extern struct mod_function *add_cb(char *, char *, char *);
extern int del_cb(char *, char *);
extern int modsym_load(char *, void*, void**);
extern Module load_module(char *);
extern int unload_module(char *);

struct delimfile {
  char **c;
  int n;
  int hc;
  UT_hash_handle hh;
};

struct linelist {
  struct linelist *next;
  struct linelist *prev;
  char **c;
  int hc;
};

#define df struct delimfile

extern int slurp_file(char *);

extern Module mods;
extern struct delimfile *configfile;

#ifndef TF_PID
#define TF_PID "./troutfin.pid"
#endif

#ifndef TF_EXENAME
#define TF_EXENAME "troutfin"
#endif

#ifndef DPATH
#define DPATH "/home/janicez/services-rainbow-trout" // CHANGE THIS!!!!!!
#endif

extern char **explode(char *, char *, int *);
extern char **ircexplode(char *, int *, int *);

// From IRCD
extern int  mycmp(char *, char *);
extern int  mycmp_diff(char *, char *);
extern int  myncmp(char *, char *, int);

#define ifccmp(x, y) if (strlen((x)) == strlen((y))) if (0==myncmp((x), (y), strlen((x))))
#define ifcmp(x, y) if (strlen((x)) == strlen((y))) if (0==strncmpcmp((x), (y), strlen((x))))
