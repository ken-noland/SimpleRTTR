extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <string>
#include <locale>
#include <codecvt>

template<typename Type>
bool from_lua(Type& out, lua_State* luaState, int index);
