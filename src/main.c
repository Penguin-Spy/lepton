#include <stdio.h>
#include <stdlib.h>

#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>

//#include <vlc/vlc.h>
//#include <vlc/libvlc.h>

// yoinked from lua.c (the standalone interpreter)
/*
** Message handler used to run all chunks
*/
static int msghandler(lua_State* L) {
  const char* msg = lua_tostring(L, 1);
  if(msg == NULL) {  /* is error object not a string? */
    if(luaL_callmeta(L, 1, "__tostring") &&  /* does it have a metamethod */
      lua_type(L, -1) == LUA_TSTRING)  /* that produces a string? */
      return 1;  /* that is the message */
    else
      msg = lua_pushfstring(L, "(error object is a %s value)",
        luaL_typename(L, 1));
  }
  luaL_traceback(L, L, msg, 1);  /* append a standard traceback */
  return 1;  /* return the traceback */
}

// yoinked from lua.c (the standalone interpreter)
/*
** Interface to 'lua_pcall', which sets appropriate message function
** and C-signal handler. Used to run all chunks.
*/
static int docall(lua_State* L, int narg, int nres) {
  int status;
  int base = lua_gettop(L) - narg;  /* function index */
  lua_pushcfunction(L, msghandler);  /* push message handler */
  lua_insert(L, base);  /* put it under function and args */
  //globalL = L;  /* to be available to 'laction' */
  //setsignal(SIGINT, laction);  /* set C-signal handler */
  status = lua_pcall(L, narg, nres, base);
  //setsignal(SIGINT, SIG_DFL); /* reset C-signal handler */
  lua_remove(L, base);  /* remove message handler from the stack */
  return status;
}

int main(int argc, char* argv[]) {

  //libvlc_instance_t * inst;
  /*libvlc_media_player_t *mp;
  libvlc_media_t *m;*/

  /* Load the VLC engine */
  //inst = libvlc_new (0, NULL);


  printf("Hello world!\n");

  lua_State* L = luaL_newstate();
  luaL_openlibs(L);  /* open standard libraries */

  int status = luaL_loadfile(L, "test.lua");
  printf("[load]: %i\n", status);

  status = docall(L, 0, LUA_MULTRET);
  printf("[call]: %i\n", status);
  if(status) {
    const char* msg = lua_tostring(L, 1);
    printf("%s\n", msg);
  }

  /*status = luaL_dostring(L, "print('hello?')");

  printf("dostring: %i\n", status);*/

  /*char* input = "C:/Users/Elliott Hart/Music/Celeste - Farewell (Original Soundtrack)/wavedash.ppt.mp3";
  PlaySound(input, NULL, SND_FILENAME | SND_SYNC);*/

  return 0;
}
