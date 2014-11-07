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
#define  INCL_GPI
#define  INCL_ERRORS
#include <os2.h>

#include "cmap_grid.h"
#include "cmap_ids.h"
#include "pm_gui.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

const long CMapGrid::grid_cell_size = 21;
const long CMapGrid::grid_dimension = CMapGrid::grid_cell_size*16+1;
const long CMapGrid::grid_font_id   = 1;

/*--------------------------------------------------
 * Constructs the grid window
 *--------------------------------------------------*/
CMapGrid::CMapGrid( SHORT id, HWND parent, HWND owner, long x, long y )

: inherited( id, "", parent, owner, x, y,
             grid_dimension+4, grid_dimension+4, WS_VISIBLE | WS_TABSTOP | WS_GROUP ),

  grid_selected(0)
{
  DEVOPENSTRUC      dop;
  BITMAPINFOHEADER2 bmp_info;
  APIRET rc;
  char   queue_name[_MAX_PATH];

  SIZEL size = { grid_dimension, grid_dimension };

  memset( &dop, 0, sizeof(dop));
  dop.pszDriverName = "DISPLAY";

  grid_dc = DevOpenDC( PMGUI::hab(), OD_MEMORY, "*", 5L,
                      (PDEVOPENDATA)&dop, WinOpenWindowDC( handle()));

  if( grid_dc == DEV_ERROR )
    PM_THROW_GUIERROR();

  grid_ps = GpiCreatePS( PMGUI::hab(), grid_dc, &size, PU_PELS | GPIA_ASSOC );

  if( grid_ps == GPI_ERROR )
    PM_THROW_GUIERROR();

  GpiCreateLogColorTable( grid_ps, 0, LCOLF_RGB, 0, 0, 0 );
  memset( &bmp_info, 0, sizeof(bmp_info));

  bmp_info.cbFix     = sizeof(bmp_info);
  bmp_info.cx        = grid_dimension;
  bmp_info.cy        = grid_dimension;
  bmp_info.cPlanes   = 1;
  bmp_info.cBitCount = 8;

  grid_bitmap = GpiCreateBitmap( grid_ps, &bmp_info, 0L, NULL, NULL );

  if( grid_bitmap == GPI_ERROR )
    PM_THROW_GUIERROR();

  if( GpiSetBitmap( grid_ps, grid_bitmap ) == HBM_ERROR )
    PM_THROW_GUIERROR();

  sprintf( queue_name, "\\QUEUES\\CMAP%08X", PMGUI::pid());

  if(( rc = DosCreateQueue( &grid_queue, QUE_FIFO, queue_name )) != NO_ERROR )
    PM_THROW_OS2ERROR(rc);

  clear();
  start(); // Activate secondary thread
}

/*--------------------------------------------------
 * Destructs the grid window
 *--------------------------------------------------*/
CMapGrid::~CMapGrid()
{
  APIRET rc;
  stop();

  if(( rc = DosCloseQueue( grid_queue ))  != NO_ERROR  )
    PM_THROW_OS2ERROR(rc);

  if( GpiSetBitmap( grid_ps, NULLHANDLE ) == HBM_ERROR )
    PM_THROW_GUIERROR();

  if( !GpiDeleteBitmap( grid_bitmap ))
    PM_THROW_GUIERROR();

  if( !GpiDestroyPS( grid_ps ))
    PM_THROW_GUIERROR();

  if( DevCloseDC( grid_dc ) == DEV_ERROR )
    PM_THROW_GUIERROR();
}

/*--------------------------------------------------
 * Selects the displayed font
 *--------------------------------------------------*/
CMapGrid& CMapGrid::select_font( const FATTRS& font )
{
  FATTRS* attrs = new FATTRS;
  APIRET  rc;
  memcpy( attrs, &font, sizeof(FATTRS));

  if(( rc = DosWriteQueue( grid_queue, 0, sizeof(attrs), attrs, 0 )) != NO_ERROR )
    PM_THROW_OS2ERROR(rc);

  return *this;
}

/*--------------------------------------------------
 * Returns calculated cell's rectangle
 *--------------------------------------------------*/
