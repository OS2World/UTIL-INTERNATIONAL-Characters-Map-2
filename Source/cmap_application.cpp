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
#define  INCL_DOS
#define  INCL_GPI
#define  INCL_ERRORS
#include <os2.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "pm_error.h"
#include "pm_gui.h"
#include "pm_messages.h"
#include "pm_profile.h"
#include "cmap_application.h"
#include "cmap_clipboard.h"
#include "cmap_about.h"
#include "cmap_fonts.h"
#include "cmap_ids.h"

extern const char* application_name;

/*--------------------------------------------------
 * Constructs the application window
 *--------------------------------------------------*/
CMapApplication::CMapApplication( SHORT id )

: inherited   ( id, application_name, 0, 0, 0, 485, 452, FCF_NOBYTEALIGN     |
                                                         FCF_NOMOVEWITHOWNER |
                                                         FCF_TITLEBAR        |
                                                         FCF_SYSMENU         |
                                                         FCF_MINBUTTON       |
                                                         FCF_DLGBORDER       |
                                                         FCF_TASKLIST        |
                                                         FCF_AUTOICON        |
                                                         FCF_CLOSEBUTTON     ),

  lbl_font    ( WC_STATIC, ID_STA_FONT, MSG_LBL_FONT, handle(), handle(),
                  8, 407, 341, 18, WS_VISIBLE | SS_TEXT | DT_LEFT | DT_MNEMONIC | DT_BOTTOM ),
  lbl_size    ( WC_STATIC, ID_STA_SIZE, MSG_LBL_SIZE, handle(), handle(),
                355, 407, 121, 18, WS_VISIBLE | SS_TEXT | DT_LEFT | DT_MNEMONIC | DT_BOTTOM ),
  lbl_codepage( WC_STATIC, ID_STA_CODEPAGE, MSG_LBL_CODEPAGE, handle(), handle(),
                355, 364, 121, 18, WS_VISIBLE | SS_TEXT | DT_LEFT | DT_MNEMONIC | DT_BOTTOM ),

  cmb_font    ( ID_CMB_FONT    , handle(), handle(),   8, 105, 341, 300 ),
  cmb_size    ( ID_CMB_SIZE    , handle(), handle(), 355, 205, 121, 200 ),
  cmb_codepage( ID_CMB_CODEPAGE, handle(), handle(), 355, 162, 121, 200 ),

  lbl_status  ( ID_STA_STATUS  , handle(), handle(),   8,   8, 469, 22  ),
  fnt_grid    ( ID_GRD_FONT    , handle(), handle(),   8,  36           ),
  fnt_glyph   ( ID_GLP_FONT    , handle(), handle(), 355, 214, 121, 120 ),

  btn_about   ( WC_BUTTON, ID_BTN_ABOUT, MSG_BTN_ABOUT, handle(), handle(), 354,  36, 122, 32,
                WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON | BS_NOPOINTERFOCUS ),
  btn_specs   ( WC_BUTTON, ID_BTN_SPECS, MSG_BTN_SPECS, handle(), handle(), 354,  70, 122, 32,
                WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON | BS_NOPOINTERFOCUS ),
  btn_clear   ( WC_BUTTON, ID_BTN_CLEAR, MSG_BTN_CLEAR, handle(), handle(), 354, 104, 122, 32,
                WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON | BS_NOPOINTERFOCUS | WS_GROUP ),
  win_specs   ( 0 )
{
  lbl_status  .set_font( "9.WarpSans" );
  lbl_font    .set_font( "9.WarpSans" );
  lbl_size    .set_font( "9.WarpSans" );
  cmb_font    .set_font( "9.WarpSans" );
  cmb_size    .set_font( "9.WarpSans" );
  lbl_codepage.set_font( "9.WarpSans" );
  cmb_codepage.set_font( "9.WarpSans" );
  btn_clear   .set_font( "9.WarpSans" );
  btn_about   .set_font( "9.WarpSans" );
  btn_specs   .set_font( "9.WarpSans" );

  ULONG i;

  // Restore the last window position

  SHAPE old_rect = shape();

  if( !load( INI_APPS_POS ))
    center_at();

  SHAPE new_rect = shape();
  new_rect.cx  = old_rect.cx;
  new_rect.cy  = old_rect.cy;
  reshape( new_rect );

  set_state( MSG_LOADING );

  // Load context menu

  mnu_handle = WinLoadMenu( HWND_DESKTOP, NULLHANDLE, ID_MNU_MAIN );

  if( !mnu_handle )
    PM_THROW_GUIERROR();

  WinSendMsg( mnu_handle, MM_SETITEMTEXT, MPFROMLONG(ID_ITM_ADD  ), MPFROMP(MSG_MNU_ADD  ));
  WinSendMsg( mnu_handle, MM_SETITEMTEXT, MPFROMLONG(ID_ITM_CLEAR), MPFROMP(MSG_MNU_CLEAR));
  WinSendMsg( mnu_handle, MM_SETITEMTEXT, MPFROMLONG(ID_ITM_SPECS), MPFROMP(MSG_MNU_SPECS));
  WinSendMsg( mnu_handle, MM_SETITEMTEXT, MPFROMLONG(ID_ITM_RTF  ), MPFROMP(MSG_MNU_RTF  ));
  WinSendMsg( mnu_handle, MM_SETITEMTEXT, MPFROMLONG(ID_ITM_CLOSE), MPFROMP(MSG_MNU_CLOSE));

  clb_flags  = PMProfile::get_ulong( INI_CLB_FLAGS, CMapClipboard::ascii |
                                                    CMapClipboard::rtf );
  clb_flags |= CMapClipboard::ascii;

  WinCheckMenuItem( mnu_handle, ID_ITM_RTF, clb_flags & CMapClipboard::rtf );

  // Prepare codepages list

  ULONG  cp_count  = WinQueryCpList( PMGUI::hab(), 0, NULL );
  ULONG  cp_system = WinQueryCp( PMGUI::hmq());
  ULONG* cp_list   = new ULONG[cp_count];
  char   cp_name[64];

  cp_count = WinQueryCpList( PMGUI::hab(), cp_count, cp_list );

  while( cp_count-- )
  {
    if( cp_list[cp_count] == cp_system ) {
      sprintf( cp_name, "%u (%s)", cp_system, MSG_OTH_SYSTEM );
    } else {
      ltoa( cp_list[cp_count], cp_name, 10 );
    }
    cmb_codepage.insert( cp_name, cp_list[cp_count] );
  }

  // Select active codepage

  PMProfile::get_chars( INI_SEL_CPAGE, cp_name, sizeof(cp_name), "" );

  if( !*cp_name || cmb_codepage.search( cp_name ) == LIT_NONE ) {
    sprintf( cp_name, "%u (%s)", cp_system, MSG_OTH_SYSTEM );
  }

  cmb_codepage.select( cmb_codepage.search( cp_name ));
  delete cp_list;

  // Prepare fonts list

  FONTMETRICS* fnMetrics = CMapFonts::metrics();
  ULONG        fnCount   = CMapFonts::metrics_count();
  char*        fnName;
  char         fnSave[FACESIZE];
  char         fnSize[FACESIZE];

  for( i = 0; i < fnCount; i++ )
  {
    fnName = fnMetrics[i].szFacename;

    if( fnName[0] != '@' && cmb_font.search( fnName ) == LIT_NONE )
        cmb_font.insert( fnName, 0, LIT_SORTASCENDING );
  }

  for( i = 0; i < fnCount; i++ )
  {
    fnName = fnMetrics[i].szFacename;

    if( fnName[0] == '@' && cmb_font.search( fnName ) == LIT_NONE )
        cmb_font.insert( fnName, 0, LIT_END );
  }

  // Restore saved font and character code

  PMProfile::get_chars( INI_SEL_FONT, fnSave, sizeof(fnSave), "" );
  PMProfile::get_chars( INI_SEL_SIZE, fnSize, sizeof(fnSize), "" );
  i = cmb_font.search( fnSave );

  if( *fnSave && i != LIT_NONE ) {
    cmb_font.select( i );
    cmb_size.select( cmb_size.search( fnSize ));
  } else {
    cmb_font.select( 0 );
  }

  fnt_grid.select( PMProfile::get_short( INI_SEL_CHAR, 0 ));
  attach_accelerators( ID_ACL_MAIN );
}

