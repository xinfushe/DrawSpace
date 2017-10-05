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

#include "mainloopservice.h"
#include "drawspace.h"

using namespace DrawSpace;
using namespace DrawSpace::Core;
using namespace DrawSpace::EntityGraph;
using namespace DrawSpace::Aspect;
using namespace DrawSpace::AspectImplementations;
using namespace DrawSpace::Utils;
using namespace DrawSpace::Interface::Module;

_DECLARE_DS_LOGGER( logger, "test01mainloopservice", NULL )

MainLoopService::MainLoopService( void ) :
m_fps_transformer( m_tm ),
m_free_transformer( m_tm ),
m_left_mousebutton( false ),
m_right_mousebutton( false )
{
}

MainLoopService::~MainLoopService( void )
{
}

void MainLoopService::GetKeys( std::vector<DrawSpace::Module::KeySinkBase*>& p_keys )
{

}

void MainLoopService::Init( DrawSpace::Logger::Configuration* p_logconf, 
                            DrawSpace::Core::BaseCallback<void, bool>* p_mousecircularmode_cb, 
                            DrawSpace::Core::BaseCallback<void, bool>* p_mousevisible_cb, 
                            DrawSpace::Core::BaseCallback<void, int>* p_closeapp_cb )
{
    m_roty = 0.0;

    (*p_mousecircularmode_cb)( true )
        ;
    p_logconf->RegisterSink( &logger );
    logger.SetConfiguration( p_logconf );

    p_logconf->RegisterSink( MemAlloc::GetLogSink() );
    MemAlloc::GetLogSink()->SetConfiguration( p_logconf );

    /////////////////////////////////////////////////////////////////////////////////

    m_renderer = DrawSpace::Core::SingletonPlugin<DrawSpace::Interface::Renderer>::GetInstance()->m_interface;

    m_renderer->GetDescr( m_pluginDescr );

    m_renderer->SetRenderState( &DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::SETCULLING, "cw" ) );
    m_renderer->SetRenderState( &DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::SETTEXTUREFILTERTYPE, "linear" ) );
    m_renderer->SetRenderState( &DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::ENABLEZBUFFER, "false" ) );


    m_meshe_import = new DrawSpace::Utils::AC3DMesheImport();

    /////////////////////////////////////////////////////////////////////////////////

    m_finalpass = m_rendergraph.CreateRoot( "final_pass" );

    m_finalpass.CreateViewportQuad();

    m_finalpass.GetViewportQuad()->SetFx( _DRAWSPACE_NEW_( Fx, Fx ) );

    RenderStatesSet finalpass_rss;
    finalpass_rss.AddRenderStateIn( DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::SETTEXTUREFILTERTYPE, "point" ) );
    finalpass_rss.AddRenderStateOut( DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::SETTEXTUREFILTERTYPE, "linear" ) );
    //finalpass_rss.AddRenderStateIn( DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::SETFILLMODE, "line" ) );
    //finalpass_rss.AddRenderStateOut( DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::SETFILLMODE, "solid" ) );

    m_finalpass.GetViewportQuad()->GetFx()->SetRenderStates( finalpass_rss );


    m_finalpass.GetViewportQuad()->GetFx()->AddShader( _DRAWSPACE_NEW_( Shader, Shader( "texture.vso", true ) ) );
    m_finalpass.GetViewportQuad()->GetFx()->AddShader( _DRAWSPACE_NEW_( Shader, Shader( "texture.pso", true ) ) );

    m_finalpass.GetViewportQuad()->GetFx()->GetShader( 0 )->LoadFromFile();
    m_finalpass.GetViewportQuad()->GetFx()->GetShader( 1 )->LoadFromFile();

    m_finalpass.GetRenderingQueue()->EnableDepthClearing( false );
    m_finalpass.GetRenderingQueue()->EnableTargetClearing( false );




    m_texturepass = m_finalpass.CreateChild( "texture_pass", 0 );

    //m_texturepass.GetRenderingQueue()->SetTargetClearingColor( 0, 0, 200, 255 );
    m_texturepass.GetRenderingQueue()->EnableDepthClearing( true );
    m_texturepass.GetRenderingQueue()->EnableTargetClearing( false );


    //////////////////////////////////////////////////////////////////////////
    
    m_rootEntity.AddAspect<RenderingAspect>();
    RenderingAspect* rendering_aspect = m_rootEntity.GetAspect<RenderingAspect>();
    rendering_aspect->AddImplementation( &m_passesRender );

    m_passesRender.SetRendergraph( &m_rendergraph );

    rendering_aspect->AddImplementation( &m_textRender );
    rendering_aspect->AddComponent<TextRenderingAspectImpl::TextDisplay>( "fps", 10, 10, 0, 255, 0, "..." );

    m_rootEntityNode = m_entitygraph.SetRoot( &m_rootEntity );

    //////////////////////////////////////////////////////////////////////////

    m_cubeEntity.AddAspect<RenderingAspect>();
    rendering_aspect = m_cubeEntity.GetAspect<RenderingAspect>();

    rendering_aspect->AddImplementation( &m_cubeRender );

    rendering_aspect->AddComponent<MesheRenderingAspectImpl::PassSlot>( "cube_texturepass_slot", "texture_pass" );

    

    RenderingNode* cube_texturepass = rendering_aspect->GetComponent<MesheRenderingAspectImpl::PassSlot>( "cube_texturepass_slot" )->getPurpose().GetRenderingNode();
    cube_texturepass->SetFx( _DRAWSPACE_NEW_( Fx, Fx ) );

    cube_texturepass->GetFx()->AddShader( _DRAWSPACE_NEW_( Shader, Shader( "texture.vso", true ) ) );
    cube_texturepass->GetFx()->AddShader( _DRAWSPACE_NEW_( Shader, Shader( "texture.pso", true ) ) );

    cube_texturepass->GetFx()->GetShader( 0 )->LoadFromFile();
    cube_texturepass->GetFx()->GetShader( 1 )->LoadFromFile();

    RenderStatesSet cube_texturepass_rss;
    cube_texturepass_rss.AddRenderStateIn( DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::ENABLEZBUFFER, "true" ) );
    cube_texturepass_rss.AddRenderStateOut( DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::ENABLEZBUFFER, "false" ) );

    cube_texturepass->GetFx()->SetRenderStates( cube_texturepass_rss );


    cube_texturepass->SetMeshe( _DRAWSPACE_NEW_( Meshe, Meshe ) );
    cube_texturepass->GetMeshe()->SetImporter( m_meshe_import );
    cube_texturepass->GetMeshe()->LoadFromFile( "object.ac", 0 );

    cube_texturepass->SetTexture( _DRAWSPACE_NEW_( Texture, Texture( "bellerophon.jpg" ) ), 0 );
    cube_texturepass->GetTexture( 0 )->LoadFromFile();

    m_cubeEntity.AddAspect<WorldAspect>();

    WorldAspect* world_aspect = m_cubeEntity.GetAspect<WorldAspect>();

    world_aspect->AddImplementation( &m_transformer );  
    world_aspect->AddComponent<Matrix>( "cube_rotation" );
    world_aspect->AddComponent<Matrix>( "cube_translation" );

    world_aspect->GetComponent<Matrix>( "cube_translation" )->getPurpose().Translation( Vector( 0.0, 0.0, -6.0, 1.0 ) );
    world_aspect->GetComponent<Matrix>( "cube_rotation" )->getPurpose().Rotation( Vector( 0.0, 1.0, 0.0, 1.0 ), Utils::Maths::DegToRad( m_roty ) );

    ///////////////////////////////////////////////////////////////////////////

    m_cameraEntity.AddAspect<WorldAspect>();
    world_aspect = m_cameraEntity.GetAspect<WorldAspect>();


    world_aspect->AddImplementation( &m_fps_transformer );
    world_aspect->AddComponent<dsreal>( "yaw", 0.0 );
    world_aspect->AddComponent<dsreal>( "pitch", 0.0 );

    world_aspect->AddComponent<Vector>( "speed" );
    world_aspect->AddComponent<Matrix>( "pos" );

    world_aspect->GetComponent<Matrix>( "pos" )->getPurpose().Translation( Vector( 0.0, 0.0, 10.0, 1.0 ) );

    world_aspect->AddComponent<bool>( "ymvt", true );

    m_cameraEntity.AddAspect<CameraAspect>();

    CameraAspect* camera_aspect = m_cameraEntity.GetAspect<CameraAspect>();
    camera_aspect->AddComponent<Matrix>( "camera_proj" );

    DrawSpace::Interface::Renderer::Characteristics characteristics;
    m_renderer->GetRenderCharacteristics( characteristics );
    camera_aspect->GetComponent<Matrix>( "camera_proj" )->getPurpose().Perspective( characteristics.width_viewport, characteristics.height_viewport, 1.0, 100000.0 );


    ///////////////////////////////////////////////////////////////////////////

    m_camera2Entity.AddAspect<WorldAspect>();
    world_aspect = m_camera2Entity.GetAspect<WorldAspect>();

    world_aspect->AddImplementation( &m_free_transformer );
  
    world_aspect->AddComponent<dsreal>( "rspeed_x", 0.0 );
    world_aspect->AddComponent<dsreal>( "rspeed_y", 0.0 );
    world_aspect->AddComponent<dsreal>( "rspeed_z", 0.0 );

    world_aspect->AddComponent<Vector>( "speed" );
    world_aspect->AddComponent<Vector>( "rot_axis_x", Vector( 1.0, 0.0, 0.0, 1.0 ) );
    world_aspect->AddComponent<Vector>( "rot_axis_y", Vector( 0.0, 1.0, 0.0, 1.0 ) );
    world_aspect->AddComponent<Vector>( "rot_axis_z", Vector( 0.0, 0.0, 1.0, 1.0 ) );

    world_aspect->AddComponent<Matrix>( "pos" );
    world_aspect->GetComponent<Matrix>( "pos" )->getPurpose().Translation( Vector( 0.0, 2.0, 5.0, 1.0 ) );

    world_aspect->AddComponent<Quaternion>( "quat" );
    world_aspect->GetComponent<Quaternion>( "quat" )->getPurpose().Identity();
    


    m_camera2Entity.AddAspect<CameraAspect>();
    camera_aspect = m_camera2Entity.GetAspect<CameraAspect>();
    camera_aspect->AddComponent<Matrix>( "camera_proj" );

    m_renderer->GetRenderCharacteristics( characteristics );
    camera_aspect->GetComponent<Matrix>( "camera_proj" )->getPurpose().Perspective( characteristics.width_viewport, characteristics.height_viewport, 1.0, 100000.0 );


    ///////////////////////////////////////////////////////////////////////////

    m_skyboxEntity.AddAspect<RenderingAspect>();
    rendering_aspect = m_skyboxEntity.GetAspect<RenderingAspect>();

    rendering_aspect->AddImplementation( &m_skyboxRender );

    rendering_aspect->AddComponent<SkyboxRenderingAspectImpl::PassSlot>( "skybox_texturepass_slot", "texture_pass" );

    Fx* skybox_texturepass_fx = _DRAWSPACE_NEW_( Fx, Fx );

    skybox_texturepass_fx->AddShader( _DRAWSPACE_NEW_( Shader, Shader( "texture.vso", true ) ) );
    skybox_texturepass_fx->AddShader( _DRAWSPACE_NEW_( Shader, Shader( "texture.pso", true ) ) );

    skybox_texturepass_fx->GetShader( 0 )->LoadFromFile();
    skybox_texturepass_fx->GetShader( 1 )->LoadFromFile();

    RenderStatesSet skybox_texturepass_rss;
    skybox_texturepass_rss.AddRenderStateIn( DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::ENABLEZBUFFER, "false" ) );
    skybox_texturepass_rss.AddRenderStateOut( DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::ENABLEZBUFFER, "false" ) );

    skybox_texturepass_fx->SetRenderStates( skybox_texturepass_rss );

    for( int i = 0; i < 6; i++ )
    {
        RenderingNode* skybox_texturepass = rendering_aspect->GetComponent<SkyboxRenderingAspectImpl::PassSlot>( "skybox_texturepass_slot" )->getPurpose().GetRenderingNode( i );
        skybox_texturepass->SetOrderNumber( -1000 );
        skybox_texturepass->SetFx( skybox_texturepass_fx );
    }

    rendering_aspect->GetComponent<SkyboxRenderingAspectImpl::PassSlot>( "skybox_texturepass_slot" )->getPurpose().GetRenderingNode( SkyboxRenderingAspectImpl::PassSlot::FrontQuad )->SetTexture( _DRAWSPACE_NEW_( Texture, Texture( "sb0.bmp" ) ), 0 );
    rendering_aspect->GetComponent<SkyboxRenderingAspectImpl::PassSlot>( "skybox_texturepass_slot" )->getPurpose().GetRenderingNode( SkyboxRenderingAspectImpl::PassSlot::FrontQuad )->GetTexture( 0 )->LoadFromFile();

    rendering_aspect->GetComponent<SkyboxRenderingAspectImpl::PassSlot>( "skybox_texturepass_slot" )->getPurpose().GetRenderingNode( SkyboxRenderingAspectImpl::PassSlot::RearQuad )->SetTexture( _DRAWSPACE_NEW_( Texture, Texture( "sb2.bmp" ) ), 0 );
    rendering_aspect->GetComponent<SkyboxRenderingAspectImpl::PassSlot>( "skybox_texturepass_slot" )->getPurpose().GetRenderingNode( SkyboxRenderingAspectImpl::PassSlot::RearQuad )->GetTexture( 0 )->LoadFromFile();

    rendering_aspect->GetComponent<SkyboxRenderingAspectImpl::PassSlot>( "skybox_texturepass_slot" )->getPurpose().GetRenderingNode( SkyboxRenderingAspectImpl::PassSlot::LeftQuad )->SetTexture( _DRAWSPACE_NEW_( Texture, Texture( "sb3.bmp" ) ), 0 );
    rendering_aspect->GetComponent<SkyboxRenderingAspectImpl::PassSlot>( "skybox_texturepass_slot" )->getPurpose().GetRenderingNode( SkyboxRenderingAspectImpl::PassSlot::LeftQuad )->GetTexture( 0 )->LoadFromFile();

    rendering_aspect->GetComponent<SkyboxRenderingAspectImpl::PassSlot>( "skybox_texturepass_slot" )->getPurpose().GetRenderingNode( SkyboxRenderingAspectImpl::PassSlot::RightQuad )->SetTexture( _DRAWSPACE_NEW_( Texture, Texture( "sb1.bmp" ) ), 0 );
    rendering_aspect->GetComponent<SkyboxRenderingAspectImpl::PassSlot>( "skybox_texturepass_slot" )->getPurpose().GetRenderingNode( SkyboxRenderingAspectImpl::PassSlot::RightQuad )->GetTexture( 0 )->LoadFromFile();

    rendering_aspect->GetComponent<SkyboxRenderingAspectImpl::PassSlot>( "skybox_texturepass_slot" )->getPurpose().GetRenderingNode( SkyboxRenderingAspectImpl::PassSlot::TopQuad )->SetTexture( _DRAWSPACE_NEW_( Texture, Texture( "sb4.bmp" ) ), 0 );
    rendering_aspect->GetComponent<SkyboxRenderingAspectImpl::PassSlot>( "skybox_texturepass_slot" )->getPurpose().GetRenderingNode( SkyboxRenderingAspectImpl::PassSlot::TopQuad )->GetTexture( 0 )->LoadFromFile();

    rendering_aspect->GetComponent<SkyboxRenderingAspectImpl::PassSlot>( "skybox_texturepass_slot" )->getPurpose().GetRenderingNode( SkyboxRenderingAspectImpl::PassSlot::BottomQuad )->SetTexture( _DRAWSPACE_NEW_( Texture, Texture( "sb4.bmp" ) ), 0 );
    rendering_aspect->GetComponent<SkyboxRenderingAspectImpl::PassSlot>( "skybox_texturepass_slot" )->getPurpose().GetRenderingNode( SkyboxRenderingAspectImpl::PassSlot::BottomQuad )->GetTexture( 0 )->LoadFromFile();


    m_skyboxEntity.AddAspect<WorldAspect>();
    world_aspect = m_skyboxEntity.GetAspect<WorldAspect>();
    world_aspect->AddImplementation( &m_transformer );

    world_aspect->AddComponent<Matrix>( "skybox_scaling" );

    world_aspect->GetComponent<Matrix>( "skybox_scaling" )->getPurpose().Scale( 100.0, 100.0, 100.0 );

    ///////////////////////////////////////////////////////////////////////////

    // ajouter la skybox a la scene

    m_skyboxEntityNode = m_rootEntityNode.AddChild( &m_skyboxEntity );
    m_skyboxRender.RegisterToRendering( m_rendergraph );

    // ajouter le cube a la scene
    m_cubeEntityNode = m_rootEntityNode.AddChild( &m_cubeEntity );
    m_cubeRender.RegisterToRendering( m_rendergraph );

    // ajouter la camera a la scene
    m_cameraEntityNode = m_rootEntityNode.AddChild( &m_cameraEntity );

    m_camera2EntityNode = m_rootEntityNode.AddChild( &m_camera2Entity );



    //m_worldSystem.SetCurrentCameraEntity( &m_cameraEntity );
    m_worldSystem.SetCurrentCameraEntity( &m_camera2Entity );

    m_rendergraph.RenderingQueueModSignal();

    _DSDEBUG( logger, dsstring("main loop service : startup...") );
}