SHAPE CMapGrid::cell_shape( char code ) const
{
  SHAPE rect = { grid_cell_size*(     code % 16 ) + 2,
                 grid_cell_size*(15 - code / 16 ) + 2,
                 grid_cell_size+1,
                 grid_cell_size+1 };
  return rect;
}

/*--------------------------------------------------
 * Selects the specified character code
 *--------------------------------------------------*/
CMapGrid& CMapGrid::select( char code )
{
  if( code != grid_selected )
  {
    invalidate( cell_shape());
    grid_selected = code;
    invalidate( cell_shape());
    WinPostMsg( owner(), CMGR_SELECT, MPFROMSHORT(code), 0 );
  }
  return *this;
}

/*--------------------------------------------------
 * Clearing the grid
 *--------------------------------------------------*/
void CMapGrid::clear()
{
  // WinFillRect must use inclusive-exclusive rectangles
  RECTL  rect = { 0, 0, grid_dimension, grid_dimension };
  POINTL pos;

  WinFillRect( grid_ps, &rect, background());
  GpiSetColor( grid_ps, RGB_DARKGRAY );

  for( int i = 1; i < 16; i++  )
  {
    pos.x = 0;
    pos.y = grid_cell_size*i;
    GpiMove( grid_ps, &pos );
    pos.x = grid_cell_size*16;
    GpiLine( grid_ps, &pos );
    pos.y = 0;
    pos.x = grid_cell_size*i;
    GpiMove( grid_ps, &pos );
    pos.y = grid_cell_size*16;
    GpiLine( grid_ps, &pos );
  }
}

/*--------------------------------------------------
 * Secondary thread
 *--------------------------------------------------*/
void CMapGrid::run()
{
  try
  {
    REQUESTDATA  rd;
    ULONG        rd_length;
    FATTRS*      fattrs;
    BYTE         priority;
    APIRET       rc;
    ULONG        have_more_events;
    POINTL       pos;
    RECTL        rct;
    FONTMETRICS  metrics;

    for(;;)
    {
      rc = DosReadQueue( grid_queue, &rd, &rd_length, (PPVOID)&fattrs, 0,
                         DCWW_WAIT, &priority, NULLHANDLE );

      WinPostMsg( owner(), CMGR_RENDERING, 0, 0 );
      grid_sentry.request();

      if( rc != NO_ERROR )
        PM_THROW_OS2ERROR(rc);

      clear();

      // Load the specified font and release loaded earlier.

      if( rd_length )
      {
        if( GpiQueryCharSet( grid_ps ) > 0 )
        {
          GpiSetCharSet ( grid_ps, LCID_DEFAULT );
          GpiDeleteSetId( grid_ps, grid_font_id );
        }

        GpiCreateLogFont( grid_ps, NULL, grid_font_id, fattrs );
        GpiSetCharSet( grid_ps, grid_font_id );
        GpiSetTextAlignment( grid_ps, TA_CENTER, TA_HALF );
        delete fattrs;
      }

      // Draw font glyphs

      GpiSetColor    ( grid_ps, foreground());
      #ifdef CMGRID_DEBUG
      GpiSetBackColor( grid_ps, RGB_YELLOW  );
      #endif

      if( !GpiQueryFontMetrics( grid_ps, sizeof(metrics), &metrics ))
        PM_THROW_GUIERROR();

      LONG box_width  = 13;
      LONG box_height = 13;

      // set charbox height so that max-baseline-extent fits within the cell
      if( metrics.lEmHeight && metrics.lMaxBaselineExt )
        // (workaround for abnormally large lMaxBaselineExt in some fonts)
        box_height = (1.5*metrics.lEmHeight) > metrics.lMaxBaselineExt ?
                     (grid_cell_size-4)*metrics.lEmHeight/metrics.lMaxBaselineExt :
                     (grid_cell_size-4)/1.5;

      // set charbox width so that max-char-increment fits within the cell
      if( metrics.lEmInc && metrics.lMaxCharInc )
        // (workaround for abnormally large lMaxCharInc in some fonts)
        box_width = (2*metrics.lAveCharWidth) > metrics.lMaxCharInc ?
                    (grid_cell_size-4)*metrics.lEmInc/metrics.lMaxCharInc :
                    box_height;

      SIZEF sizef = { MAKEFIXED( box_width < box_height ? box_width : box_height, 0 ),
                      MAKEFIXED( box_width < box_height ? box_width : box_height, 0 ) };
      GpiSetCharBox ( grid_ps, &sizef );

      have_more_events = 0;

      for( int i = 0; i < 16 && !have_more_events; i++  )
        for( int j = 0; j < 16 && !have_more_events; j++ )
        {
          DosQueryQueue( grid_queue, &have_more_events );

          LONG cell_x = (   j)*grid_cell_size;
          LONG cell_y = (15-i)*grid_cell_size;

          pos.x       = cell_x+grid_cell_size/2;
          pos.y       = cell_y+grid_cell_size/2+1;
          rct.xLeft   = cell_x+2;
          rct.yBottom = cell_y+2;
          rct.xRight  = cell_x+grid_cell_size-2;
          rct.yTop    = cell_y+grid_cell_size-2;

          char c = i*16+j;
          #ifdef CMGRID_DEBUG
          GpiCharStringPosAt( grid_ps, &pos, &rct, CHS_OPAQUE | CHS_CLIP, 1, &c, 0 );
          #else
          GpiCharStringPosAt( grid_ps, &pos, &rct, CHS_CLIP, 1, &c, 0 );
          #endif
        }

      grid_sentry.release();
      if( !have_more_events )
      {
        WinPostMsg( owner(), CMGR_RENDERING, MPFROMLONG(1),0 );
        invalidate();
      }
    }
  }
  catch( PMError& error )
  {
    error.display();
    exit(1);
  }
}

