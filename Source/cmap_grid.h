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
#ifndef _CMAP_GRID_H
#define _CMAP_GRID_H

#include "pm_window.h"
#include "pm_thread.h"
#include "pm_exclusion.h"
/**@#+*/

/**
 * Characters Map/2 grid window class.
 * @author Dmitry A Steklenev
 */

class CMapGrid: public PMWindow, protected PMThread {

  typedef PMWindow inherited;

  public:
    /** Constructs the grid window. */
    CMapGrid( SHORT id, HWND parent, HWND owner, long x, long y );
    /** Destructs the grid window. */
   ~CMapGrid();

    /** Selects the displayed font. */
    CMapGrid& select_font( const FATTRS& fonts );
    /** Returns the selected character code. */
    char selected() const;
    /** Selects the specified character code. */
    CMapGrid& select( char code );

  protected:
    /** Dispatch system native events. */
    virtual MRESULT dispatch( ULONG msg, MPARAM mp1, MPARAM mp2 );
    /** Dispatch keyboard events. */
    virtual MRESULT dispatch_keyboard( ULONG msg, MPARAM mp1, MPARAM mp2 );

  private:
    void  run  ();
    void  clear();

    /** Returns calculated cell's rectangle. */
    SHAPE cell_shape( char code ) const;
    /** Returns calculated cell's rectangle. */
    SHAPE cell_shape(           ) const;

    static const long grid_cell_size;
    static const long grid_dimension;
    static const long grid_font_id;

    HDC         grid_dc;
    HPS         grid_ps;
    HBITMAP     grid_bitmap;
    HQUEUE      grid_queue;
    char        grid_selected;
    PMExclusion grid_sentry;
};

#if !defined(__ccdoc__)
/*--------------------------------------------------
 * Returns the selected character code
 *--------------------------------------------------*/
inline char CMapGrid::selected() const
{
  return grid_selected;
}

/*--------------------------------------------------
 * Returns calculated cell's rectangle
 *--------------------------------------------------*/
inline SHAPE CMapGrid::cell_shape() const
{
  return cell_shape( grid_selected );
}
#endif

/**@#-*/
#endif