void MainLoopService::Run( void )
{
    m_worldSystem.Run( &m_entitygraph );
    m_renderingSystem.Run( &m_entitygraph );
    m_renderer->FlipScreen();

    if( m_display_switch )
    {      
        char comment[256];
        sprintf( comment, "%d fps - %s", m_tm.GetFPS(), m_pluginDescr.c_str() );

        RenderingAspect* rendering_aspect = m_rootEntity.GetAspect<RenderingAspect>();

        rendering_aspect->GetComponent<TextRenderingAspectImpl::TextDisplay>( "fps" )->getPurpose().m_text = comment;
    }

    m_tm.Update();
    if( m_tm.IsReady() )
    {

        /*
        m_tm.AngleSpeedInc( &m_roty, 15 );

        WorldAspect* world_aspect = m_cubeEntity.GetAspect<WorldAspect>();
        world_aspect->GetComponent<Matrix>( "cube_rotation" )->getPurpose().Rotation( Vector( 0.0, 1.0, 0.0, 1.0 ), Utils::Maths::DegToRad( m_roty ) );
        */
    }
    
    WorldAspect* world_aspect = m_camera2Entity.GetAspect<WorldAspect>();
    world_aspect->GetComponent<dsreal>( "rspeed_x" )->getPurpose() = 0.0;
    world_aspect->GetComponent<dsreal>( "rspeed_y" )->getPurpose() = 0.0;
    world_aspect->GetComponent<dsreal>( "rspeed_z" )->getPurpose() = 0.0;
    
}