/*--------------------------------------------------
 * Dispatch system native events
 *--------------------------------------------------*/
MRESULT CMapGrid::dispatch( ULONG msg, MPARAM mp1, MPARAM mp2 )
{
  switch( msg )
  {
    case WM_PAINT:
    {
      HPS    ps      = WinBeginPaint( handle(), NULL, NULL );
      SHAPE  rect    = shape();
      POINTL copy[3] = { 2, 2, rect.cx-2, rect.cy-2, 0, 0 };
      POINTL pos;

      grid_sentry.request();
      GpiBitBlt( ps, grid_ps, 3L, copy, ROP_SRCCOPY, BBO_IGNORE );
      grid_sentry.release();
      GpiCreateLogColorTable( ps, 0, LCOLF_RGB, 0, 0, 0 );

      if( has_focus())
      {
        GpiSetColor    ( ps, foreground()       );
        GpiSetLineWidth( ps, LINEWIDTH_NORMAL   );
        GpiSetLineType ( ps, LINETYPE_ALTERNATE );

        pos.x = 2;
        pos.y = 2;
        GpiMove( ps, &pos );
        pos.x += rect.cx - 5;
        pos.y += rect.cy - 5;
        GpiBox ( ps, DRO_OUTLINE, &pos, 0, 0 );
      }

      GpiSetColor    ( ps, RGB_RED          );
      GpiSetLineWidth( ps, LINEWIDTH_THICK  );
      GpiSetLineType ( ps, LINETYPE_DEFAULT );

      SHAPE cell_rect = cell_shape();

      pos.x  = cell_rect.x  + 1;
      pos.y  = cell_rect.y  + 1;
      GpiMove( ps, &pos );
      pos.x += cell_rect.cx - 2;
      pos.y += cell_rect.cy - 2;
      GpiBox ( ps, DRO_OUTLINE, &pos, 0, 0 );

      rect.x = 0;
      rect.y = 0;
      draw_3dborder( ps, rect    );

      WinEndPaint( ps );
      return 0;
    }

    case WM_CONTEXTMENU:
    {
      POINTL pos = { SHORT1FROMMP(mp1), SHORT2FROMMP(mp1) };
      WinMapWindowPoints( handle(), owner(), &pos, 1 );
      WinPostMsg( owner(), msg, MPFROM2SHORT( pos.x, pos.y), mp2 );
      return 0;
    }

    case WM_PRESPARAMCHANGED:
    {
      // Clear the grid for more fast displaying of changes and
      // send the notice to the secondary thread.
      clear();

      APIRET rc = DosWriteQueue( grid_queue, 0, 0, 0, 0 );
      if( rc != NO_ERROR )
        PM_THROW_OS2ERROR(rc);

      break;
    }

    case WM_SETFOCUS:
      invalidate();
      break;

    case WM_BUTTON1UP:
      release_pointer();
      return 0;

    case WM_BUTTON1DOWN:
      set_focus      ();
      capture_pointer();

    case WM_BUTTON2DOWN: // and WM_BUTTON1DOWN
    case WM_MOUSEMOVE:
    {
      HPOINTER arrow = WinQuerySysPointer( HWND_DESKTOP, SPTR_ARROW, FALSE );
      WinSetPointer( HWND_DESKTOP, arrow );

      if( has_pointer_captured() || msg == WM_BUTTON2DOWN )
      {
        POINTL pos = { LOUSHORT(mp1)-2, HIUSHORT(mp1)-2 };

        if( pos.x >= 0 && pos.x < grid_dimension - 1 &&
            pos.y >= 0 && pos.y < grid_dimension - 1 )
            select((grid_dimension - pos.y - 2)/grid_cell_size*16 + pos.x/grid_cell_size );
      }
      return 0;
    }

    case WM_BUTTON1DBLCLK:
      WinPostMsg( owner(), CMGR_CLICKED, MPFROMSHORT(selected()), 0 );
      return 0;

    case WM_COMMAND:
      switch ( SHORT1FROMMP(mp1))
      {
        case ID_ITM_ADD:
          WinPostMsg( owner(), CMGR_CLICKED, MPFROMSHORT(selected()), 0 );
          return 0;

        case ID_ITM_CLEAR:
          WinPostMsg( owner(), msg, MPFROMSHORT( ID_BTN_CLEAR ), 0 );
          return 0;
      }
      break;

    case WM_CHAR:
      return dispatch_keyboard( msg, mp1, mp2 );
  }

  return inherited::dispatch( msg, mp1, mp2 );
}

