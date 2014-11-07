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

/**@#-*/
#ifndef _PM_PROFILE_H
#define _PM_PROFILE_H
/**@#+*/

/**
 * User profile management class.
 * @author Dmitry A Steklenev
 */

class PMProfile {

  public:
    /** Puts data to user profile.  */
    static void put_data  ( const char* key, const void* exp, unsigned long len );
    /** Puts long value to user profile.  */
    static void put_long  ( const char *key, long exp );
    /** Puts unsigned long value to user profile.  */
    static void put_ulong ( const char *key, unsigned long exp  );
    /** Puts short value to user profile. */
    static void put_short ( const char *key, short exp );
    /** Puts unsigned short value to user profile. */
    static void put_ushort( const char *key, unsigned short exp );
    /** Puts characters string to user profile. */
    static void put_chars ( const char *key, const char* exp );

    /** Retrieves data from user profile. */
    static unsigned long get_data( const char* key, void* exp, unsigned long len );
    /** Retrieves long value from user profile. */
    static long get_long( const char *key, long def_value );
    /** Retrieves unsigned long value from user profile. */
    static unsigned long get_ulong( const char *key, unsigned long def_value );
    /** Retrieves short value from user profile. */
    static short get_short ( const char *key, short def_value );
    /** Retrieves unsigned short value from user profile. */
    static unsigned short get_ushort( const char *key, unsigned short def_value );
    /** Retrieves characters string from user profile. */
    static char* get_chars ( const char *key, char* exp, unsigned long len, char* def_value );
};

/**@#-*/
#endif
