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
#include <stdio.h>

#include "cmap_specs.h"
#include "cmap_ids.h"
#include "pm_messages.h"

/*--------------------------------------------------
 * Constructs the characteristics window
 *--------------------------------------------------*/
CMapSpecs::CMapSpecs( SHORT id, PMWindow* owner )

: inherited  ( id, "Font Specifications", owner->handle(), 0, 0, 300, 440,
               FCF_NOBYTEALIGN  | FCF_TITLEBAR | FCF_SYSMENU         |
               FCF_SIZEBORDER   | FCF_AUTOICON | FCF_CLOSEBUTTON     ),

  cnt_specs  ( WC_CONTAINER, ID_CNT_SPECS, "", handle(), handle(),
               0, 0, 0, 0, WS_VISIBLE | CCS_READONLY | CCS_SINGLESEL )
{
  cnt_specs.set_font( "9.WarpSans" );

  // Restore the last window position and size.

  SHAPE new_rect;

  if( !load( INI_SPECS_POS ))
  {
    new_rect    = shape();
    new_rect.x  = owner->shape().x + owner->shape().cx;
    new_rect.y  = owner->shape().y;
    new_rect.cy = owner->shape().cy;

    reshape( new_rect );
  }

  // Insert fields into container.

  FIELDINFO* field_first =
    (FIELDINFO*)WinSendMsg( cnt_specs.handle(),
                            CM_ALLOCDETAILFIELDINFO, MPFROMLONG(2), 0 );
  if( !field_first )
    PM_THROW_GUIERROR();

  FIELDINFO* field = field_first;

  field->flData    = CFA_STRING | CFA_VCENTER | CFA_LEFT | CFA_HORZSEPARATOR;
  field->offStruct = (USHORT)(ULONG)(&(((metric*)0)->key));

  field = field->pNextFieldInfo;

  field->flData    = CFA_STRING | CFA_VCENTER | CFA_LEFT | CFA_HORZSEPARATOR;
  field->offStruct = (USHORT)(ULONG)(&(((metric*)0)->val));

  FIELDINFOINSERT field_insert;

  field_insert.cb                   = sizeof(field_insert);
  field_insert.pFieldInfoOrder      = (FIELDINFO*)CMA_FIRST;
  field_insert.fInvalidateFieldInfo = TRUE;
  field_insert.cFieldInfoInsert     = 2;

  if( !WinSendMsg( cnt_specs.handle(),
                   CM_INSERTDETAILFIELDINFO, MPFROMP(field_first), MPFROMP(&field_insert)))
  {
    PM_THROW_GUIERROR();
  }

  // Select details view.

  CNRINFO cnrinfo;

  cnrinfo.cb             = sizeof(cnrinfo);
  cnrinfo.pFieldInfoLast = field_first;
  cnrinfo.flWindowAttr   = CV_DETAIL;
  cnrinfo.xVertSplitbar  = 150;

  if( !WinSendMsg( cnt_specs.handle(), CM_SETCNRINFO, MPFROMP(&cnrinfo),
                   MPFROMLONG( CMA_PFIELDINFOLAST | CMA_XVERTSPLITBAR | CMA_FLWINDOWATTR )))
  {
    PM_THROW_GUIERROR();
  }

  // Inserts font specifications records.

  metric* first_record =
    (metric*)WinSendMsg( cnt_specs.handle(), CM_ALLOCRECORD,
                         MPFROMLONG(sizeof(metric)-sizeof(RECORDCORE)),
                         MPFROMLONG(20));
  if( !first_record )
    PM_THROW_GUIERROR();

  metric* record = first_record;

  for( int i = 0; i < SPC_COUNT; i++ )
  {
    record->key = record->key_data;
    record->val = record->val_data;
    record = (metric*)record->core.preccNextRecord;
  }

  RECORDINSERT record_insert;

  record_insert.cb                = sizeof(record_insert);
  record_insert.pRecordOrder      = (RECORDCORE*)CMA_FIRST;
  record_insert.pRecordParent     = 0;
  record_insert.fInvalidateRecord = TRUE;
  record_insert.zOrder            = CMA_TOP;
  record_insert.cRecordsInsert    = SPC_COUNT;

  if( !WinSendMsg( cnt_specs.handle(),
                   CM_INSERTRECORD, MPFROMP(first_record), MPFROMP(&record_insert)))
  {
    PM_THROW_GUIERROR();
  }

  // Needed for correct repainting of a frame controls.
  WinSendMsg( handle(), WM_UPDATEFRAME, MPFROMLONG(FCF_TITLEBAR), 0 );
  show();
}

