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
#define  INCL_ERRORS
#include <os2.h>
#include <stdlib.h>
#include <stdio.h>

#include "pm_error.h"
#include "pm_gui.h"
#include "cmap_clipboard.h"

#define  CF_RTF  "Rich Text Format"

/*--------------------------------------------------
 * Clear clipboard
 *--------------------------------------------------*/
void CMapClipboard::clear()
{
  if( !WinOpenClipbrd ( PMGUI::hab()))
    PM_THROW_GUIERROR();

  if( !WinEmptyClipbrd( PMGUI::hab()))
    PM_THROW_GUIERROR();

  if( !WinCloseClipbrd( PMGUI::hab()))
    PM_THROW_GUIERROR();
}

/*--------------------------------------------------
 * If the clipboard is empty, true is returned
 *--------------------------------------------------*/
BOOL CMapClipboard::is_empty()
{
  BOOL is_empty = TRUE;

  if( !WinOpenClipbrd ( PMGUI::hab()))
    PM_THROW_GUIERROR();

  if( WinQueryClipbrdData( PMGUI::hab(), CF_TEXT ))
    is_empty = FALSE;
  else
  {
    ATOM cf_rtf = WinFindAtom( WinQuerySystemAtomTable(), CF_RTF );
    is_empty    = !cf_rtf || !WinQueryClipbrdData( PMGUI::hab(), cf_rtf );
  }

  if( !WinCloseClipbrd( PMGUI::hab()))
    PM_THROW_GUIERROR();

  return is_empty;
}

/*--------------------------------------------------
 * Inserts the string as the last string
 *--------------------------------------------------*/
void CMapClipboard::push_back( const char* string,
                               const char* font  , ULONG cp, ULONG flags )
{
  if( !WinOpenClipbrd ( PMGUI::hab()))
    PM_THROW_GUIERROR();

  // Prepare current attributes

  char* clipboard_text = (char*)WinQueryClipbrdData( PMGUI::hab(), CF_TEXT );
  LONG  clipboard_len  = clipboard_text ? strlen(clipboard_text) : 0;

  // Prepare new ascii and RTF texts

  char* rtf_template = "{\\rtf1\\ansi{\\fonttbl{\\f0\\fnil \\cpg%u %s;}} \\f0 ";
  char* asc_data     = 0;
  char* rtf_data     = 0;

  ATOM cf_rtf = WinFindAtom( WinQuerySystemAtomTable(), CF_RTF );
  if( !cf_rtf ) cf_rtf = WinAddAtom( WinQuerySystemAtomTable(), CF_RTF );
  if( !cf_rtf ) PM_THROW_GUIERROR();

  APIRET rc;

  rc = DosAllocSharedMem((void**)&asc_data, NULL,
                         clipboard_len+strlen(string)+1,
                         PAG_WRITE | OBJ_GIVEABLE | PAG_COMMIT );
  if( rc != NO_ERROR )
    PM_THROW_OS2ERROR(rc);

  rc = DosAllocSharedMem((void**)&rtf_data, NULL,
                         clipboard_len*4+strlen(font)+strlen(rtf_template)+strlen(string)+1,
                         PAG_WRITE | OBJ_GIVEABLE | PAG_COMMIT );
  if( rc != NO_ERROR )
    PM_THROW_OS2ERROR(rc);

  if( clipboard_text )
    strcpy( asc_data, clipboard_text );

  strcat ( asc_data, string );
  sprintf( rtf_data, rtf_template, cp, font );

  char* trg = rtf_data + strlen(rtf_data);
  char* src = asc_data;

  while( *src ) {
    sprintf( trg, "\\'%02x", *src++ );
    trg += 4;
  }

  *trg++ = '}';
  *trg   = 0;

  // Clear clipboard and set new data

  if( !WinEmptyClipbrd( PMGUI::hab()))
    PM_THROW_GUIERROR();

  if( flags & rtf   )
    if( !WinSetClipbrdData( PMGUI::hab(), (ULONG)rtf_data, cf_rtf , CFI_POINTER ))
      PM_THROW_GUIERROR();

  if( flags & ascii )
    if( !WinSetClipbrdData( PMGUI::hab(), (ULONG)asc_data, CF_TEXT, CFI_POINTER ))
      PM_THROW_GUIERROR();

  if( !WinCloseClipbrd( PMGUI::hab()))
    PM_THROW_GUIERROR();
}

