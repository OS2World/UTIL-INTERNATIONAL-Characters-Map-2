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
#include <os2.h>

#include "pm_combobox.h"

/*--------------------------------------------------
 * Constructs the combination box window
 *--------------------------------------------------*/
PMComboBox::PMComboBox( SHORT id, HWND parent, HWND  owner,
                        long  x, long y, long cx, long cy )

: inherited(  WC_COMBOBOX, id, "", parent, owner, x, y, cx, cy,
              WS_VISIBLE | WS_TABSTOP | WS_GROUP | CBS_DROPDOWNLIST )
{}

/*--------------------------------------------------
 * Inserts the string
 *--------------------------------------------------*/
int PMComboBox::insert( const char* string, ULONG hitem, int before )
{
  int pos = (int)WinSendMsg( handle(), LM_INSERTITEM,
                             MPFROMSHORT(before),
                             MPFROMP(string));

  if( pos == LIT_ERROR || pos == LIT_MEMERROR )
    PM_THROW_GUIERROR();

  if( hitem )
    set_item_handle( pos, hitem );

  return pos;
}

/*--------------------------------------------------
 * Searches the specified string
 *--------------------------------------------------*/
int PMComboBox::search( const char* string ) const
{
  int pos = (int)WinSendMsg( handle(),
                             LM_SEARCHSTRING,
                             MPFROM2SHORT( 0, LIT_FIRST ),
                             MPFROMP(string));
  if( pos == LIT_ERROR )
    PM_THROW_GUIERROR();

  return pos;
}

/*--------------------------------------------------
 * Sets the selection state of an item
 *--------------------------------------------------*/
PMComboBox& PMComboBox::select( int pos, BOOL state )
{
  if( pos != LIT_NONE )
    if( !WinSendMsg( handle(), LM_SELECTITEM, MPFROMSHORT(pos), MPFROMLONG(state)))
      PM_THROW_GUIERROR();

  return *this;
}

/*--------------------------------------------------
 * Returns the selected item
 *--------------------------------------------------*/
int PMComboBox::selected() const
{
  return (int)WinSendMsg( handle(), LM_QUERYSELECTION,
                          MPFROMLONG(LIT_FIRST), 0 );
}

/*--------------------------------------------------
 * Returns the item string
 *--------------------------------------------------*/
char* PMComboBox::string( int pos, char* buffer, int buf_len ) const
{
  WinSendMsg( handle(), LM_QUERYITEMTEXT,
              MPFROM2SHORT( pos, buf_len), MPFROMP(buffer));

  return buffer;
}

/*--------------------------------------------------
 * Returns the number of strings
 *--------------------------------------------------*/
int PMComboBox::count() const
{
  return (int)WinSendMsg( handle(), LM_QUERYITEMCOUNT, 0, 0 );
}

/*--------------------------------------------------
 * Removes all items from the combination box
 *--------------------------------------------------*/
PMComboBox& PMComboBox::clear()
{
  if( !WinSendMsg( handle(), LM_DELETEALL, 0, 0 ))
    PM_THROW_GUIERROR();

  return *this;
}

/*--------------------------------------------------
 * Sets the handle of the specified item
 *--------------------------------------------------*/
PMComboBox& PMComboBox::set_item_handle( int pos, ULONG hitem )
{
  if( !WinSendMsg( handle(), LM_SETITEMHANDLE, MPFROMSHORT(pos), MPFROMLONG(hitem)))
    PM_THROW_GUIERROR();

  return *this;
}

/*--------------------------------------------------
 * Returns the handle of the specified item
 *--------------------------------------------------*/
ULONG PMComboBox::item_handle( int pos ) const
{
  return (ULONG)WinSendMsg( handle(), LM_QUERYITEMHANDLE, MPFROMSHORT(pos), 0 );
}

