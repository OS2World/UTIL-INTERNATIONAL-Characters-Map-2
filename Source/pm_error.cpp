/* Characters Map/2
 * Copyright (C) 2001-2005 Dmitry A.Steklenev
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#define  INCL_WIN
#define  INCL_DOS
#define  INCL_ERRORS
#include <os2.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "pm_gui.h"
#include "pm_error.h"

extern const char* application_name;

/*--------------------------------------------------
 * Constructs error from current GUI error
 *--------------------------------------------------*/
PMError::PMError( const char* file, const char* func, int line )

: err_file( strdup( file )),
  err_func( strdup( func )),
  err_line( line ),
  err_code( 0    ),
  err_info( NULL )
{
  PERRINFO os2_info = WinGetErrorInfo( PMGUI::hab());

  if( os2_info )
  {
    err_code = os2_info->idError;
    err_info = (char*)os2_info +
                     ((char*)os2_info + os2_info->offaoffszMsg)[0];
  }

  err_info = strdup( err_info );

  if( os2_info )
    WinFreeErrorInfo(os2_info);
}

/*--------------------------------------------------
 * Constructs error from specified error id and information
 *--------------------------------------------------*/
PMError::PMError( int code, const char* info,
                            const char* file, const char* func, int line )
: err_file( strdup( file )),
  err_func( strdup( func )),
  err_line( line ),
  err_code( code )
{
  if( !info )
  {
    char   szErrInfo[1024];
    ULONG  ulMessageLength;
    APIRET rc;

    rc = DosGetMessage( 0, 0, (PCHAR)szErrInfo, sizeof(szErrInfo),
                        code, (PSZ)"OSO001.MSG", &ulMessageLength );

    info = rc ? "No error text is available" : szErrInfo;
  }

  err_info = strdup( info );
}

/*--------------------------------------------------
 * Copy constructor
 *--------------------------------------------------*/
PMError::PMError( const PMError& error )

: err_file( strdup( error.err_file )),
  err_func( strdup( error.err_func )),
  err_info( strdup( error.err_info )),
  err_line( error.err_line ),
  err_code( error.err_code )

{}

/*--------------------------------------------------
 * Assignment operator
 *--------------------------------------------------*/
PMError& PMError::operator=( const PMError& error )
{
  err_line = error.err_line;
  err_code = error.err_code;
  err_file = strdup( error.err_file );
  err_func = strdup( error.err_func );
  err_info = strdup( error.err_info );

  return *this;
}

/*--------------------------------------------------
 * Destructor
 *--------------------------------------------------*/
PMError::~PMError()
{
  free( err_info );
  free( err_func );
  free( err_file );
}

/*--------------------------------------------------
 * Display error information
 *--------------------------------------------------*/
PMError& PMError::display( FILE* out )
{
  const char* msg_fmt = "Unexpected error at:\n"
                        "  file: %s\n"
                        "  function: %s\n"
                        "  line: %u\n\n"
                        "Error Id is: %u (%x)\n"
                        "Exception text is:\n\n"
                        "%s\n";

  char* msg_buf = new char[ strlen(msg_fmt) +
                            strlen(file() ) +
                            strlen(func() ) +
                            strlen(info() ) + 64 ];

  char* msg_ttl = new char[71];

  sprintf( msg_buf, msg_fmt, file(), func(), line(), code(), code(), info());
  sprintf( msg_ttl, "%-070s", application_name );

  if( out )
  {
    fprintf( out, msg_buf );
    fflush ( out );
  }
  else
    WinMessageBox( HWND_DESKTOP, HWND_DESKTOP, msg_buf, msg_ttl, 100,
                   MB_OK | MB_ERROR | MB_APPLMODAL | MB_MOVEABLE );

  delete[] msg_buf;
  delete[] msg_ttl;
  return *this;
}

