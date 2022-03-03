#ifndef __lua_http_server_h__
#define __lua_http_server_h__

#ifdef __cplusplus
extern "C" {
#endif
#include "tolua++.h"
#ifdef __cplusplus
}
#endif

int lua_module_register_http_server(lua_State* tolua_S);

#endif // __lua_http_server_h__
