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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define  INCL_WIN
#define  INCL_DOS
#define  INCL_GPI
#define  INCL_ERRORS
#include <os2.h>

#include "cmap_glyph.h"
#include "pm_gui.h"

#undef CMGLYPH_DEBUG
const long CMapGlyph::glyph_font_id = 1;

/*--------------------------------------------------
 * Constructs the glyph window
 *--------------------------------------------------*/
CMapGlyph::CMapGlyph( SHORT id, HWND parent, HWND owner,
                                long x, long y, long cx, long cy )

: inherited( id, "", parent, owner, x, y, cx, cy, WS_VISIBLE ),
  glyph_selected(0)
{
  DEVOPENSTRUC      dop;
  BITMAPINFOHEADER2 bmp_info;
  SIZEL size = { cx-4, cy-4 };

  memset( &dop, 0, sizeof(dop));
  dop.pszDriverName = "DISPLAY";

  glyph_dc = DevOpenDC( PMGUI::hab(), OD_MEMORY, "*", 5L,
                      (PDEVOPENDATA)&dop, WinOpenWindowDC( handle()));

  if( glyph_dc == DEV_ERROR )
    PM_THROW_GUIERROR();

  glyph_ps = GpiCreatePS( PMGUI::hab(), glyph_dc, &size, PU_PELS | GPIA_ASSOC );

  if( glyph_ps == GPI_ERROR )
    PM_THROW_GUIERROR();

  GpiCreateLogColorTable( glyph_ps, 0, LCOLF_RGB, 0, 0, 0 );
  memset( &bmp_info, 0, sizeof(bmp_info));

  bmp_info.cbFix     = sizeof(bmp_info);
  bmp_info.cx        = cx-4;
  bmp_info.cy        = cy-4;
  bmp_info.cPlanes   = 1;
  bmp_info.cBitCount = 8;

  glyph_bitmap = GpiCreateBitmap( glyph_ps, &bmp_info, 0L, NULL, NULL );

  if( glyph_bitmap == GPI_ERROR )
    PM_THROW_GUIERROR();

  if( GpiSetBitmap( glyph_ps, glyph_bitmap ) == HBM_ERROR )
    PM_THROW_GUIERROR();
}

/*--------------------------------------------------
 * Destructs the glyph window
 *--------------------------------------------------*/
CMapGlyph::~CMapGlyph()
{
  if( GpiSetBitmap( glyph_ps, NULLHANDLE ) == HBM_ERROR )
    PM_THROW_GUIERROR();

  if( !GpiDeleteBitmap( glyph_bitmap ))
    PM_THROW_GUIERROR();

  if( !GpiDestroyPS( glyph_ps ))
    PM_THROW_GUIERROR();

  if( DevCloseDC( glyph_dc ) == DEV_ERROR )
    PM_THROW_GUIERROR();
}

/*--------------------------------------------------
 * Prepare gluph
 *--------------------------------------------------*/
CMapGlyph& CMapGlyph::draw_it()
{
  // WinFillRect use inclusive-exclusive rectangles
  RECTL  rect = { 0, 0, shape().cx-3, shape().cy-3 };
  POINTL pos;

  WinFillRect( glyph_ps, &rect, background());

  GpiSetColor    ( glyph_ps, foreground());
  #ifdef CMGLYPH_DEBUG
  GpiSetBackColor( glyph_ps, RGB_YELLOW  );
  #endif

  rect.xLeft   += 8;
  rect.xRight  -= 8;
  rect.yBottom += 8;
  rect.yTop    -= 8;

  pos.x = (rect.xLeft   + rect.xRight)/2;
  pos.y = (rect.yBottom + rect.yTop  )/2;

  FONTMETRICS metrics;
  if( !GpiQueryFontMetrics( glyph_ps, sizeof(metrics), &metrics ))
    PM_THROW_GUIERROR();

  LONG box_width  = 70;
  LONG box_height = 70;

  if( metrics.lEmHeight && metrics.lMaxBaselineExt )
    box_height = (rect.yTop-rect.yBottom+1)*metrics.lEmHeight/metrics.lMaxBaselineExt;
  if( metrics.lEmInc && metrics.lMaxCharInc )
    box_width  = (rect.xRight-rect.xLeft+1)*metrics.lEmInc/metrics.lMaxCharInc;

  SIZEF sizef = { MAKEFIXED( box_width < box_height ? box_width : box_height, 0 ),
                  MAKEFIXED( box_width < box_height ? box_width : box_height, 0 ) };

  GpiSetCharBox ( glyph_ps, &sizef );

  GpiSetTextAlignment( glyph_ps, TA_CENTER, TA_HALF );
  #ifdef CMGLYPH_DEBUG
  GpiCharStringPosAt ( glyph_ps, &pos, &rect, CHS_OPAQUE | CHS_CLIP, 1, &glyph_selected, 0 );
  #else
  GpiCharStringPosAt ( glyph_ps, &pos, &rect, CHS_CLIP, 1, &glyph_selected, 0 );
  #endif

  invalidate();
  return *this;
}

/*--------------------------------------------------
 * Selects the displayed font
 *--------------------------------------------------*/
CMapGlyph& CMapGlyph::select_font( const FATTRS& fattrs )
{
  if( GpiQueryCharSet( glyph_ps ) > 0 )
  {
    GpiSetCharSet ( glyph_ps, LCID_DEFAULT  );
    GpiDeleteSetId( glyph_ps, glyph_font_id );
  }

  GpiCreateLogFont( glyph_ps, NULL, glyph_font_id, (PFATTRS)&fattrs );
  GpiSetCharSet( glyph_ps, glyph_font_id );
  return draw_it();
}

/*--------------------------------------------------
 * Selects the specified character code
 *--------------------------------------------------*/
CMapGlyph& CMapGlyph::select( char code )
{
  if( code != glyph_selected )
  {
    glyph_selected = code;
    draw_it();
  }
  return *this;
}

/*--------------------------------------------------
 * Dispatch system native events
 *--------------------------------------------------*/
MRESULT CMapGlyph::dispatch( ULONG msg, MPARAM mp1, MPARAM mp2 )
{
  switch( msg )
  {
    case WM_PAINT:
    {
      HPS   ps   = WinBeginPaint( handle(), NULL, NULL );
      SHAPE rect = shape();

      // GpiBitBlt use inclusive-exclusive rectangles?
      POINTL copy[4] = { 2, 2, rect.cx-2, rect.cy-2, 0, 0, rect.cx-4, rect.cy-4 };

      FONTMETRICS metrics;
      if( !GpiQueryFontMetrics( glyph_ps, sizeof(metrics), &metrics ))
        PM_THROW_GUIERROR();

      if(!( metrics.fsDefn & FM_DEFN_OUTLINE ))
      {
        copy[2].x = (copy[3].x-metrics.lMaxBaselineExt-6)/2;
        copy[2].y = (copy[3].y-metrics.lMaxBaselineExt-6)/2;
        copy[3].x = (copy[2].x+metrics.lMaxBaselineExt+6)+1;
        copy[3].y = (copy[2].y+metrics.lMaxBaselineExt+6)+1;
      }

      GpiBitBlt( ps, glyph_ps, 4L, copy, ROP_SRCCOPY, BBO_IGNORE );

      rect.x = 0;
      rect.y = 0;
      draw_3dborder( ps, rect );
      WinEndPaint( ps );
      return 0;
    }

    case WM_PRESPARAMCHANGED:
      draw_it();
      break;

    case WM_QUERYDLGCODE:
      return MRFROMLONG(DLGC_STATIC);
  }

  return inherited::dispatch( msg, mp1, mp2 );
}

