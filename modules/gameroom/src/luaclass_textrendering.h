/* -*-LIC_BEGIN-*- */
/*
*                                                                          
* DrawSpace Rendering engine                                               
* Emmanuel Chaumont Copyright (c) 2013-2021                     
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
/* -*-LIC_END-*- */

#ifndef _LUACLASS_TEXTRENDERINGASPECTIMPL_H_
#define _LUACLASS_TEXTRENDERINGASPECTIMPL_H_

#include "luna.h"
#include "textrenderingaspectimpl.h"
#include "luaclass_entity.h"
#include "renderingaspect.h"

class LuaClass_TextRendering
{
private:
    
    DrawSpace::AspectImplementations::TextRenderingAspectImpl   m_text_render;
    DrawSpace::Aspect::RenderingAspect*                         m_entity_rendering_aspect;
    dsstring                                                    m_id;

public:

	LuaClass_TextRendering( lua_State* p_L );
	~LuaClass_TextRendering( void );

    int LUA_configure( lua_State* p_L );
    int LUA_release( lua_State* p_L );

    int LUA_update( lua_State* p_L );
   
    static const char className[];
    static const Luna<LuaClass_TextRendering>::RegType methods[];

};

#endif
