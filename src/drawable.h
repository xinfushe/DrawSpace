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

#ifndef _DRAWABLE_H_
#define _DRAWABLE_H_

#include "transformnode.h"
#include "renderer.h"
#include "property.h"
#include "callback.h"

namespace DrawSpace
{
class Drawable : public Core::TransformNode
{
public:
    virtual void GetDescr( dsstring& p_descr ) = 0;    
    virtual void SetRenderer( Interface::Renderer * p_renderer ) = 0;
    
    virtual Core::Meshe* GetMeshe( const dsstring& p_mesheid ) = 0;
    virtual void RegisterPassSlot( const dsstring p_passname ) = 0;
    virtual Core::RenderingNode* GetNodeFromPass( const dsstring& p_passname, const dsstring& p_nodeid ) = 0;
    virtual void GetNodesIdsList( std::vector<dsstring>& p_ids ) = 0;
    virtual void SetNodeFromPassSpecificFx( const dsstring& p_passname, const dsstring& p_nodeid, const dsstring& p_fxname ) = 0;
    virtual void GetPropertiesList( std::vector<dsstring>& p_props ) = 0;
    virtual Core::Property* GetProperty( const dsstring& p_name ) = 0;
    virtual void SetProperty( const dsstring& p_name, Core::Property* p_prop ) = 0;
    virtual void Initialize( void ) = 0;
    virtual void RegisterEventHandler( DrawSpace::Core::BaseCallback<void, const dsstring&>* p_handler ) = 0;
};
}

#endif