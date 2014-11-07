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
#include <os2.h>
#include <stdio.h>
#include <stdlib.h>

#include "pm_messages.h"
#include "pm_error.h"

nl_catd PMMsg::msg_catd;
int     PMMsg::msg_country;
PMMsg   PMMsg::msg_sentry;

/*--------------------------------------------------
 * Destructs PMMsg object
 *--------------------------------------------------*/
PMMsg::~PMMsg()
{
  if( msg_catd )
    catclose( msg_catd );
}

/*--------------------------------------------------
 * Open message catalog
 *--------------------------------------------------*/
void PMMsg::open_catalog( const char* catalog_name )
{
  msg_catd    = catopen( catalog_name, 0 );
  msg_country = 1;

  if( msg_catd == CATD_ERR )
  {
    char msg_drive[_MAX_PATH] = "";
    char msg_path [_MAX_PATH] = "";
    char msg_name [_MAX_PATH] = "";

    PPIB ppib;
    DosGetInfoBlocks( NULL, &ppib );

    if( ppib->pib_pchcmd )
    {
      _splitpath( ppib->pib_pchcmd, msg_drive, msg_path, NULL, NULL );
      sprintf( msg_name, "%s%s%s", msg_drive, msg_path, catalog_name );
      msg_catd = catopen( msg_name, 0 );
    }
  }

  if( msg_catd == CATD_ERR )
  {
    char err_info[1024];
    sprintf( err_info, "Message catalog '%s' not found!", catalog_name );
    PM_THROW_ERROR( 0, err_info );
  }
  else
    select_country();
}

/*--------------------------------------------------
 * Select a country
 *--------------------------------------------------*/
void PMMsg::select_country( int country )
{
  if( !msg_catd )
    return;

  if( !country  )
  {
    char  country_code[16] = "";
    char* lang = getenv( "LANG" );
    ULONG len  = sizeof(country_code) - 1;

    PrfQueryProfileData( HINI_USERPROFILE,
                         "PM_National", "iCountry", &country_code, &len );

    country_code[len] = '\0';
    msg_country = atoi( country_code );

    if( lang && strlen(lang) >= 2 ) {
      if( strnicmp( "EN", lang, 2 ) == 0 ) {
        msg_country =  1;       
      }
      if( strnicmp( "RU", lang, 2 ) == 0 ) {
        msg_country =  7;       
      }
      if( strnicmp( "DE", lang, 2 ) == 0 ) {
        msg_country = 49;       
      }
    }
  }
  else
    msg_country = country;

  if( catgets( msg_catd, msg_country, 1, NULL ) == NULL )
    msg_country = 1;
}

/*--------------------------------------------------
 * Get message
 *--------------------------------------------------*/
char* PMMsg::get( int msg_no )
{
  return catgets( msg_catd, msg_country, msg_no, "Message not found" );
}

