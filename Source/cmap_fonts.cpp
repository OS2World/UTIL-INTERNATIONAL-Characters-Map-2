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

#include "cmap_fonts.h"
#include "pm_gui.h"
#include "pm_error.h"

FONTMETRICS* CMapFonts::fonts_metrics = 0;
ULONG        CMapFonts::count_metrics = 0;

/*--------------------------------------------------
 * Refreshes font metrics list
 *--------------------------------------------------*/
void CMapFonts::refresh()
{
  HPS  ps = WinGetPS( HWND_DESKTOP );
  LONG rem_fonts = 0;
  LONG req_fonts = 0;

  rem_fonts = GpiQueryFonts( ps, QF_PUBLIC, NULL, &req_fonts,
                                            sizeof(FONTMETRICS), 0 );
  if( rem_fonts == GPI_ALTERROR )
    PM_THROW_GUIERROR();

  if( count_metrics < rem_fonts )
  {
    delete fonts_metrics;
    fonts_metrics = new FONTMETRICS[rem_fonts];
  }

  req_fonts = rem_fonts;
  rem_fonts = GpiQueryFonts( ps, QF_PUBLIC, NULL, &req_fonts,
                                 sizeof(FONTMETRICS), fonts_metrics );
  if( rem_fonts != 0 )
    PM_THROW_GUIERROR();

  WinReleasePS(ps);
  count_metrics = req_fonts;
}

/*--------------------------------------------------
 * Returns available font metrics
 *--------------------------------------------------*/
FONTMETRICS* CMapFonts::metrics()
{
  if( !fonts_metrics )
    refresh();

  return fonts_metrics;
}

