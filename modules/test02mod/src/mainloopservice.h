/*
*                                                                          
* DrawSpace Rendering engine                                               
* Emmanuel Chaumont Copyright (c) 2013-2016                              
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

#ifndef _MAINLOOPSERVICE_H_
#define _MAINLOOPSERVICE_H_

#include "module_service.h"
#include "drawspace.h"

class MainLoopService : public DrawSpace::Interface::Module::Service
{
protected:

    DrawSpace::Interface::Renderer*                                     m_renderer;
    DrawSpace::Utils::TimeManager                                       m_tm;
    dsstring                                                            m_device;
    dsstring                                                            m_pluginDescr;

    DrawSpace::FinalPass*                                               m_finalpass;
    DrawSpace::IntermediatePass*                                        m_texturepass;

    DrawSpace::Spacebox*                                                m_spacebox;
    DrawSpace::Core::SceneNode<DrawSpace::Spacebox>*                    m_spacebox_node;
    DrawSpace::Core::SceneNode<DrawSpace::Core::Transformation>*        m_spacebox_transfo_node;


    DrawSpace::Dynamics::CameraPoint*                                   m_camera;
    DrawSpace::Core::SceneNode<DrawSpace::Dynamics::CameraPoint>*       m_camera_node;

    DrawSpace::Core::FPSMovement                                        m_fpsmove;
    DrawSpace::Core::SceneNode<DrawSpace::Core::FPSMovement>*           m_fpsmove_node;

    DrawSpace::Core::SceneNodeGraph                                     m_scenenodegraph;

    void    create_passes( void );
    void    init_passes( void );

    void    create_spacebox( void );
    void    create_camera( void );

public:
    MainLoopService( void );
    ~MainLoopService( void );

    virtual void                            GetLocalKeys( const dsstring p_instanceName, std::vector<DrawSpace::Module::KeySinkBase*>& p_keys );
    virtual void                            GetGlobalKeys( std::vector<DrawSpace::Module::KeySinkBase*>& p_keys );
    virtual void                            Init( DrawSpace::Logger::Configuration* p_logconf );
    virtual void                            Run( void );
    virtual void                            Release( void );
    virtual DrawSpace::Core::BaseSceneNode* GetSceneNode( void );

    virtual void                            OnKeyPress( long p_key );
    virtual void                            OnEndKeyPress( long p_key );
    virtual void                            OnKeyPulse( long p_key );
    virtual void                            OnMouseMove( long p_xm, long p_ym, long p_dx, long p_dy );
    virtual void                            OnMouseLeftButtonDown( long p_xm, long p_ym );
    virtual void                            OnMouseLeftButtonUp( long p_xm, long p_ym );
    virtual void                            OnMouseRightButtonDown( long p_xm, long p_ym );
    virtual void                            OnMouseRightButtonUp( long p_xm, long p_ym );
    virtual void                            OnAppEvent( WPARAM p_wParam, LPARAM p_lParam );
};

#endif