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
#include <stdlib.h>

#include "cmap_application.h"
#include "cmap_ids.h"
#include "pm_messages.h"
#include "pm_error.h"
#include "pm_gui.h"

const char* application_name = "Characters Map/2";

/*--------------------------------------------------
 * Start here
 *--------------------------------------------------*/
void main( /*int, char **argv*/ )
{
  PMGUI gui;

  try
  {
    PMMsg::open_catalog( "chmap.cat" );
    CMapApplication win_apps( ID_WIN_MAIN );

    win_apps.show()
            .set_focus();

    gui.run();
  }
  catch( PMError& error )
  {
    error.display();
    exit(1);
  }
}