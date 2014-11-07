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
#define  INCL_GPI
#include <os2.h>

#include "pm_window.h"
#include "pm_gui.h"

#include <string.h>

/*--------------------------------------------------
 * Constructs the object from window handle
 *--------------------------------------------------*/
PMWindow::PMWindow( HWND window_handle )

: win_handle ( window_handle ),
  win_wrapper( TRUE          ),

  win_default_handler(0)
{
  if( win_handle )
    win_id = WinQueryWindowUShort( win_handle, QWS_ID );
}

/*--------------------------------------------------
 *  Constructs the presentation window
 *--------------------------------------------------*/
PMWindow::PMWindow( const char* class_name ,
                    SHORT       id         ,
                    const char* caption    ,
                    HWND        parent     ,
                    HWND        owner      ,
                    long        x          ,
                    long        y          ,
                    long        cx         ,
                    long        cy         ,
                    ULONG       style      ,
                    PVOID       class_data )

: win_id     ( id    ),
  win_wrapper( FALSE ),

  win_default_handler(0)
{
  win_handle = WinCreateWindow( parent, class_name, caption, style,
                                x, y, cx, cy, owner, HWND_BOTTOM, id, class_data, 0 );

  if( win_handle == NULLHANDLE )
    PM_THROW_GUIERROR();

  handle_events();
}

/*--------------------------------------------------
 *  Constructs the presentation window
 *--------------------------------------------------*/
PMWindow::PMWindow( SHORT       id         ,
                    const char* caption    ,
                    HWND        parent     ,
                    HWND        owner      ,
                    long        x          ,
                    long        y          ,
                    long        cx         ,
                    long        cy         ,
                    ULONG       style      )

: win_id     ( id    ),
  win_wrapper( FALSE ),

  win_default_handler(0)
{
  static BOOL initialized = FALSE;

  if( !initialized )
  {
    if( !WinRegisterClass( PMGUI::hab(), "PMWindow", WinDefWindowProc, CS_SIZEREDRAW, 4 ))
      PM_THROW_GUIERROR();

    initialized = TRUE;
  }

  win_handle = WinCreateWindow( parent, "PMWindow", caption, style,
                                x, y, cx, cy, owner, HWND_BOTTOM, id, NULL, 0 );
  if( win_handle == NULLHANDLE )
    PM_THROW_GUIERROR();

  handle_events();
}

/*--------------------------------------------------
 * Destructs the window
 *--------------------------------------------------*/
PMWindow::~PMWindow()
{
  if( !win_wrapper )
    destroy();
}

/*--------------------------------------------------
 * Destroys the window
 *--------------------------------------------------*/
PMWindow& PMWindow::destroy()
{
  // Prevent recursive destroying
  win_wrapper = TRUE;

  if( WinIsWindow( PMGUI::hab(), handle()))
    if( !WinDestroyWindow( handle()))
      PM_THROW_GUIERROR();

  return *this;
}

/*--------------------------------------------------
 * Makes the window visible
 *--------------------------------------------------*/
PMWindow& PMWindow::show( BOOL state )
{
  if( !WinShowWindow( win_handle, state ? SWP_SHOW : SWP_HIDE ))
    PM_THROW_GUIERROR();

  return *this;
}

/*--------------------------------------------------
 * Sets the input focus to the window
 *-------------------------------------------------*/
PMWindow& PMWindow::set_focus()
{
  if( !WinSetFocus( HWND_DESKTOP, handle()))
    PM_THROW_GUIERROR();

  return *this;
}

/*--------------------------------------------------
 * Enables the window to accept keyboard and mouse
 * input
 *-------------------------------------------------*/
PMWindow& PMWindow::enable( BOOL state )
{
  if( !WinEnableWindow( handle(), state ))
    PM_THROW_GUIERROR();

  return *this;
}

/*--------------------------------------------------
 * Prevents keyboard and mouse input from being sent
 * to the window
 *-------------------------------------------------*/
PMWindow& PMWindow::disable()
{
  return enable(FALSE);
}

/*--------------------------------------------------
 * Get the window shape
 *--------------------------------------------------*/
SHAPE PMWindow::shape() const
{
  SWP   swp_rect;
  SHAPE shp_rect;

  if( !WinQueryWindowPos( win_handle, &swp_rect ))
    PM_THROW_GUIERROR();

  shp_rect.x  = swp_rect.x;
  shp_rect.y  = swp_rect.y;
  shp_rect.cx = swp_rect.cx;
  shp_rect.cy = swp_rect.cy;

  return shp_rect;
}

/*--------------------------------------------------
 * Sets a new font to be used by the window
 *--------------------------------------------------*/
PMWindow& PMWindow::set_caption( const char* caption )
{
  WinSetWindowText( handle(), caption );
  return *this;
}