void MainLoopService::Release( void )
{
    _DSDEBUG( logger, dsstring("main loop service : shutdown...") );

}

DrawSpace::Core::BaseSceneNode* MainLoopService::InstanciateSceneNode( const dsstring& p_sceneNodeName, DrawSpace::Dynamics::Calendar* p_calendar, LODDependantNodeInfoStateHandler* p_handler )
{
    return NULL;
}

void MainLoopService::RegisterScenegraphCallbacks( DrawSpace::Core::SceneNodeGraph& p_scenegraph )
{
}

void MainLoopService::UnregisterScenegraphCallbacks( DrawSpace::Core::SceneNodeGraph& p_scenegraph )
{ 
}

void MainLoopService::ReleaseSceneNode( const dsstring& p_sceneNodeName, DrawSpace::Dynamics::Calendar* p_calendar )
{
}

void MainLoopService::OnKeyPress( long p_key )
{   
    switch( p_key )
    {
        case 'Q':
        {
            
            WorldAspect* world_aspect = m_camera2Entity.GetAspect<WorldAspect>();
            world_aspect->GetComponent<Vector>( "speed" )->getPurpose()[2] = 2.0;
            

            //m_free_transformer.SetSpeed( 10.0 );
        }
        break;

        case 'W':
        {
            
            WorldAspect* world_aspect = m_camera2Entity.GetAspect<WorldAspect>();
            world_aspect->GetComponent<Vector>( "speed" )->getPurpose()[2] = -2.0;
            

            //m_free_transformer.SetSpeed( -2.0 );
        }
        break;
    }
}

