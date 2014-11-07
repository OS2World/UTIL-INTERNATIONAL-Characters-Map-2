/* Characters Map/2
 * Copyright (C) 2001-2013 Dmitry A.Steklenev
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
#include <stdio.h>

#include "cmap_about.h"
#include "cmap_ids.h"

extern const char* application_name;

/*--------------------------------------------------
 * Constructs the about window
 *--------------------------------------------------*/
CMapAbout::CMapAbout( SHORT id, PMWindow* owner )

: inherited  ( id, "", owner->handle(), 0, 0, 360, 187,
               FCF_NOBYTEALIGN | FCF_NOMOVEWITHOWNER | FCF_TITLEBAR |
               FCF_SYSMENU     | FCF_DLGBORDER       | FCF_AUTOICON |
               FCF_CLOSEBUTTON ),

  lbl_about  ( WC_STATIC, ID_STA_APPNAME, "Characters Map/2", handle(), handle(),
               19, 135, 310,  20, WS_VISIBLE | SS_TEXT | DT_LEFT | DT_BOTTOM ),
  lbl_version( WC_STATIC, ID_STA_VERSION, ""    , handle(), handle(),
               20, 115, 310,  18, WS_VISIBLE | SS_TEXT | DT_LEFT | DT_TOP    ),
  lbl_sep    ( WC_STATIC, ID_STA_SEP,     "", handle(), handle(),
               20, 110, 310,   1, SS_FGNDRECT | WS_VISIBLE ),
  lbl_author ( WC_STATIC, ID_STA_AUTHOR,  ""   , handle(), handle(),
               20,  40, 310,  65, WS_VISIBLE | SS_TEXT | DT_LEFT | DT_WORDBREAK ),
  btn_ok     ( WC_BUTTON, DID_OK,         "~OK", handle(), handle(),
               240,  10, 110,  30, WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON )
{
  char ver[64];
  sprintf( ver, "Version %s", PCK_VERSION );

  btn_ok     .set_font   ( "9.WarpSans" );
  lbl_version.set_font   ( "9.WarpSans" )
             .set_caption( ver );
  lbl_author .set_font   ( "9.WarpSans" )
             .set_caption( "Copyright (C) 1998-2013 Dmitry A.Steklenev\n"
                           "http://glass.ptv.ru/charmap.html\n"
                           "dmitry@5nets.ru\n" );

  // Restore the last window position and size
  SHAPE old_rect = shape();

  load( INI_ABOUT_POS );
  center_at();

  SHAPE new_rect = shape();
  new_rect.cx  = old_rect.cx;
  new_rect.cy  = old_rect.cy;
  reshape( new_rect );

  show();
  btn_ok.set_focus();

  WinProcessDlg( handle());
}

/*--------------------------------------------------
 * Dispatch system native event
 *-------------------------------------------------*/
MRESULT CMapAbout::dispatch( ULONG msg, MPARAM mp1, MPARAM mp2 )
{
  switch( msg )
  {
    case WM_CLOSE:
      WinDismissDlg( handle(), 0 );
      destroy();
      return 0;

    case WM_DESTROY:
      save( INI_ABOUT_POS  );
      stop_handling_events();
      delete this;
      return 0;

    case WM_COMMAND:
    {
      switch ( SHORT1FROMMP(mp1))
      {
        case DID_OK:
        case DID_CANCEL:
          WinPostMsg( handle(), WM_CLOSE, 0, 0 );
          return 0;
      }
    }
  }

  return inherited::dispatch( msg, mp1, mp2 );
}

