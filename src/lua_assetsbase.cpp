/*
*                                                                          
* DrawSpace Rendering engine                                               
* Emmanuel Chaumont Copyright (c) 2013-2014                                
*                                                                          
* This file is part of DrawSpace.                                          
*                                                                          
*    DrawSpace is free software: you can redistribute it and/or modify     
*    it under the terms of the GNU General Public License as published by  
*    the Free Software Foundation, either version 3 of the License, or     
*    (at your option) any later version.                                   
*                                                                          
*    DrawSpace is distributed in the hope that it will be useful,          
*    but WITHOUT ANY WARRANTY; without even the implied warranty of        
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         
*    GNU General Public License for more details.                          
*                                                                          
*    You should have received a copy of the GNU General Public License     
*    along with DrawSpace.  If not, see <http://www.gnu.org/licenses/>.    
*                                                                          
*/

#include "lua_assetsbase.h"
#include "memalloc.h"

using namespace DrawSpace;
using namespace DrawSpace::Core;

const char LuaAssetsBase::className[] = "AssetsBase";
const DrawSpace::Luna<LuaAssetsBase>::RegType LuaAssetsBase::Register[] =
{
    { "RegisterAssetObject", &LuaAssetsBase::Lua_RegisterAssetObject },
    { "GetAssetObject", &LuaAssetsBase::Lua_GetAssetObject },
    { 0 }
};


LuaAssetsBase::LuaAssetsBase( lua_State* p_L ) 
{
    m_assetsbase = _DRAWSPACE_NEW_( AssetsBase, AssetsBase );
}

LuaAssetsBase::~LuaAssetsBase( void ) 
{
    _DRAWSPACE_DELETE_( m_assetsbase );
}

int LuaAssetsBase::Lua_RegisterAssetObject( lua_State* p_L )
{
	int argc = lua_gettop( p_L );
	if( argc != 3 )
	{
		lua_pushstring( p_L, "RegisterAssetObject : bad number of args" );
		lua_error( p_L );
	}

    const char* id = luaL_checkstring( p_L, 2 );
    Asset* asset = (Asset*)luaL_checkinteger( p_L, 3 );
    m_assetsbase->RegisterAsset( id, asset );
    return 0;
}

int LuaAssetsBase::Lua_GetAssetObject( lua_State* p_L )
{
	int argc = lua_gettop( p_L );
	if( argc != 2 )
	{
		lua_pushstring( p_L, "GetAssetObject : bad number of args" );
		lua_error( p_L );
	}

    const char* id = luaL_checkstring( p_L, 2 );

    Asset* asset = m_assetsbase->GetAsset( id );

    if( NULL == asset )
    {
		lua_pushstring( p_L, "GetAssetObject : unknown id" );
		lua_error( p_L );
    }

    lua_pushunsigned( p_L, (lua_Unsigned)asset );
    return 1;
}
