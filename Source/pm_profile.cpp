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
#include <os2.h>
#include <string.h>

#include "pm_profile.h"

extern const char* application_name;

/*--------------------------------------------------
 * Puts data to user profile
 *--------------------------------------------------*/
void PMProfile::put_data( const char* key, const void* exp, unsigned long len )
{
  PrfWriteProfileData( HINI_USERPROFILE,
                       application_name,
                       key, (PVOID)exp, len );
}

/*--------------------------------------------------
 * Retrieves data from user profile
 *--------------------------------------------------*/
unsigned long PMProfile::get_data( const char* key, void* exp, unsigned long len )
{
  unsigned long get_len = len;

  PrfQueryProfileData( HINI_USERPROFILE,
                       application_name,
                       key, exp, &get_len );
  return get_len;
}

/*--------------------------------------------------
 * Puts long value to user profile
 *--------------------------------------------------*/
void PMProfile::put_long( const char *key, long exp )
{
  PrfWriteProfileData( HINI_USERPROFILE,
                       application_name,
                       key, &exp, sizeof(exp));
}

/*--------------------------------------------------
 * Retrieves long value from user profile
 *--------------------------------------------------*/
long PMProfile::get_long( const char *key, long def )
{
  long exp = def;
  unsigned long len = sizeof(exp);

  return PrfQueryProfileData( HINI_USERPROFILE,
                              application_name,
                              key, &exp, &len ) ? exp : def;
}

/*--------------------------------------------------
 * Puts unsigned long value to user profile
 *--------------------------------------------------*/
void PMProfile::put_ulong( const char *key, unsigned long exp )
{
  PrfWriteProfileData( HINI_USERPROFILE,
                       application_name,
                       key, &exp, sizeof(exp));
}

/*--------------------------------------------------
 * Retrieves unsigned long value from user profile
 *--------------------------------------------------*/
unsigned long PMProfile::get_ulong( const char *key, unsigned long def )
{
  unsigned long exp = def;
  unsigned long len = sizeof(exp);

  return PrfQueryProfileData( HINI_USERPROFILE,
                              application_name,
                              key, &exp, &len ) ? exp : def;
}

/*--------------------------------------------------
 * Puts short value to user profile
 *--------------------------------------------------*/
void PMProfile::put_short( const char *key, short exp )
{
  PrfWriteProfileData( HINI_USERPROFILE,
                       application_name,
                       key, &exp, sizeof(exp));
}

/*--------------------------------------------------
 * Retrieves short value from user profile
 *--------------------------------------------------*/
short PMProfile::get_short( const char *key, short def )
{
  short exp = def;
  unsigned long len = sizeof(exp);

  return PrfQueryProfileData( HINI_USERPROFILE,
                              application_name,
                              key, &exp, &len ) ? exp : def;
}

/*--------------------------------------------------
 * Puts unsigned short value to user profile
 *--------------------------------------------------*/
void PMProfile::put_ushort( const char *key, unsigned short exp )
{
  PrfWriteProfileData( HINI_USERPROFILE,
                       application_name,
                       key, &exp, sizeof(exp));
}

/*--------------------------------------------------
 * Retrieves unsigned short value from user profile
 *--------------------------------------------------*/
unsigned short PMProfile::get_ushort( const char *key, unsigned short def )
{
  unsigned short exp = def;
  unsigned long  len = sizeof(exp);

  return PrfQueryProfileData( HINI_USERPROFILE,
                              application_name,
                              key, &exp, &len ) ? exp : def;
}

/*--------------------------------------------------
 * Puts characters string to user profile
 *--------------------------------------------------*/
void PMProfile::put_chars( const char *key, const char* exp )
{
  PrfWriteProfileData( HINI_USERPROFILE,
                       application_name,
                       key, (PVOID)exp, strlen(exp)+1 );
}

/*--------------------------------------------------
 * Retrieves characters string from user profile
 *--------------------------------------------------*/
char* PMProfile::get_chars( const char *key, char* exp, unsigned long len, char* def )
{
  unsigned long get_len = len;

  if( !PrfQueryProfileData( HINI_USERPROFILE,
                            application_name, key, exp, &get_len ))
  {
    strcpy( exp, def );
  }

  return exp;
}
