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

#include "pm_frame.h"

extern const char* application_name;
static FRAMECDATA  fcdata = { sizeof( FRAMECDATA ), 0, 0, 0 };

/*--------------------------------------------------
 * Constructs the frame window
 *--------------------------------------------------*/
PMFrame::PMFrame( SHORT       id,
                  const char* caption,
                  HWND        owner,
                  long        x,
                  long        y,
                  long        cx,
                  long        cy,
                  ULONG       style )

: inherited ( WC_FRAME, id, caption, HWND_DESKTOP, owner,
              x, y, cx, cy, WS_SAVEBITS, ( fcdata.flCreateFlags = style, &fcdata )),
  frm_client( 0 )
{
  // Change window background
  WinSetWindowUShort( handle(), QWS_FLAGS,
    WinQueryWindowUShort( handle(), QWS_FLAGS ) | FF_DIALOGBOX );

  // Assign icon
  HPOINTER hIcon = WinLoadPointer( HWND_DESKTOP, NULLHANDLE, id );

  if( hIcon == NULLHANDLE  ||
     !WinSendMsg( handle(), WM_SETICON, MPFROMLONG(hIcon), 0 ))
  {
    PM_THROW_GUIERROR();
  }

  if( !WinSetWindowPos( handle(), HWND_TOP, 0, 0, 0, 0, SWP_ZORDER ))
    PM_THROW_GUIERROR();

  // Needed for correct repainting of a frame controls
  WinSendMsg( handle(), WM_UPDATEFRAME,
                        MPFROMLONG(fcdata.flCreateFlags), 0 );
}

/*--------------------------------------------------
 * Calculate client rectangle
 *--------------------------------------------------*/
SHAPE PMFrame::client_shape() const
{
   SHAPE shape = this->shape();
   RECTL rect  = { 0, 0, shape.cx, shape.cy };

   WinCalcFrameRect( handle(), &rect, TRUE );

   shape.x  = rect.xLeft;
   shape.y  = rect.yBottom;
   shape.cx = rect.xRight - rect.xLeft;
   shape.cy = rect.yTop   - rect.yBottom;

   return shape;
}

/*--------------------------------------------------
 * Save the current position and size of the window
 *--------------------------------------------------*/
PMFrame& PMFrame::save( const char* key )
{
  WinStoreWindowPos( application_name, key, handle());
  return *this;
}

/*--------------------------------------------------
 * Restore the current position and size of the window
 *--------------------------------------------------*/
BOOL PMFrame::load( const char* key )
{
  return WinRestoreWindowPos( application_name, key, handle());
}

/*--------------------------------------------------
 * Sets the client window
 *--------------------------------------------------*/
PMFrame& PMFrame::set_client( HWND client )
{
  frm_client = client;

  if( frm_client )
    PMWindow( frm_client ).reshape( client_shape());

  return *this;
}

/*--------------------------------------------------
 * Dispatch system native events
 *--------------------------------------------------*/
MRESULT PMFrame::dispatch( ULONG msg, MPARAM mp1, MPARAM mp2 )
{
  switch( msg )
  {
    case WM_ERASEBACKGROUND:
      if( frm_client )
        return 0;

      break;

    case WM_FORMATFRAME:
      if( frm_client )
      {
        MRESULT rc = dispatch_everything( msg, mp1, mp2 );
        PMWindow( frm_client ).reshape( client_shape());
        return  rc;
      }
      break;
  }
  return inherited::dispatch( msg, mp1, mp2 );
}