void MainLoopService::OnEndKeyPress( long p_key )
{
    switch( p_key )
    {
        case 'Q':
        case 'W':
        {
            
            WorldAspect* world_aspect = m_camera2Entity.GetAspect<WorldAspect>();
            world_aspect->GetComponent<Vector>( "speed" )->getPurpose()[2] = 0.0; 
            

            //m_free_transformer.SetSpeed( 0.0 );
        }
        break;      
    }
}

void MainLoopService::OnKeyPulse( long p_key )
{
    switch( p_key )
    {
        case VK_F1:
            {
                
                if( m_display_switch )
                {
                    RenderingAspect* renderingAspect = m_rootEntity.GetAspect<RenderingAspect>();
                    renderingAspect->RemoveComponent<TextRenderingAspectImpl::TextDisplay>( "fps");

                    m_display_switch = false;
                }
                else
                {
                    RenderingAspect* renderingAspect = m_rootEntity.GetAspect<RenderingAspect>();
                    renderingAspect->AddComponent<TextRenderingAspectImpl::TextDisplay>( "fps", 10, 10, 0, 255, 0, "..." );

                    m_display_switch = true;
                }
                
            }       
            break;
    }
}

void MainLoopService::OnChar( long p_char, long p_scan )
{
}

void MainLoopService::OnMouseMove( long p_xm, long p_ym, long p_dx, long p_dy )
{


    WorldAspect* world_aspect = m_camera2Entity.GetAspect<WorldAspect>();

    if( m_left_mousebutton )
    {
        world_aspect->GetComponent<dsreal>( "rspeed_x" )->getPurpose() = - p_dy / 4.0;
        world_aspect->GetComponent<dsreal>( "rspeed_y" )->getPurpose() = - p_dx / 4.0;
    }
    else if( m_right_mousebutton )
    {
        world_aspect->GetComponent<dsreal>( "rspeed_z" )->getPurpose() = - p_dx;
    }
}

void MainLoopService::OnMouseWheel( long p_delta )
{
}


void MainLoopService::OnMouseLeftButtonDown( long p_xm, long p_ym )
{
    m_left_mousebutton = true;
}

void MainLoopService::OnMouseLeftButtonUp( long p_xm, long p_ym )
{
    m_left_mousebutton = false;
}

void MainLoopService::OnMouseRightButtonDown( long p_xm, long p_ym )
{
    m_right_mousebutton = true;
}

void MainLoopService::OnMouseRightButtonUp( long p_xm, long p_ym )
{
    m_right_mousebutton = false;
}

void MainLoopService::OnAppEvent( WPARAM p_wParam, LPARAM p_lParam )
{
}


