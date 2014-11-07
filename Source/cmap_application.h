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
#ifndef _CMAP_APPLICATION_H
#define _CMAP_APPLICATION_H

#include "pm_frame.h"
#include "pm_combobox.h"
#include "cmap_status.h"
#include "cmap_grid.h"
#include "cmap_glyph.h"
#include "cmap_specs.h"
/**@#+*/

/**
 * Characters Map/2 application window.
 * @author Dmitry A Steklenev
 */

class CMapApplication: public PMFrame {

  typedef PMFrame inherited;

  public:
    /** Constructs the application window. */
    CMapApplication( SHORT id );
    /** Destructs  the application window. */
   ~CMapApplication();

    /** Sets the input focus to the window. */
    virtual CMapApplication& set_focus();
    /** Displays a state. */
    CMapApplication& set_state( const char* state, ... );

  public:
    /** Selects specified font. */
    CMapApplication& select_font( int pos );
    /** Selects specified font size. */
    CMapApplication& select_size( int pos );

  protected:
    /** Dispatch system native events. */
    virtual MRESULT dispatch( ULONG msg, MPARAM mp1, MPARAM mp2 );

  private:
    CMapStatus lbl_status;
    PMWindow   lbl_font;
    PMComboBox cmb_font;
    PMWindow   lbl_size;
    PMComboBox cmb_size;
    CMapGrid   fnt_grid;
    PMWindow   lbl_codepage;
    PMComboBox cmb_codepage;
    CMapGlyph  fnt_glyph;
    PMWindow   btn_clear;
    PMWindow   btn_specs;
    PMWindow   btn_about;
    CMapSpecs* win_specs;

    HWND  mnu_handle;
    ULONG clb_flags;
};

/**@#-*/
#endif