/*--------------------------------------------------
 * Sets a new font to be used by the window
 *--------------------------------------------------*/
PMWindow& PMWindow::set_font( const char* font )
{
  static int   have_warpsans = -1;
  const  char* used_font = font;

  if( strcmp( font, "9.WarpSans" ) == 0 )
  {
    if( have_warpsans == -1 )
    {
      LONG rc;
      LONG fonts = 0;
      HPS  hps;

      hps = WinGetPS( HWND_DESKTOP );
      rc  = GpiQueryFonts( hps, QF_PUBLIC, "WarpSans", &fonts, 0, NULL );
      WinReleasePS( hps );

      have_warpsans = ( rc > 0 && rc != GPI_ALTERROR );
    }

    if( !have_warpsans ) {
      used_font = "8.Helv";
    }
  }

  if( !WinSetPresParam( win_handle, PP_FONTNAMESIZE, strlen(used_font)+1, (PVOID)used_font ))
    PM_THROW_GUIERROR();

  return *this;
}

/*--------------------------------------------------
 * Set the window shape
 *--------------------------------------------------*/
PMWindow& PMWindow::reshape( const SHAPE& rect )
{
  if( !WinSetWindowPos( handle(), 0, rect.x, rect.y, rect.cx, rect.cy, SWP_SIZE | SWP_MOVE ))
    PM_THROW_GUIERROR();

  return *this;
}

/*--------------------------------------------------
 * Center the window concerning his owner window
 *--------------------------------------------------*/
PMWindow& PMWindow::center_at()
{
  HWND  o_handle = owner   ();
  HWND  p_handle = parent  ();
  SHAPE o_shape  = PMWindow( o_handle ? o_handle : p_handle ).shape();

  SHAPE shape = this->shape();

  if( o_handle )
  {
    shape.x = (o_shape.cx - shape.cx)/2 + o_shape.x;
    shape.y = (o_shape.cy - shape.cy)/2 + o_shape.y;
  }
  else
  {
    shape.x = (o_shape.cx - shape.cx)/2;
    shape.y = (o_shape.cy - shape.cy)/2;
  }

  return reshape( shape );
}

/*--------------------------------------------------
 * Adds a region to a window's update region
 *-------------------------------------------------*/
PMWindow& PMWindow::invalidate( const SHAPE& rect )
{
  // Invalidation must use inclusive-exclusive rectangles
  RECTL os2_rect = { rect.x, rect.y,
                     rect.x+rect.cx, rect.y+rect.cy };

  if( !WinInvalidateRect( handle(), &os2_rect, TRUE ))
    PM_THROW_GUIERROR();

  return *this;
}

/*--------------------------------------------------
 * Adds the whole window to a window's update region
 *-------------------------------------------------*/
PMWindow& PMWindow::invalidate()
{
  if( !WinInvalidateRect( handle(), NULL, TRUE ))
    PM_THROW_GUIERROR();

  return *this;
}

/*--------------------------------------------------
 * Capture mouse pointer
 *-------------------------------------------------*/
PMWindow& PMWindow::capture_pointer( BOOL capture )
{
  if( !WinSetCapture( HWND_DESKTOP, capture ? handle() : NULLHANDLE ))
    PM_THROW_GUIERROR();

  return *this;
}

/*--------------------------------------------------
 * Release mouse pointer
 *-------------------------------------------------*/
PMWindow& PMWindow::release_pointer()
{
  return capture_pointer(FALSE);
}

/*--------------------------------------------------
 * Replaces the accelerator keys used by the window
 *-------------------------------------------------*/
PMWindow& PMWindow::attach_accelerators( SHORT id )
{
  HACCEL h_accel = WinLoadAccelTable( PMGUI::hab(), 0, id );

  if( h_accel == NULLHANDLE )
    PM_THROW_GUIERROR();

  if( !WinSetAccelTable( PMGUI::hab(), h_accel, handle()))
    PM_THROW_GUIERROR();

  return *this;
}

/*--------------------------------------------------
 * Has pointer captured
 *-------------------------------------------------*/
BOOL PMWindow::has_pointer_captured() const
{
  return (WinQueryCapture( HWND_DESKTOP ) == handle());
}

/*--------------------------------------------------
 * Returns the parent window handle
 *-------------------------------------------------*/
HWND PMWindow::parent() const
{
  return WinQueryWindow( handle(), QW_PARENT );
}

/*--------------------------------------------------
 * Returns the owner window handle
 *-------------------------------------------------*/
HWND PMWindow::owner() const
{
  return WinQueryWindow( handle(), QW_OWNER );
}

/*--------------------------------------------------
 * Has focus
 *-------------------------------------------------*/
BOOL PMWindow::has_focus() const
{
  return WinQueryFocus( HWND_DESKTOP ) == handle();
}

