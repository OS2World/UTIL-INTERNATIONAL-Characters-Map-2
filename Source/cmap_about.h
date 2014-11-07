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

/**@#-*/
#ifndef _CMAP_ABOUT_H
#define _CMAP_ABOUT_H

#include "pm_frame.h"
/**@#+*/

/**
 * Characters Map/2 about window
 * @author Dmitry A Steklenev
 */

class CMapAbout: public PMFrame {

  typedef PMFrame inherited;

  public:
    /** Constructs the about window. */
    CMapAbout( SHORT id, PMWindow* owner );

  protected:
    /** Dispatch system native events. */
    virtual MRESULT dispatch( ULONG msg, MPARAM mp1, MPARAM mp2 );

  private:
    PMWindow lbl_about;
    PMWindow lbl_version;
    PMWindow lbl_sep;
    PMWindow lbl_author;
    PMWindow btn_ok;
};

/**@#-*/
#endif
