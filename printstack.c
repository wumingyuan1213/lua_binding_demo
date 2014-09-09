#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lua.h"
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

int main(void)
{
	lua_State *L = luaL_newstate();

	// lua stack栈顶索引为最大值，通过lua_gettop()获取这个值，栈底索引为1；同时可以使用负值来进行操作，比如-1表示栈顶，而-n表示栈底部；这个索引的用法切记！！！
	lua_pushboolean(L, 1);
	lua_pushnumber(L, 10);
	lua_pushnil(L);
	// 向stack中push字符串的时候，stack会对字符串进行复制!!!
	char * str = (char *)malloc(sizeof(char) * 20);
	strcpy(str, "hello");
	lua_pushstring(L, str);
	free(str);
	
	stackDump(L); // 从栈底部开始打印： true 10 nil hello
	
	lua_pushvalue(L, -3); // 将指定索引上的副本压入栈顶部，即再压入一个1   true 10 nil hello 10
	stackDump(L);
	
	lua_replace(L, 3); // 弹出栈顶的值，并将该值放到指定的索引上，即将栈顶的1放到nil的位置上 ture 10 true hello
	stackDump(L);
	
	lua_settop(L, 6); // 设置栈顶位置，即添加两个nil, true 10 true hello nil nil
	stackDump(L);
	
	lua_remove(L, -3); // 删除指定索引上的值，并将上面的值下移，即删除hello，  true 10 true nil nil
	stackDump(L);
	
	lua_settop(L, -5); // true
	stackDump(L);
	

	lua_close(L);
	L = NULL;
	return 0;
}