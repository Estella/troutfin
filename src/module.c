/* Troutfin src/module.c
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

#include "struct.h"
#include "sys.h"
#include "dlfcn.h"

Module mods = NULL;

struct mod_function *get_cb(char *mod, char *cb) {
  Module modu = NULL;
  ModFunc modf = NULL;
  HASH_FIND_STR(mods, mod, modu);
  if (modu != NULL) {
    HASH_FIND_STR(modu->funcs, cb, modf);
    if (modf != NULL) {
      return modf;
    }
  }
  return NULL;
};

struct mod_function *add_cb(char *mod, char *cbn, char *cbsym) {
  Module modu = NULL;
  ModFunc modf = malloc(sizeof(struct mod_function));
  HASH_FIND_STR(mods, mod, modu);
  if (modu != NULL) {
    int (*func)() = dlsym(modu->hand, cbsym);
    if (!func) return NULL;
    HASH_FIND_STR(modu->funcs, cbn, modf);
    if (modf == NULL) {
      modf->cb = cbn;
      modf->func = &func;
      HASH_ADD_KEYPTR(hh, modu->funcs, modf->cb, strlen(modf->cb), modf);
      return modf;
    }
  }
  return NULL;
};

int del_cb(char *mod, char *cb) {
  Module modu = NULL;
  ModFunc modf = NULL;
  HASH_FIND_STR(mods, mod, modu);
  if (modu != NULL) {
    HASH_FIND_STR(modu->funcs, cb, modf);
    if (modf != NULL) {
      HASH_DEL(modu->funcs, modf);
      free(modf);
    }
  }
  return 0;
};

//int load_sym(void *opaque, char *sym, void **retfunc) {
//  void *ret;
/* const char *error;

  dlerror();

  ret = dlsym(opaque, sym);
  error = dlerror();

  if (ret != NULL)
    *retfunc = ret;

  if (error != NULL) return 1; // Return 1 for "WARN"
  return 0;
}; */


// from bahamut

Module load_module(char *mn) {
  char *modname = malloc(sizeof(char)*768);
  snprintf(modname, 767, "%s/%s.so", DPATH "/modules", mn);
  int (*mod_init)();
  void *modhand;
  struct loaded_module *mod = malloc(sizeof(struct loaded_module));
  modhand = mod->hand = dlopen(modname, RTLD_LAZY);
  mod->name = strdup(mn);
  if (modhand == NULL) exit(64);
  mod_init = (int (*)())dlsym(modhand, "mod_init");
  if (mod_init) (*mod_init)();
  else exit(65);
  HASH_ADD_KEYPTR(hh, mods, mod->name, strlen(mod->name), mod);
  return mod;
};

int unload_module(char *c) {
  Module m = NULL;
  HASH_FIND_STR(mods, c, m);
  if (m == NULL) return 1; // Couldn't.
  HASH_DEL(mods, m);
  dlclose(m->hand);
  free(m->funcs);
  free(m);
  return 0; // Could.
};


