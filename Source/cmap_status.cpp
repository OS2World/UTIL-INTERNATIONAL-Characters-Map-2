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
#include <stdio.h>

#include "cmap_status.h"

/*--------------------------------------------------
 * Constructs the status window
 *--------------------------------------------------*/
CMapStatus::CMapStatus( SHORT id, HWND parent, HWND owner,
                        long x, long y, long cx, long cy )

: inherited( WC_STATIC, id, "", parent, owner,
             x, y, cx, cy, WS_VISIBLE | SS_TEXT | DT_LEFT | DT_VCENTER )
{}

/*--------------------------------------------------
 * Sets the window caption
 *-------------------------------------------------*/
CMapStatus& CMapStatus::set_caption( const char* caption )
{
  char* padded = new char[strlen(caption)+3];
  sprintf( padded, "  %s", caption );
  inherited::set_caption( padded );
  delete padded;

  return *this;
}

/*--------------------------------------------------
 * Dispatch system native event
 *-------------------------------------------------*/
MRESULT CMapStatus::dispatch( ULONG msg, MPARAM mp1, MPARAM mp2 )
{
  switch( msg )
  {
    case WM_PAINT:
    {
      inherited::dispatch( msg, mp1, mp2 );

      HPS   ps   = WinGetPS( handle());
      SHAPE rect = shape();

      rect.x = 0;
      rect.y = 0;
      draw_3dborder( ps, rect, 1 );

      WinReleasePS(ps);
      return 0;
    }
  }

  return inherited::dispatch( msg, mp1, mp2 );
}

