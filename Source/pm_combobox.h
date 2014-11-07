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
#ifndef _PM_COMBOBOX_H
#define _PM_COMBOBOX_H

#include "pm_window.h"
/**@#+*/

/**
 * Combination box window class.
 *
 * The PMComboBox class creates and manages combination box control
 * windows. The PMBaseComboBox class combines an entry field and a
 * list box to form one control containing the features of both.
 *
 * @author Dmitry A Steklenev
 */

class PMComboBox : public PMWindow
{
  typedef PMWindow inherited;

  public:
    /** Constructs the combination box window. */
    PMComboBox( SHORT id, HWND parent, HWND owner, long x, long y,long cx, long cy );

  public:
    /** Inserts the string. */
    int insert( const char* string, ULONG handle = 0, int pos = LIT_SORTASCENDING );

    /** Searches the specified string. */
    int search( const char* string ) const;
    /** Sets the selection state of an item. */
    PMComboBox& select( int pos, BOOL state = TRUE );

    /** Returns the item string. */
    char* string( int pos, char* buffer, int buf_len ) const;
    /** Returns the selected item or LIT_NONE. */
    int selected() const;
    /** Returns the number of strings stored in the combination box. */
    int count() const;
    /** Removes all items from the combination box. */
    PMComboBox& clear();

    /** Sets the handle of the specified combination box item. */
    PMComboBox& set_item_handle( int pos, ULONG handle );
    /** Returns the handle of the specified combination box item. */
    ULONG item_handle( int pos ) const;
};

/**@#-*/
#endif