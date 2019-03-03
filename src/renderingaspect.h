/* -*-LIC_BEGIN-*- */
/*
*                                                                          
* DrawSpace Rendering engine                                               
* Emmanuel Chaumont Copyright (c) 2013-2019                     
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

#ifndef _RENDERINGASPECT_H_
#define _RENDERINGASPECT_H_

#include "aspect.h"
#include "renderingaspectimpl.h"

namespace DrawSpace
{
namespace Aspect
{
class RenderingAspect : public Core::Aspect
{
protected:
    std::vector<DrawSpace::Interface::AspectImplementations::RenderingAspectImpl*>   m_impls;
    EntityGraph::EntityNodeGraph*                                                    m_entitynodegraph;

public:
    RenderingAspect( void );

    void AddImplementation( DrawSpace::Interface::AspectImplementations::RenderingAspectImpl* p_impl );
    void RemoveImplementation( DrawSpace::Interface::AspectImplementations::RenderingAspectImpl* p_impl );

    void Run( Core::Entity* p_owner_entity, bool p_drawtextlements );

    void OnAddedInGraph(EntityGraph::EntityNodeGraph* p_entitynodegraph);
    void OnRemovedFromGraph(EntityGraph::EntityNodeGraph* p_entitynodegraph);

    void ComponentsUpdated(void);

};
}
}

#endif
