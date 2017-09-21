/*
*                                                                          
* DrawSpace Rendering engine                                               
* Emmanuel Chaumont Copyright (c) 2013-2017                        
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

#include "renderingsystem.h"
//#include "screenrenderingaspect.h"
#include "renderingaspect.h"

using namespace DrawSpace;
using namespace DrawSpace::Core;

RenderingSystem::RenderingSystem(void)
{
}

RenderingSystem::~RenderingSystem(void)
{
}

void RenderingSystem::Run( EntityNodeGraph* p_entitygraph )
{
    p_entitygraph->AcceptRenderingSystem( this );
}

void RenderingSystem::VisitEntity( Entity* p_entity ) const
{
    RenderingAspect* rendering_aspect = p_entity->GetAspect<RenderingAspect>();
    if( rendering_aspect )
    {
        rendering_aspect->draw( p_entity );
    }
}