/*
*                                                                          
* DrawSpace Rendering engine                                               
* Emmanuel Chaumont Copyright (c) 2013-2015                              
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

#include "exceptions.h"
#include "memalloc.h"
#include "impostor.h"
#include "exceptions.h"
#include "configsbase.h"
#include "assetsbase.h"
#include "misc_utils.h"

using namespace DrawSpace;
using namespace DrawSpace::Interface;
using namespace DrawSpace::Core;
using namespace DrawSpace::Utils;

Impostor::Impostor( void ) :
m_scenenodegraph( NULL ),
m_meshe( NULL )
{
    m_renderer = DrawSpace::Core::SingletonPlugin<DrawSpace::Interface::Renderer>::GetInstance()->m_interface;


}

Impostor::~Impostor( void )
{

}

void Impostor::Init( const Impostor::DisplayList& p_list )
{
    m_meshe = _DRAWSPACE_NEW_( Core::Meshe, Core::Meshe );


    for( size_t i = 0; i < p_list.size(); i++ )
    {
        Vertex v1, v2, v3, v4;

        v1.x = -0.5;
        v1.y = 0.5;
        v1.z = 0.0;
        v1.tu[0] = p_list[i].u1;
        v1.tv[0] = p_list[i].v1;
        v1.nx = 1.0;
        v1.tu[7] = p_list[i].localpos[0];
        v1.tv[7] = p_list[i].localpos[1];
        v1.tw[7] = p_list[i].localpos[2];
        v1.tu[8] = p_list[i].width_scale;
        v1.tv[8] = p_list[i].height_scale;

        v2.x = 0.5;
        v2.y = 0.5;
        v2.z = 0.0;
        v2.tu[0] = p_list[i].u2;
        v2.tv[0] = p_list[i].v2;
        v2.nx = 2.0;
        v2.tu[7] = p_list[i].localpos[0];
        v2.tv[7] = p_list[i].localpos[1];
        v2.tw[7] = p_list[i].localpos[2];
        v2.tu[8] = p_list[i].width_scale;
        v2.tv[8] = p_list[i].height_scale;

        v3.x = 0.5;
        v3.y = -0.5;
        v3.z = 0.0;
        v3.tu[0] = p_list[i].u3;
        v3.tv[0] = p_list[i].v3;
        v3.nx = 3.0;
        v3.tu[7] = p_list[i].localpos[0];
        v3.tv[7] = p_list[i].localpos[1];
        v3.tw[7] = p_list[i].localpos[2];
        v3.tu[8] = p_list[i].width_scale;
        v3.tv[8] = p_list[i].height_scale;

        v4.x = -0.5;
        v4.y = -0.5;
        v4.z = 0.0;
        v4.tu[0] = p_list[i].u4;
        v4.tv[0] = p_list[i].v4;
        v4.nx = 4.0;
        v4.tu[7] = p_list[i].localpos[0];
        v4.tv[7] = p_list[i].localpos[1];
        v4.tw[7] = p_list[i].localpos[2];
        v4.tu[8] = p_list[i].width_scale;
        v4.tv[8] = p_list[i].height_scale;
    
        m_meshe->AddVertex( v1 );
        m_meshe->AddVertex( v2 );
        m_meshe->AddVertex( v3 );
        m_meshe->AddVertex( v4 );

        int index_base = 4 * i;

        m_meshe->AddTriangle( Triangle( index_base, 3 + index_base, 1 + index_base ) );
        m_meshe->AddTriangle( Triangle( 1 + index_base, 3 + index_base, 2 + index_base ) );
        
    }
}

void Impostor::Update( DrawSpace::Utils::TimeManager& p_timemanager )
{
}

void Impostor::Update2( DrawSpace::Utils::TimeManager& p_timemanager )
{
}

void Impostor::SetDrawingState( Pass* p_passname, bool p_drawing )
{
    if( m_passesnodes.count( p_passname ) > 0 )
    {
        m_passesnodes[p_passname]->SetDrawingState( p_drawing );
        return;
    }

    dsstring msg = "Chunk : unknown pass";
    _DSEXCEPTION( msg )
}

void Impostor::OnRegister( DrawSpace::Core::SceneNodeGraph* p_scenegraph, DrawSpace::Core::BaseSceneNode* p_node )
{
    if( NULL == m_meshe )
    {
        _DSEXCEPTION( "NULL meshe ; please allocate a meshe object for chunk prior to other operations" );
    }

    for( std::map<Pass*, RenderingNode*>::iterator it = m_passesnodes.begin(); it != m_passesnodes.end(); ++it )
    {
        Pass* current_pass = it->first;
        current_pass->GetRenderingQueue()->Add( (*it).second );
    }
    m_scenenodegraph = p_scenegraph;
}


void Impostor::RegisterPassSlot( Pass* p_pass )
{
    m_passesnodes[p_pass] = _DRAWSPACE_NEW_( RenderingNode, RenderingNode );

    if( NULL == m_meshe )
    {
        _DSEXCEPTION( "NULL meshe ; please allocate a meshe object for chunk prior to other operations" );
    }

    m_passesnodes[p_pass]->SetMeshe( m_meshe );

    RenderingNodeDrawCallback* cb = _DRAWSPACE_NEW_( RenderingNodeDrawCallback, RenderingNodeDrawCallback( this, &Impostor::on_renderingnode_draw ) );

    m_passesnodes[p_pass]->RegisterHandler( cb );
    m_callbacks.push_back( cb );
}

DrawSpace::Core::RenderingNode* Impostor::GetNodeFromPass( Pass* p_pass )
{
    if( 0 == m_passesnodes.count( p_pass ) )
    {
        return NULL;
    }
    return m_passesnodes[p_pass];
}

void Impostor::on_renderingnode_draw( DrawSpace::Core::RenderingNode* p_rendering_node )
{
    DrawSpace::Utils::Matrix view;
    DrawSpace::Utils::Matrix proj;

    view.Identity();
    if( m_scenenodegraph )
    {
        m_scenenodegraph->GetCurrentCameraView( view );
        m_scenenodegraph->GetCurrentCameraProj( proj );
    }
    m_renderer->DrawMeshe( p_rendering_node->GetMeshe()->GetVertexListSize(), p_rendering_node->GetMeshe()->GetTrianglesListSize(), m_globaltransformation, view, proj );
}


void Impostor::GetBaseTransform( DrawSpace::Utils::Matrix& p_mat )
{
    p_mat.Identity();
}

void Impostor::SetFinalTransform( const DrawSpace::Utils::Matrix& p_mat )
{
    m_globaltransformation = p_mat;
}

void Impostor::GetPassesNodesList( std::map<Pass*, DrawSpace::Core::RenderingNode*>& p_list )
{
    p_list = m_passesnodes;
}