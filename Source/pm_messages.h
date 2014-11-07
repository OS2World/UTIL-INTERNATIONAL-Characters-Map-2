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
#ifndef _PM_MESSAGES_H
#define _PM_MESSAGES_H

#include <nl_types.h>
#define   PM_MAX_MSG_SIZE 2048
/**@#+*/

/**
 * Message catalog management class.
 * @author Dmitry A Steklenev
 */

class PMMsg {

  public:
     /** Opens message catalog. */
     static void  open_catalog( const char* catalog_file );
     /** Returns the message. */
     static char* get( int msg_no );
     /** Selects a country. */
     static void  select_country( int country = 0 );

  private:
     ~PMMsg();

     static nl_catd msg_catd;
     static int     msg_country;
     static PMMsg   msg_sentry;
};

/**@#-*/
#endif
