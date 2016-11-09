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

#include <Physfs.h>

#include "file.h"
#include "memalloc.h"
#include "exceptions.h"


using namespace DrawSpace::Utils;

File::FSMode File::m_fsMode = File::LOCALFILESYSTEM;
dsstring File::m_virtualFsArchiveName;

File::File( const dsstring& p_filename, Mode p_mode ) : m_fp( NULL )
{
    if( CREATENEW == p_mode )
    {
        m_fp = fopen( p_filename.c_str(), "wb" );
    }
    else if( OPENEXISTINGB == p_mode )
    {
        m_fp = fopen( p_filename.c_str(), "rb" );
    }	
    else if( CREATENEWTEXT == p_mode )
    {
        m_fp = fopen( p_filename.c_str(), "wt" );
    }
    else if( OPENEXISTINGTEXT == p_mode )
    {
        m_fp = fopen( p_filename.c_str(), "r" );
    }

    if( !m_fp )
    {
        _DSEXCEPTION( dsstring( "Failed to open file " ) + p_filename );
    }
}

File::~File( void )
{
    if( m_fp )
    {
        fclose( m_fp );
    }
}

void File::SaveArchive( Archive& p_arc )
{
    if( m_fp )
    {
        fwrite( p_arc.m_data, p_arc.m_total_length, 1, m_fp );	
    }
}

bool File::LoadArchive( Archive& p_arc )
{
    if( m_fp )
    {
        unsigned long fsize = FileSize( m_fp ); //obtenir taille du fichier

        p_arc.SetArchiveTotalLength( fsize );
        fread( p_arc.GetCurrentPtr(), fsize, 1, m_fp );
        return true;
    }
    else
    {
        return false;
    }
}

void File::Puts( const dsstring& p_string )
{
    if( m_fp )
    {
        fputs( p_string.c_str(), m_fp );
    }
}

bool File::Gets( char* p_buff, int p_nbToRead )
{
    if( m_fp )
    {
        char* s = fgets( p_buff, p_nbToRead, m_fp );
        if( s )
        {
            return true;
        }
    }
    return false;
}

void File::Flush( void )
{
    if( m_fp )
    {
        fflush( m_fp );
    }
}

long File::FileSize( void )
{
    if( m_fp )
    {
        return FileSize( m_fp );
    }
    return 0;
}

long File::FileSize( FILE *p_fp )
{
    long current_pos;
    current_pos = ftell( p_fp );
    fseek( p_fp, 0, SEEK_END );
    long size = ftell( p_fp );
    fseek( p_fp, current_pos, SEEK_SET );
    return size;
}

void* File::LoadAndAllocBinaryFile( const dsstring& p_file, long* p_size )
{
    if( LOCALFILESYSTEM == m_fsMode )
    {
        void* ptr = NULL;
        FILE* fp;
        fp = fopen( p_file.c_str(), "rb" );
        if( fp )
        {
            unsigned long fs = FileSize( fp );
            ptr = (void*)_DRAWSPACE_NEW_EXPLICIT_SIZE_( unsigned char, unsigned char[fs], fs );
            if( ptr )
            {
                fread( (void *)ptr, fs, 1, fp );	
                if( p_size )
                {
                    *p_size = fs;
                }
            }
            fclose( fp );
        }
        return ptr;
    }
    else // VIRTUALFILESYSTEM
    {
        void* ptr = NULL;

        PHYSFS_file* vfile = PHYSFS_openRead( p_file.c_str() );
        if( NULL == vfile )
        {
            _DSEXCEPTION( dsstring( "cannot open file from virtual filesystem : " ) + p_file );
        }
        PHYSFS_sint64 file_size = PHYSFS_fileLength( vfile );

        unsigned char *vFileBuf;
        vFileBuf = _DRAWSPACE_NEW_EXPLICIT_SIZE_( unsigned char, unsigned char[file_size], file_size );
        int length_read = PHYSFS_read( vfile, vFileBuf, 1, file_size );

        if( length_read != file_size )
        {
            _DSEXCEPTION( dsstring( "unexpected error while reading file from virtual filesystem : " ) + p_file );
        }

        ptr = vFileBuf;
        PHYSFS_close( vfile );

        *p_size = length_read;

        return ptr;
    }
}

void File::MountVirtualFS( const dsstring& p_virtualFsArchiveName )
{
    if( m_fsMode != VIRTUALFILESYSTEM )
    {
        PHYSFS_init( NULL );

        int status = PHYSFS_mount( p_virtualFsArchiveName.c_str(), NULL, 1 );
        if( !status )
        {
            _DSEXCEPTION( dsstring( "Cannot mount virtual filesystem archive : " ) + p_virtualFsArchiveName );
        }

        m_fsMode = VIRTUALFILESYSTEM;
    }
}

void File::UnmountVirtualFS( void )
{
    if( VIRTUALFILESYSTEM == m_fsMode )
    {
        PHYSFS_deinit();
        m_fsMode = LOCALFILESYSTEM;
    }
}
