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
#ifndef _PM_FRAME_H
#define _PM_FRAME_H

#include "pm_window.h"
/**@#+*/

/**
 * Frame window class.
 *
 * A composite window class that can contain child windows
 * of many of the other window classes.
 *
 * @author Dmitry A Steklenev
 */

class PMFrame: public PMWindow {

  typedef PMWindow inherited;

  public:
    /** Constructs the frame window. */
    PMFrame( SHORT       id,
             const char* caption,
             HWND        owner,
             long        x,
             long        y,
             long        cx,
             long        cy,
             ULONG       style  );
  public:

    /** Calculates client shape. */
    virtual SHAPE client_shape() const;
    /** Sets the client window. */
    virtual PMFrame& set_client( HWND client );

    /**
     * Save the current position and size of the window.
     *
     * This method will also save the presentation parameters
     * of the window and all children.
     */

    virtual PMFrame& save( const char* key );

    /**
     * Restore the position and size of the window.
     *
     * This method will also restore the presentation parameters
     * of the window and all children.
     */

    virtual BOOL load( const char* key );

  protected:
    /** Dispatch system native events. */
    virtual MRESULT dispatch( ULONG msg, MPARAM mp1, MPARAM mp2 );

  private:
    HWND frm_client;
};

/**@#-*/
#endif