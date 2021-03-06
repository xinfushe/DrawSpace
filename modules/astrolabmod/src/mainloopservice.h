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

#ifndef _MAINLOOPSERVICE_H_
#define _MAINLOOPSERVICE_H_

#include "crtp_singleton.h"
#include "drawspace.h"

class MainLoopService : public DrawSpace::Interface::Module::Service, public BaseSingleton<MainLoopService>
{
public:

    typedef enum
    {
        GUIEVT_PLANETSETUP_QUITBUTTON_CLIC,
        GUIEVT_PLANETSETUP_PLANETVIEWBUTTON_CLIC,
		GUIEVT_PLANETSETUP_PLANETGROUNDSETUPBUTTON_CLIC,
        GUIEVT_PLANETSETUP_PLANETSEEDSSETUPBUTTON_CLIC,
        GUIEVT_PLANETSETUP_PLANETFOGATMOSETUPBUTTON_CLIC,
        GUIEVT_PLANETSETUP_F1_KEY,

        GUIEVT_PLANETVIEW_CLOSEBUTTON_CLIC,
		GUIEVT_PLANETGROUNDSETUP_CLOSEBUTTON_CLIC,
        GUIEVT_PLANETSEEDSSETUP_CLOSEBUTTON_CLIC,
        GUIEVT_PLANETFOGATMOSETUP_CLOSEBUTTON_CLIC,
    
    } APP_GUI_EVENT;

protected:

    DrawSpace::Interface::Renderer*                                     m_renderer;
    DrawSpace::Utils::TimeManager                                       m_tm;
    dsstring                                                            m_pluginDescr;

    DrawSpace::Interface::MesheImport*                                  m_meshe_import;

    DrawSpace::Core::BaseCallback<void, int>*                           m_closeapp_cb;

    DrawSpace::Interface::Module::Service*                              m_current_subservice;

    DrawSpace::Interface::Module::Root*                                 m_cdlodp_root;
    DrawSpace::Interface::Module::Service*                              m_cdlodp_service;

    DrawSpace::Interface::Module::Root*                                 m_sbmod_root;
    DrawSpace::Interface::Module::Service*                              m_sb_service;

    DrawSpace::Module::KeySource<dsstring>                              m_skybox_texturesbankpath;
    DrawSpace::Module::KeySource<dsstring>                              m_skybox_texturesbankvirtualfspath;
    DrawSpace::Module::KeySource<std::vector<dsstring>>                 m_skybox_texturesnames;

    void load_cdlodplanet_module( DrawSpace::Logger::Configuration* p_logconf );
    
    void load_skybox_module( DrawSpace::Logger::Configuration* p_logconf );


    MainLoopService( void );
public:

    ~MainLoopService( void );

    virtual void                            GetKeys( std::vector<DrawSpace::Module::KeySinkBase*>& p_keys );
    virtual void                            Init( DrawSpace::Logger::Configuration* p_logconf, 
                                                    DrawSpace::Core::BaseCallback<void, bool>* p_mousecircularmode_cb, 
                                                    DrawSpace::Core::BaseCallback<void, bool>* p_mousevisible_cb, 
                                                    DrawSpace::Core::BaseCallback<void, int>* p_closeapp_cb );
    virtual void                            Run( void );
    virtual void                            Release( void );

    virtual DrawSpace::Core::BaseSceneNode* InstanciateSceneNode( const dsstring& p_sceneNodeName, DrawSpace::Dynamics::Calendar* p_calendar, LODDependantNodeInfoStateHandler* p_handler );
    virtual void                            RegisterScenegraphCallbacks( DrawSpace::Core::SceneNodeGraph& p_scenegraph );
    virtual void                            UnregisterScenegraphCallbacks( DrawSpace::Core::SceneNodeGraph& p_scenegraph );
    virtual void                            ReleaseSceneNode( const dsstring& p_sceneNodeName, DrawSpace::Dynamics::Calendar* p_calendar );


    virtual void                            OnKeyPress( long p_key );
    virtual void                            OnEndKeyPress( long p_key );
    virtual void                            OnKeyPulse( long p_key );
    virtual void                            OnChar( long p_char, long p_scan );
    virtual void                            OnMouseMove( long p_xm, long p_ym, long p_dx, long p_dy );
    virtual void                            OnMouseWheel( long p_delta );
    virtual void                            OnMouseLeftButtonDown( long p_xm, long p_ym );
    virtual void                            OnMouseLeftButtonUp( long p_xm, long p_ym );
    virtual void                            OnMouseRightButtonDown( long p_xm, long p_ym );
    virtual void                            OnMouseRightButtonUp( long p_xm, long p_ym );
    virtual void                            OnAppEvent( WPARAM p_wParam, LPARAM p_lParam );

    void                                    OnGUIEvent( APP_GUI_EVENT p_evt );

    friend class BaseSingleton<MainLoopService>;
};

#endif
