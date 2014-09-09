#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

static void stackDump(lua_State *L)
{
	int i;
	int top = lua_gettop(L);
	for (i = 1; i<= top; i++) {
		int t = lua_type(L, i);
		switch(t) {
			case LUA_TSTRING:{
				printf("'%s'", lua_tostring(L, i));
				break;
			}
			case LUA_TBOOLEAN: {
				printf(lua_toboolean(L, i)?"true":"false");
				break;
			}
			case LUA_TNUMBER: {
				printf("%g", lua_tonumber(L, i));
				break;
			}
			default: {
				printf("%s", lua_typename(L, t));
				break;
			}
		}
		
		printf(" ");
	}
	printf("\n");
}

void getfield (lua_State * L, const char *key)
{
	lua_pushstring(L, key);
	lua_gettable(L, -2);
	stackDump(L);
	lua_pop(L, 1);
}

int average(lua_State *L) {
	int n = lua_gettop(L);
	double sum = 0;
	int i = 1;
	while(i <= n) {
		sum += lua_tonumber(L, i);
		i ++;
	}
	// 设置lua中调用时的函数返回值
	lua_pushnumber(L, sum/n);
	lua_pushnumber(L, sum);

	// 这个返回值是告诉lua虚拟机返回值的个数，以便支持多返回值函数调用
	return 2;
}

int main(int argc, char const *argv[])
{
	lua_State *L = luaL_newstate();
	// 加载这个方法避免lua中无法调用系统库的错误，http://lua-users.org/lists/lua-l/2014-05/msg00279.html
	// 在"lualib.h"中，有各种各样的基础库的类型，如果仅仅只是使用一些基本库，可以只加载比如"base"库即可，参考：http://cn.cocos2d-x.org/tutorial/show?id=1213
	// 这里我们偷懒一下，把所有的lua的基本库都加载进去
	luaL_openlibs(L); 

	lua_register(L, "average", average);

	char filePath[100];
	getcwd(filePath, 100);
	strcat(filePath, "/call_c.lua");
	printf("file path: %s\n", filePath);

	if (luaL_loadfile(L, filePath)) { 
		printf("luaL_loadfile error\n");
	}

	if ( lua_pcall(L, 0, 0, 0)) {
		printf("lua_pcall error\n");
	}

	lua_getglobal(L, "result");
	if (!lua_istable(L, -1)) {
		printf("lua_istable error");
	}
	getfield(L, "avg");
	getfield(L, "sum");

	lua_close(L);
	L = NULL;

	return 0;
}