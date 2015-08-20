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

extern struct delimfile *configfile;

int mod_init() {
  struct delimfile *c;
  for (c = configfile; c != NULL; c = c->hh.next) {
    ifccmp (c->c[c->hc], "load_module")
      load_module(c->c[(c->hc)+1]);
    printf("Processed config line named %s\n", c->c[c->hc]);
    fflush(stdout);
  }
  return 0;
}
