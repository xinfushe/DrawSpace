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

#ifndef _TRANSFORMASPECT_H_
#define _TRANSFORMASPECT_H_

#include "worldaspectimpl.h"
#include "entity.h"

namespace DrawSpace
{
namespace Aspect
{
class TransformAspect : public Core::Aspect
{
protected:
    DrawSpace::Utils::Matrix                                                        m_worldtransform;

    // transfos view et proj "distribuees" par le worldsystem
    DrawSpace::Utils::Matrix                                                        m_dispatched_viewtransform;
    DrawSpace::Utils::Matrix                                                        m_dispatched_projtransform;


    std::vector<DrawSpace::Interface::AspectImplementations::WorldAspectImpl*>      m_impls;

public:
    TransformAspect( void );

    void AddImplementation( DrawSpace::Interface::AspectImplementations::WorldAspectImpl* p_impl );
    
    void GetWorldTransform( DrawSpace::Utils::Matrix& p_worldtransform );
    void GetViewTransform( DrawSpace::Utils::Matrix& p_viewtransform );
    void GetProjTransform( DrawSpace::Utils::Matrix& p_projtransform );

    void ComputeTransforms( Core::Entity* p_parent, Core::Entity* p_entity );
    void DispatchViewProj( const DrawSpace::Utils::Matrix& p_view, DrawSpace::Utils::Matrix& p_proj );
};
}
}

#endif