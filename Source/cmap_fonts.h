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
#ifndef _CMAP_FONTS_H
#define _CMAP_FONTS_H
/**@#+*/

/**
 * Characters Map/2 fonts management class.
 *
 * Provide support for managing system fonts.
 * @author Dmitry A Steklenev
 */

class CMapFonts {

  public:
    /** Returns available font metrics. */
    static FONTMETRICS* metrics();
    /** Returns number of font metrics available. */
    static ULONG metrics_count ();

    /**
     * Refreshes font metrics list.
     *
     * Updates the list of font metrics mirroring the last changes
     * in the system. After that the pointer obtained earlier from
     * metrics() can be invalid.
     */

    static void refresh();

  private:
    static FONTMETRICS* fonts_metrics;
    static ULONG        count_metrics;
};

#if !defined(__ccdoc__)
/*--------------------------------------------------
 * Returns number of font metrics available
 *--------------------------------------------------*/
inline ULONG CMapFonts::metrics_count()
{
  return count_metrics;
}
#endif
/**@#-*/
#endif
