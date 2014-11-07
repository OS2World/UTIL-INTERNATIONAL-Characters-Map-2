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
#ifndef _CMAP_STATUS_H
#define _CMAP_STATUS_H

#include "pm_window.h"
/**@#+*/

/**
 * Characters Map/2 status window.
 * @author Dmitry A Steklenev
 */

class CMapStatus: public PMWindow {

  typedef PMWindow inherited;

  public:
    /** Constructs the presentation window. */
    CMapStatus( SHORT id, HWND parent, HWND owner, long x, long y, long cx, long cy );
    /** Sets the window caption. */
    virtual CMapStatus& set_caption( const char* caption );

  protected:
    /** Dispatch system native events. */
    virtual MRESULT dispatch( ULONG msg, MPARAM mp1, MPARAM mp2 );
};

/**@#-*/
#endif
