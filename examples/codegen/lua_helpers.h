extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <string>
#include <locale>
#include <codecvt>

// predeclarations
inline const char* get_type_name(int type)
{
	switch (type) {
	case LUA_TNIL: return "LUA_TNIL";
	case LUA_TNUMBER: return "LUA_TNUMBER";
	case LUA_TBOOLEAN: return "LUA_TBOOLEAN";
	case LUA_TSTRING: return "LUA_TSTRING";
	case LUA_TTABLE: return "LUA_TTABLE";
	case LUA_TFUNCTION: return "LUA_TFUNCTION";
	case LUA_TUSERDATA: return "LUA_TUSERDATA";
	case LUA_TTHREAD: return "LUA_TTHREAD";
	case LUA_TLIGHTUSERDATA: return "LUA_TLIGHTUSERDATA";
	}

	return "<unknown>";
}

inline std::string to_utf8(std::wstring str)
{
	static std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.to_bytes(str);
}

inline std::wstring to_utf16(std::string str)
{
	static std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.from_bytes(str);
}

inline bool read_int(int& output, lua_State* luaState, int index, const char* key)
{
	error = false;
	lua_pushstring(luaState, key);
	lua_gettable(luaState, index - 1);
	if (!lua_isnumber(luaState, -1)) {
		lua_pop(luaState, 1);
		return false;
	}

	lua_Number luaNumber = lua_tonumber(luaState, -1);
	lua_pop(luaState, 1);
	output = (int)luaNumber;
	return true;
}

inline bool read_boolean(bool& output, lua_State* luaState, int index, const char* key)
{
	error = false;
	lua_pushstring(luaState, key);
	lua_gettable(luaState, index - 1);
	if (!lua_isboolean(luaState, -1)) {
		lua_pop(luaState, 1);
		return false;
	}

	bool luaBool = lua_toboolean(luaState, -1);
	lua_pop(luaState, 1);
	output = luaBool;
	return true;
}

inline bool read_string(std::string& output, lua_State* luaState, int index, const char* key)
{
	error = false;
	lua_pushstring(luaState, key);
	lua_gettable(luaState, index - 1);
	if (!lua_isstring(luaState, -1)) {
		lua_pop(luaState, 1);
		return false;
	}

	const char* luaString = lua_tostring(luaState, -1);
	lua_pop(luaState, 1);
	output = std::string(luaString);
	return true;
}

inline bool read_wstring(std::wstring& output, lua_State* luaState, int index, const char* key)
{
	std::string luaString;
	if (!read_string(luaString, luaState, index, key, errorPrepend, error)
	{
		return false;
	}

	output = to_wstr(luaString)
	return true;
}

template<typename T>
inline bool read_object(T& output, lua_State* luaState, int index, const char* key)
{
	lua_pushstring(luaState, key);
	lua_gettable(luaState, index - 1);
	if (!lua_istable(luaState, -1)) {
		lua_pop(luaState, 1);
		return false;
	}
	Gantry::Lua::from_lua<T>(luaState, -1, output);
	lua_pop(luaState, 1);
	return true;
}

template<typename T>
inline bool read_array(T& output, lua_State* luaState, int index, const char* key)
{
	lua_pushstring(luaState, key);
	lua_gettable(luaState, index - 1);

	if (!lua_istable(luaState, -1)) {
		lua_pop(luaState, 1);
		return false;
	}

	lua_pushnil(luaState);
	while (lua_next(luaState, -2) != 0) {
		T::value_type type;
		//TODO: we could use a generic template option and create a template class which allows for nested arrays, pointers, etc, but I just want to get this up and running
		from_lua<T::value_type>(luaState, -1, type);
		output.push_back(type);
		lua_pop(luaState, 1);
	}

	lua_pop(luaState, 1);
	return true;
}