/*--------------------------------------------------
 * Returns the background color value of the window
 *-------------------------------------------------*/
LONG PMWindow::background() const
{
  LONG bg_color = 0;

  if( WinQueryPresParam( handle(), PP_BACKGROUNDCOLOR, 0,
                         NULL, sizeof(RGB), &bg_color,
                         QPF_NOINHERIT | QPF_PURERGBCOLOR ) == 0 )
  {
    bg_color = SYSCLR_ENTRYFIELD;
  }

  return bg_color;
}

/*--------------------------------------------------
 * Returns the foreground color value of the window
 *-------------------------------------------------*/
LONG PMWindow::foreground() const
{
  LONG fg_color = 0;

  if( WinQueryPresParam( handle(), PP_FOREGROUNDCOLOR, 0,
                         NULL, sizeof(RGB), &fg_color,
                         QPF_NOINHERIT | QPF_PURERGBCOLOR ) == 0 )
  {
    fg_color = SYSCLR_WINDOWTEXT;
  }

  return fg_color;
}

/*--------------------------------------------------
 * Draws the 3D border
 *-------------------------------------------------*/
void PMWindow::draw_3dborder( HPS ps, const SHAPE& rect, long thickness )
{
  LONG palette_format;

  if( !GpiQueryColorData( ps, 1, &palette_format ))
    PM_THROW_GUIERROR();

  LONG white    = palette_format == LCOLF_RGB ? RGB_WHITE    : CLR_WHITE;
  LONG darkgray = palette_format == LCOLF_RGB ? RGB_DARKGRAY : CLR_DARKGRAY;
  LONG palegray = palette_format == LCOLF_RGB ? RGB_PALEGRAY : CLR_PALEGRAY;
  LONG black    = palette_format == LCOLF_RGB ? RGB_BLACK    : CLR_BLACK;

  POINTL pos;
  GpiSetLineWidth( ps, LINEWIDTH_NORMAL );

  pos.y = rect.y;
  pos.x = rect.x;
  GpiMove( ps, &pos );

  GpiSetColor( ps, white );
  pos.x = rect.x+rect.cx-1;
  GpiLine( ps, &pos );
  pos.y = rect.y+rect.cy-1;
  GpiLine( ps, &pos );

  GpiSetColor( ps, darkgray );
  pos.x = rect.x;
  GpiLine( ps, &pos );
  pos.y = rect.y;
  GpiLine( ps, &pos );

  if( thickness > 1 )
  {
    pos.y = rect.y+1;
    pos.x = rect.x+1;
    GpiMove( ps, &pos );

    GpiSetColor( ps, palegray );
    pos.x = rect.x+rect.cx-2;
    GpiLine( ps, &pos );
    pos.y = rect.y+rect.cy-2;
    GpiLine( ps, &pos );

    GpiSetColor( ps, black );
    pos.x = rect.x+1;
    GpiLine( ps, &pos );
    pos.y = rect.y+1;
    GpiLine( ps, &pos );
  }
}

/*--------------------------------------------------
 * Handle events
 *-------------------------------------------------*/
PMWindow& PMWindow::handle_events()
{
  if( !win_default_handler )
  {
    win_default_handler = WinSubclassWindow( handle(), PMWindow::win_proc );

    if( !win_default_handler )
      PM_THROW_GUIERROR();

    if( !WinSetWindowPtr( handle(), 0, this ))
      PM_THROW_GUIERROR();
  }
  return *this;
}

/*--------------------------------------------------
 * Stop handling events
 *-------------------------------------------------*/
PMWindow& PMWindow::stop_handling_events()
{
  if( WinIsWindow( PMGUI::hab(), handle()))
  {
    if( !WinSetWindowPtr( handle(), 0, 0 ))
      PM_THROW_GUIERROR();

    if( WinSubclassWindow( handle(), win_default_handler ) == 0 )
      PM_THROW_GUIERROR();
  }
  return *this;
}

/*--------------------------------------------------
 * Dispatch all not catched system native events
 *-------------------------------------------------*/
MRESULT PMWindow::dispatch_everything( ULONG msg, MPARAM mp1, MPARAM mp2 )
{
  return win_default_handler( handle(), msg, mp1, mp2 );
}

/*--------------------------------------------------
 * Dispatch system native event
 *-------------------------------------------------*/
MRESULT PMWindow::dispatch( ULONG msg, MPARAM mp1, MPARAM mp2 )
{
  return dispatch_everything( msg, mp1, mp2 );
}

/*--------------------------------------------------
 * Message processing
 *-------------------------------------------------*/
MRESULT _System PMWindow::win_proc( HWND window, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
  PMWindow* window_ptr = (PMWindow*)WinQueryWindowPtr( window, 0 );

  if( window_ptr )
    return window_ptr->dispatch( msg, mp1, mp2 );
  else
    return 0;
}
