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
#ifndef _CMAP_SPECS_H
#define _CMAP_SPECS_H

#include "pm_frame.h"
/**@#+*/

/**
 * Characters Map/2 font specifications window.
 * @author Dmitry A Steklenev
 */

class CMapSpecs: public PMFrame {

  typedef PMFrame inherited;

  public:
    /** Constructs the specifications window. */
    CMapSpecs( SHORT id, PMWindow* owner );
    /** Selects the displayed font. */
    CMapSpecs& select_font( const FONTMETRICS& font );

  protected:
    /** Dispatch system native events. */
    virtual MRESULT dispatch( ULONG msg, MPARAM mp1, MPARAM mp2 );

  private:
    struct metric {

      RECORDCORE core;
      char* key;
      char* val;
      char  key_data[256];
      char  val_data[256];
    };

    PMWindow cnt_specs;
};

/**@#-*/
#endif