/*--------------------------------------------------
 * Selects the displayed font
 *-------------------------------------------------*/
CMapSpecs& CMapSpecs::select_font( const FONTMETRICS& font )
{
  metric* record = (metric*)WinSendMsg( cnt_specs.handle(), CM_QUERYRECORD, 0,
                                        MPFROM2SHORT( CMA_FIRST, CMA_ITEMORDER ));
  /* Face name
   *
   * The typeface name that defines the particular font, for example,
   * Times New Roman Bold Italic.
   */

  strncpy( record->key_data, MSG_SPC_FONT, sizeof(metric::key_data));
  strncpy( record->val_data, font.szFacename, sizeof(metric::val_data));
  record = (metric*)record->core.preccNextRecord;

  /* Family name
   *
   * The family name of the font that describes the basic appearance of the font,
   * for example, Times New Roman
   */

  strncpy( record->key_data, MSG_SPC_FAMILY, sizeof(metric::key_data));
  strncpy( record->val_data, font.szFamilyname, sizeof(metric::val_data));
  record = (metric*)record->core.preccNextRecord;

  /* Supported codepages
   *
   * Defines the registered code page supported by the font. A value
   * of 0 implies that the font may be used with any of the OS/2
   * supported code pages.
   *
   * Where a font contains special symbols for which there is no registered
   * code page, then code page 65400 is used.
   */

  strncpy( record->key_data, MSG_SPC_CODEPAGE, sizeof(metric::key_data));

  if( font.usCodePage == 0 )
    sprintf( record->val_data, "%s", MSG_SPC_ALL  );
  else if( font.usCodePage == 65400 )
    sprintf( record->val_data, "%s", MSG_SPC_NONE );
  else
    sprintf( record->val_data, "%u", font.usCodePage );

  record = (metric*)record->core.preccNextRecord;

  /* Weight class
   *
   * Indicates the visual weight (thickness of strokes) of the characters
   * in the font.
   */

  strncpy( record->key_data, MSG_SPC_WEIGHT, sizeof(metric::key_data));
  switch ( font.usWeightClass )
  {
    case 1: strcpy( record->val_data, MSG_WGT_ULTRALIGHT ); break;
    case 2: strcpy( record->val_data, MSG_WGT_EXTRALIGHT ); break;
    case 3: strcpy( record->val_data, MSG_WGT_LIGHT      ); break;
    case 4: strcpy( record->val_data, MSG_WGT_SEMILIGHT  ); break;
    case 5: strcpy( record->val_data, MSG_WGT_MEDIUM     ); break;
    case 6: strcpy( record->val_data, MSG_WGT_SEMIBOLD   ); break;
    case 7: strcpy( record->val_data, MSG_WGT_BOLD       ); break;
    case 8: strcpy( record->val_data, MSG_WGT_EXTRABOLD  ); break;
    case 9: strcpy( record->val_data, MSG_WGT_ULTRABOLD  ); break;
    default:
      sprintf( record->val_data, "%s (%n)", MSG_WGT_UNKNOWN, font.usWeightClass );
  }

  record = (metric*)record->core.preccNextRecord;

  /* Width class
   *
   * Indicates the relative aspect ratio of the characters of the font in relation to the normal aspect ratio
   * for this type of font.
   */

  strncpy( record->key_data, MSG_SPC_WIDTH, sizeof(metric::key_data));

  switch( font.usWidthClass )
  {
    case 1: strcpy( record->val_data, MSG_WDT_ULTRACOND   ); break;
    case 2: strcpy( record->val_data, MSG_WDT_EXTRACOND   ); break;
    case 3: strcpy( record->val_data, MSG_WDT_CONDENSED   ); break;
    case 4: strcpy( record->val_data, MSG_WDT_SEMICOND    ); break;
    case 5: strcpy( record->val_data, MSG_WDT_MEDIUM      ); break;
    case 6: strcpy( record->val_data, MSG_WDT_SEMIEXPAND  ); break;
    case 7: strcpy( record->val_data, MSG_WDT_EXPANDED    ); break;
    case 8: strcpy( record->val_data, MSG_WDT_EXTRAEXPAND ); break;
    case 9: strcpy( record->val_data, MSG_WDT_ULTRAEXPAND ); break;
    default:
      sprintf( record->val_data, "%s (%n)", MSG_WDT_UNKNOWN, font.usWidthClass );
  }

  record = (metric*)record->core.preccNextRecord;

  /* Minimum point size.
   *
   * For a bit-map font, this field does not apply. For an outline font,
   * this field contains the minimum height intended by the font designer.
   * Note that this is not a restriction of the size at which the font
   * can be used.
   */

  strncpy( record->key_data, MSG_SPC_MINPOINTS, sizeof(metric::key_data));
  sprintf( record->val_data, "%u", font.sMinimumPointSize/10 );
  record = (metric*)record->core.preccNextRecord;

  /* Nominal point size.
   *
   * For a bit-map font, this field contains the height of the font.
   *
   * For an outline font, this field contains the height intended by
   * the font designer. This is not the only size at which the font can
   * be used.
   */

  strncpy( record->key_data, MSG_SPC_NOMPOINTS, sizeof(metric::key_data));
  sprintf( record->val_data, "%u", font.sNominalPointSize/10 );
  record = (metric*)record->core.preccNextRecord;

  /* Maximum point size.
   *
   * For a bit-map font, this field does not apply.
   *
   * For an outline font, this field contains the maximum height
   * intended by the font designer. Note that this is not a
   * restriction of the size at which the font can be used.
   */

  strncpy( record->key_data, MSG_SPC_MAXPOINTS, sizeof(metric::key_data));
  sprintf( record->val_data, "%u", font.sMaximumPointSize/10  );
  record = (metric*)record->core.preccNextRecord;

  /* Device resolution.
   *
   * For bit-map fonts this is the resolution in of the intended
   * target device, measured in pels per inch.
   *
   * For outline fonts this is the number of notional units of the
   * Em square, measured in notional units per Em. (Notional units
   * are the units in which the outline is defined.)
   */

  strncpy( record->key_data, MSG_SPC_RESOLUTION, sizeof(metric::key_data));
  sprintf( record->val_data, "%u x %u", font.sXDeviceRes, font.sYDeviceRes );
  record = (metric*)record->core.preccNextRecord;

  WinSendMsg( cnt_specs.handle(), CM_INVALIDATEDETAILFIELDINFO, 0, 0 );
  return *this;
}

