/* -*-LIC_BEGIN-*- */
/*
*                                                                          
* DrawSpace Rendering engine                                               
* Emmanuel Chaumont Copyright (c) 2013-2018                        
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

float4 reflectedVertexPos(float4 p_vertex, float4 p_reflectorPos, float4 p_reflectorNormale, float4x4 p_matWorld, float4x4 p_matView, float4x4 p_matProj)
{
    float4 rvp;

    float4x4 id = 0;
    id[0][0] = 1.0;
    id[1][1] = 1.0;
    id[2][2] = -1.0;
    id[3][3] = 1.0;
    float4x4 matView2 = mul(p_matView, id);
    
    float4 worldPos = mul(p_vertex, p_matWorld);
    float4 worldPos2 = worldPos - p_reflectorPos;
    worldPos2.w = 1.0;
    
    float4 worldPosRefl;
    worldPosRefl.xyz = reflect(worldPos2.xyz, normalize(p_reflectorNormale.xyz));
    worldPosRefl.w = worldPos.w;

    float4 worldPos3 = worldPosRefl + p_reflectorPos;
    worldPos3.w = 1.0;

    float4 viewPos = mul(worldPos3, matView2);

    rvp = mul(viewPos, p_matProj);
    return rvp;
}

//pour lumieres diffuses : NORMAL transformee (sans les translations)
//dans repereworld
float4 TransformedNormaleForLights(float3 p_normale, float4x4 p_worldmat)
{
    float4x4 worldRot = p_worldmat;
    worldRot[0][3] = 0.0;
    worldRot[1][3] = 0.0;
    worldRot[2][3] = 0.0;
    return mul(p_normale, worldRot);
}

// compute half-vector for specular lights
float4 HalfVectorForLights(float4 p_pos, float4 p_lightdir, float4x4 p_cammat, float4x4 p_worldmat)
{
    float4 CamPos;
    float4 Pos2;
    float3 nView;
    float3 nLight;

    CamPos[0] = p_cammat[3][0];
    CamPos[1] = p_cammat[3][1];
    CamPos[2] = p_cammat[3][2];
    CamPos[3] = 1.0;

    Pos2 = mul(p_pos, p_worldmat);
    nView = normalize(CamPos.xyz - Pos2.xyz);

    nLight = normalize(-p_lightdir.xyz);

    float4 H;
    H.xyz = nLight + nView;
    H.w = 0.0;
    return H;
}