/*--------------------------------------------------
 * Destructs the application window
 *--------------------------------------------------*/
CMapApplication::~CMapApplication()
{
  if( WinIsWindow( PMGUI::hab(), handle()))
    if( !WinDestroyWindow( handle()))
      PM_THROW_GUIERROR();
}

/*--------------------------------------------------
 * Sets the input focus to the window
 *--------------------------------------------------*/
CMapApplication& CMapApplication::set_focus()
{
  cmb_font.set_focus();
  return *this;
}

/*--------------------------------------------------
 * Displays a state of the program message
 *--------------------------------------------------*/
CMapApplication& CMapApplication::set_state( const char* state, ... )
{
  char    message[256];
  va_list args;

  va_start( args, state );
  vsprintf( message, state, args  );

  lbl_status.set_caption( message );
  return *this;
}

/*--------------------------------------------------
 * Select specified font
 *--------------------------------------------------*/
CMapApplication& CMapApplication::select_font( int pos )
{
  char font_name[FACESIZE],
       font_size[20];

  ULONG        font_count = CMapFonts::metrics_count();
  FONTMETRICS* font_list  = CMapFonts::metrics();

  cmb_font.string( pos, font_name, FACESIZE );
  cmb_size.clear ();

  while( font_count-- )
  {
    FONTMETRICS* font = font_list + font_count;

    if( strcmp( font_name, font->szFacename ) == 0 )
    {
      if( font->fsDefn & FM_DEFN_OUTLINE )
        sprintf( font_size, "%02ux%02u (%s)", font->lAveCharWidth,
                                              font->lMaxBaselineExt, MSG_OTH_OUTLINE );
      else
        sprintf( font_size, "%02ux%02u", font->lAveCharWidth, font->lMaxBaselineExt );

      cmb_size.insert( font_size, (ULONG)font );
    }
  }

  cmb_size.select(0);
  return *this;
}

