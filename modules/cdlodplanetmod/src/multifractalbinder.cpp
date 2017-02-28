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

#include "multifractalbinder.h"

using namespace DrawSpace;
using namespace DrawSpace::Core;
using namespace DrawSpace::Utils;

MultiFractalBinder::MultiFractalBinder( void ) :
m_plains_amplitude( PLAINS_AMPLITUDE ),
m_mountains_amplitude( MOUNTAINS_AMPLITUDE ),
m_vertical_offset( VERTICAL_OFFSET ),
m_mountains_offset( MOUNTAINS_OFFSET ),
m_plains_seed1(234.4),
m_plains_seed2(9334.1),
m_mix_seed1(823.4),
m_mix_seed2(509.0)
{
}

void MultiFractalBinder::Bind( void )
{
	Vector landscape_control;
	Vector seeds;

	landscape_control[0] = m_plains_amplitude;
	landscape_control[1] = m_mountains_amplitude;
	landscape_control[2] = m_vertical_offset;
	landscape_control[3] = m_mountains_offset;

	seeds[0] = m_plains_seed1;
	seeds[1] = m_plains_seed2;
	seeds[2] = m_mix_seed1;
	seeds[3] = m_mix_seed2;

	m_renderer->SetFxShaderParams( 0, 30, landscape_control );
	m_renderer->SetFxShaderParams( 0, 31, seeds );
}

void MultiFractalBinder::Unbind( void )
{
}


