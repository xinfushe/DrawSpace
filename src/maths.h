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

#ifndef _MATHS_H_
#define _MATHS_H_

#include <math.h>
#include "vector.h"

#ifndef PI
#define PI 3.1415927
#endif

namespace DrawSpace
{
namespace Utils
{
class Maths
{
public:

	static double	Square( dsreal a )						    { return a * a; };
	static int		Floor( dsreal a )							{ return ((int)a - (a < 0 && a != (int)a)); };
	static int		Ceiling( dsreal a )						    { return ((int)a + (a > 0 && a != (int)a)); };

	static double	Min( dsreal a, dsreal b )					{ return (a < b ? a : b); };
	static double	Max( dsreal a, dsreal b )					{ return (a > b ? a : b); };
	static double	Abs( dsreal a )							    { return (a < 0 ? -a : a); };
	static double	Clamp( dsreal a, dsreal b, dsreal x )		{ return (x < a ? a : (x > b ? b : x)); };
	static double	Lerp( dsreal a, dsreal b, dsreal x )		{ return a + x * (b - a); };
	static double	Cubic( dsreal a )							{ return a * a * (3 - 2*a); };
	static double	Pulse( dsreal a, dsreal b, dsreal x )		    { return (double)((x >= a) - (x >= b)); };
	static double	Gamma( dsreal a, dsreal g )				    { return pow(a, 1/g); };
	static double	Expose( dsreal l, dsreal k )				    { return (1 - exp(-l * k)); };
	static double	DegToRad( dsreal ang )					    { return ( ( ang * PI ) / 180.0 ); };
    static double	RadToDeg( dsreal ang )					    { return ( ( ang * 180.0 ) / PI ); };


	static void		SphericaltoCartesian( const Vector& p_in, Vector& p_out )
	{
		p_out[2] = ( p_in[0] * cos( p_in[2] ) * cos( p_in[1] ) );
		p_out[0] = ( p_in[0] * cos( p_in[2] ) * sin( p_in[1] ) );
		p_out[1] = ( p_in[0] * sin( p_in[2] ) );
	}
	
	static void		CartesiantoSpherical( Vector& p_in, Vector& p_out )
	{
		// cas particulier
		if( p_in[1] > 0.0 && 0.0 == p_in[0] && 0.0 == p_in[2] )
		{
			p_out[0] = p_in[1];
			p_out[2] = PI / 2.0;
			p_out[1] = 0.0;
			return;
		}
		else if( p_in[1] < 0.0 && 0.0 == p_in[0] && 0.0 == p_in[2] )
		{
			p_out[0] = -p_in[1];
			p_out[2] = -PI / 2.0;
			p_out[1] = 0.0;
			return;
		}
		
        p_out[0] = p_in.Length();
		p_out[2] = asin( p_in[1] / p_out[0] );		
		p_out[1] = atan2( p_in[0], p_in[2] );
	}

    static void CubeToSphere( Vector& p_in, Vector& p_out )
    {
	    float xtemp = p_in[0];
	    float ytemp = p_in[1];
	    float ztemp = p_in[2];

	    p_out[0] = xtemp * sqrt( 1.0 - ytemp * ytemp * 0.5 - ztemp * ztemp * 0.5 + ytemp * ytemp * ztemp * ztemp / 3.0 );
	    p_out[1] = ytemp * sqrt( 1.0 - ztemp * ztemp * 0.5 - xtemp * xtemp * 0.5 + xtemp * xtemp * ztemp * ztemp / 3.0 );
	    p_out[2] = ztemp * sqrt( 1.0 - xtemp * xtemp * 0.5 - ytemp * ytemp * 0.5 + xtemp * xtemp * ytemp * ytemp / 3.0 );
    }

    static void VectorPlanetOrientation( int p_orientation, Vector& p_in, Vector& p_out )
    {
        Vector res;

	    if( 0 == p_orientation ) // front
	    {
		    p_out[0] = p_in[0];
		    p_out[1] = p_in[1];
		    p_out[2] = p_in[2];   
	    }
	    else if( 1 == p_orientation ) // rear
	    {
		    p_out[0] = -p_in[0];
		    p_out[1] = p_in[1];
		    p_out[2] = -p_in[2];   
	    }
	    else if( 2 == p_orientation ) // left
	    {
		    p_out[0] = -p_in[2];
		    p_out[1] = p_in[1];
		    p_out[2] = p_in[0];   
	    }
	    else if( 3 == p_orientation ) // right
	    {
		    p_out[0] = p_in[2];
		    p_out[1] = p_in[1];
		    p_out[2] = -p_in[0];   
	    }
	    else if( 4 == p_orientation ) // top
	    {
		    p_out[0] = p_in[0];
		    p_out[1] = p_in[2];
		    p_out[2] = -p_in[1];   
	    }
	    else //if( 5 == p_orientation ) // bottom
	    {
		    p_out[0] = p_in[0];
		    p_out[1] = -p_in[2];
		    p_out[2] = p_in[1];
	    }
    }
};
}
}

#endif