/*--------------------------------------------------
 * Select specified font size
 *--------------------------------------------------*/
CMapApplication& CMapApplication::select_size( int pos )
{
  FATTRS       fattrs;
  FONTMETRICS* metrics = (FONTMETRICS*)cmb_size.item_handle( pos );

  memset( &fattrs, 0, sizeof(fattrs));
  strcpy( fattrs.szFacename, metrics->szFacename );

  fattrs.usRecordLength  = sizeof(FATTRS);
  fattrs.lMatch          = metrics->lMatch;
  fattrs.idRegistry      = metrics->idRegistry;
  fattrs.usCodePage      = cmb_codepage.item_handle( cmb_codepage.selected());
  fattrs.lMaxBaselineExt = metrics->lMaxBaselineExt;
  fattrs.lAveCharWidth   = metrics->lAveCharWidth;

  if( win_specs )
      win_specs->select_font( *metrics );

  fnt_glyph.select_font( fattrs );
  fnt_grid .select_font( fattrs );

  return *this;
}

/*--------------------------------------------------
 * Dispatch system native event
 *-------------------------------------------------*/
MRESULT CMapApplication::dispatch( ULONG msg, MPARAM mp1, MPARAM mp2 )
{
  switch( msg )
  {
    case WM_DESTROY:
    {
      char selected[FACESIZE];

      PMProfile::put_chars( INI_SEL_FONT,
        cmb_font.string( cmb_font.selected(), selected, sizeof(selected)));
      PMProfile::put_chars( INI_SEL_CPAGE,
        cmb_codepage.string( cmb_codepage.selected(), selected, sizeof(selected)));
      PMProfile::put_chars( INI_SEL_SIZE,
        cmb_size.string( cmb_size.selected(), selected, sizeof(selected)));

      PMProfile::put_short( INI_SEL_CHAR, fnt_grid.selected());
      save( INI_APPS_POS );
      break;
    }

    case WM_ACTIVATE:
      if( mp1 ) btn_clear.enable( !CMapClipboard::is_empty());
      break;

    case WM_CONTEXTMENU:
    {
      LONG x = SHORT2FROMMP(mp2) ? shape().cx/2 : SHORT1FROMMP(mp1);
      LONG y = SHORT2FROMMP(mp2) ? shape().cy/2 : SHORT2FROMMP(mp1);

      WinPopupMenu( handle(), handle(), mnu_handle, x, y, ID_ITM_ADD,
                    PU_POSITIONONITEM | PU_HCONSTRAIN   | PU_VCONSTRAIN |
                    PU_MOUSEBUTTON1   | PU_MOUSEBUTTON2 | PU_KEYBOARD   );
      break;
    }

    // Grid window notification messages

    case CMGR_RENDERING:
      set_state( mp1 ? MSG_RENDERED : MSG_RENDERING );
      return 0;

    case CMGR_SELECT:
      fnt_glyph.select( (char)(long)mp1 );
      set_state( MSG_SELECTED, mp1, mp1 );
      return 0;

    case CMGR_CLICKED:
    {
      char  string[2] = { (char)(long)mp1, 0 };
      char  font[FACESIZE];

      CMapClipboard::push_back( string,
                                cmb_font.string( cmb_font.selected(), font, FACESIZE ),
                                cmb_codepage.item_handle( cmb_codepage.selected()),
                                clb_flags );

      btn_clear.enable( !CMapClipboard::is_empty());
      set_state( MSG_ADDED, mp1, mp1 );
      return 0;
    }

    // Specification window notification messages

    case CMFS_DESTROY:
      win_specs = 0;
      return 0;

    // Controls notification messages

    case WM_CONTROL:
    {
      if( SHORT2FROMMP(mp1) == LN_SELECT )
      {
        switch( SHORT1FROMMP(mp1))
        {
          case ID_CMB_FONT:
            if( cmb_font.selected() != LIT_NONE )
                select_font( cmb_font.selected());

            break;

          case ID_CMB_SIZE:
          case ID_CMB_CODEPAGE:
            if( cmb_size.selected() != LIT_NONE )
                select_size( cmb_size.selected());

            break;
        }
      }
      return 0;
    }

    // Buttons and menus

    case WM_COMMAND:
      switch ( SHORT1FROMMP(mp1))
      {
        case ID_ITM_ADD:
          WinSendMsg( handle(), CMGR_CLICKED, MPFROMSHORT( fnt_grid.selected()), 0 );
          return 0;

        case ID_ITM_CLEAR:
        case ID_BTN_CLEAR:
          CMapClipboard::clear();
          btn_clear.enable( !CMapClipboard::is_empty());
          set_state( MSG_CLEARED );
          return 0;

        case ID_BTN_ABOUT:
          new CMapAbout( ID_WIN_ABOUT, this );
          return 0;

        case ID_ITM_SPECS:
        case ID_BTN_SPECS:
          if( !win_specs )
          {
            win_specs = new CMapSpecs( ID_WIN_SPECS, this );
            int pos = cmb_size.selected();

            if( pos != LIT_NONE )
              win_specs->select_font( *((FONTMETRICS*)cmb_size.item_handle( pos )));
          }
          else
            win_specs->destroy();

          return 0;

        case ID_ITM_RTF:
          clb_flags ^= CMapClipboard::rtf;
          WinCheckMenuItem( mnu_handle, ID_ITM_RTF, clb_flags & CMapClipboard::rtf );
          PMProfile::put_ulong( INI_CLB_FLAGS, clb_flags );
          return 0;

        case ID_ITM_CLOSE:
        case DID_CANCEL:
          WinPostQueueMsg( PMGUI::hmq(), WM_QUIT, 0, 0 );
          return 0;
      }
  }

  return inherited::dispatch( msg, mp1, mp2 );
}

