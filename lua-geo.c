#include "lua.h"
#include "lauxlib.h"
#include "geo.h"

static int
lua_geo_cell_at(lua_State *L)
{
    int x = (int)luaL_checkinteger(L, 1);
    int y = (int)luaL_checkinteger(L, 2);
    int cell_size = (int)luaL_checkinteger(L, 3);
    int cells_per_row = (int)luaL_checkinteger(L, 4);
    lua_pushinteger(L, geo_cell_at(x, y, cell_size, cells_per_row));
    return 1;
}

static int
lua_geo_cell_origin(lua_State *L)
{
    int idx = (int)luaL_checkinteger(L, 1);
    int cell_size = (int)luaL_checkinteger(L, 2);
    int cells_per_row = (int)luaL_checkinteger(L, 3);
    int ox, oy;
    geo_cell_origin(idx, cell_size, cells_per_row, &ox, &oy);
    lua_newtable(L);
    lua_pushinteger(L, ox);
    lua_setfield(L, -2, "x");
    lua_pushinteger(L, oy);
    lua_setfield(L, -2, "y");
    return 1;
}

static int
lua_geo_dist(lua_State *L)
{
    double x1 = luaL_checknumber(L, 1);
    double y1 = luaL_checknumber(L, 2);
    double x2 = luaL_checknumber(L, 3);
    double y2 = luaL_checknumber(L, 4);
    lua_pushnumber(L, geo_dist(x1, y1, x2, y2));
    return 1;
}

static int
lua_geo_line_cells(lua_State *L)
{
    double x1 = luaL_checknumber(L, 1);
    double y1 = luaL_checknumber(L, 2);
    double x2 = luaL_checknumber(L, 3);
    double y2 = luaL_checknumber(L, 4);
    double speed = luaL_checknumber(L, 5);
    int cell_size = (int)luaL_checkinteger(L, 6);
    int64_t start_time = (int64_t)luaL_checkinteger(L, 7);
    int cells_per_row = (int)luaL_checkinteger(L, 8);

    struct geo_line_result *r = geo_line_cells(x1, y1, x2, y2, speed,
                                               cell_size, cells_per_row,
                                               start_time);
    if (r == NULL)
        return luaL_error(L, "line_cells: allocation failed");

    lua_newtable(L);

    lua_newtable(L);
    for (int i = 0; i < r->count; i++) {
        lua_newtable(L);
        lua_pushinteger(L, r->cells[i].idx);
        lua_setfield(L, -2, "idx");
        lua_pushinteger(L, r->cells[i].enter);
        lua_setfield(L, -2, "enter");
        lua_pushinteger(L, r->cells[i].leave);
        lua_setfield(L, -2, "leave");
        lua_rawseti(L, -2, i + 1);
    }
    lua_setfield(L, -2, "cells");

    lua_pushinteger(L, r->arrive_time);
    lua_setfield(L, -2, "arrive_time");

    geo_line_result_free(r);
    return 1;
}

static int
lua_geo_rect_cells(lua_State *L)
{
    int x1 = (int)luaL_checkinteger(L, 1);
    int y1 = (int)luaL_checkinteger(L, 2);
    int x2 = (int)luaL_checkinteger(L, 3);
    int y2 = (int)luaL_checkinteger(L, 4);
    int cell_size = (int)luaL_checkinteger(L, 5);
    int cells_per_row = (int)luaL_checkinteger(L, 6);

    struct geo_rect_result *r = geo_rect_cells(x1, y1, x2, y2,
                                               cell_size, cells_per_row);
    if (r == NULL)
        return luaL_error(L, "rect_cells: allocation failed");

    lua_newtable(L);
    for (int i = 0; i < r->count; i++) {
        lua_pushinteger(L, r->cells[i]);
        lua_rawseti(L, -2, i + 1);
    }

    geo_rect_result_free(r);
    return 1;
}

static const luaL_Reg geo_functions[] = {
    {"cell_at",     lua_geo_cell_at},
    {"cell_origin", lua_geo_cell_origin},
    {"dist",        lua_geo_dist},
    {"line_cells",  lua_geo_line_cells},
    {"rect_cells",  lua_geo_rect_cells},
    {NULL, NULL}
};

int
luaopen_geo(lua_State *L)
{
    luaL_newlib(L, geo_functions);
    return 1;
}