/*--------------------------------------------------
 * Dispatch system native event
 *-------------------------------------------------*/
MRESULT CMapSpecs::dispatch( ULONG msg, MPARAM mp1, MPARAM mp2 )
{
  switch( msg )
  {
    case WM_CLOSE:
      destroy();
      return 0;

    case WM_DESTROY:
      WinSendMsg( owner(), CMFS_DESTROY, 0, 0 );
      save( INI_SPECS_POS  );
      delete this;
      return 0;

    case WM_COMMAND:
    {
      switch ( SHORT1FROMMP(mp1))
      {
        case DID_OK:
        case DID_CANCEL:
          WinPostMsg( handle(), WM_CLOSE, 0, 0 );
          return 0;
      }
    }

    case WM_PAINT:
    {
      inherited::dispatch( msg, mp1, mp2 );

      HPS ps = WinGetPS( handle());
      draw_3dborder( ps, client_shape());
      WinReleasePS(ps);
      return 0;
    }

    case WM_FORMATFRAME:
    {
      SHAPE rect = client_shape();

      rect.x  += 2;
      rect.cx -= 3;
      rect.y  += 1;
      rect.cy -= 3;

      cnt_specs.reshape( rect );
      break;
    }

    case WM_ERASEBACKGROUND:
      return 0;
  }

  return inherited::dispatch( msg, mp1, mp2 );
}

