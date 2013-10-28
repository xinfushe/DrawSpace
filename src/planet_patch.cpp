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

#include "planet_patch.h"

using namespace DrawSpace;
using namespace DrawSpace::Core;
using namespace DrawSpace::Planet;

Patch::Patch( int p_resolution ) : m_resolution( p_resolution )
{
    for( long i = 0; i < 8; i++ )
    {
        m_neighbours[i] = NULL;
    }

    build();
}

Patch::~Patch( void )
{
}

void Patch::SetNeighbour( Patch* p_patch, int p_id )
{
    m_neighbours[p_id] = p_patch;
}

Patch* Patch::GetNeighbour( int p_id )
{
    return m_neighbours[p_id];
}

void Patch::build( void )
{
}