/*--------------------------------------------------
 * Dispatch keyboard events
 *--------------------------------------------------*/
MRESULT CMapGrid::dispatch_keyboard( ULONG msg, MPARAM mp1, MPARAM mp2 )
{
  if( msg != WM_CHAR || CHARMSG(&msg)->fs & (KC_ALT | KC_KEYUP))
    return inherited::dispatch( msg, mp1, mp2 );

  if( CHARMSG(&msg)->fs & KC_CTRL )
  {
    switch( CHARMSG(&msg)->vkey )
    {
      case VK_INSERT:
        WinPostMsg( owner(), CMGR_CLICKED, MPFROMSHORT(selected()), 0 );
        return 0;
    }
  }
  else
  {
    switch( CHARMSG(&msg)->vkey )
    {
      case VK_RIGHT:
        if( grid_selected % 16 != 15 )
          select( grid_selected + 1  );
        return 0;

      case VK_END:
        if( grid_selected % 16 != 15 )
          select(( grid_selected / 16 + 1 ) * 16 - 1 );
        return 0;

      case VK_LEFT:
        if( grid_selected % 16 != 0 )
          select( grid_selected - 1 );
        return 0;

      case VK_HOME:
        if( grid_selected % 16 != 0 )
          select(( grid_selected / 16 ) * 16 );
        return 0;

      case VK_DOWN:
        if( grid_selected < 15*16 )
          select( grid_selected + 16 );
        return 0;

      case VK_PAGEDOWN:
        if( grid_selected < 15*16 )
          select( 15*16 + grid_selected % 16 );
        return 0;

      case VK_UP:
        if( grid_selected > 15 )
          select( grid_selected - 16 );
        return 0;

      case VK_PAGEUP:
        if( grid_selected > 15 )
          select( grid_selected % 16 );
        return 0;

      case VK_NEWLINE:
      case VK_ENTER:
        WinPostMsg( owner(), CMGR_CLICKED, MPFROMSHORT(selected()), 0 );
        return 0;

      case VK_DELETE:
        WinPostMsg( owner(), WM_COMMAND, MPFROMSHORT( ID_BTN_CLEAR ), 0 );
        return 0;
    }
  }

  return inherited::dispatch( msg, mp1, mp2 );
}
