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

#define PCK_VERSION      "2.0.5"

#define INI_APPS_POS     "Pos"
#define INI_ABOUT_POS    "About Pos"
#define INI_SPECS_POS    "Specs Pos"
#define INI_CLB_FLAGS    "Clipboard formats"
#define INI_SEL_FONT     "Font"
#define INI_SEL_CPAGE    "CodePage"
#define INI_SEL_SIZE     "Size"
#define INI_SEL_CHAR     "Character"

#define ID_WIN_MAIN       100
#define ID_STA_STATUS     101
#define ID_STA_FONT       102
#define ID_STA_SIZE       103
#define ID_CMB_FONT       104
#define ID_CMB_SIZE       105
#define ID_STA_CODEPAGE   106
#define ID_CMB_CODEPAGE   107
#define ID_GRD_FONT       108
#define ID_GLP_FONT       109
#define ID_BTN_CLEAR      110
#define ID_BTN_ABOUT      111
#define ID_BTN_SPECS      112
#define ID_BTN_EXIT       113
#define ID_ACL_MAIN       114

#define ID_MNU_MAIN       200
#define ID_ITM_ADD        201
#define ID_ITM_CLEAR      202
#define ID_ITM_SPECS      203
#define ID_ITM_RTF        204
#define ID_ITM_CLOSE      205

#define ID_WIN_ABOUT      100
#define ID_STA_APPNAME    101
#define ID_STA_VERSION    102
#define ID_STA_AUTHOR     103
#define ID_STA_SEP        104

#define ID_WIN_SPECS      100
#define ID_CNT_SPECS      101

#define MSG_LBL_FONT        PMMsg::get(1   )
#define MSG_LBL_SIZE        PMMsg::get(2   )
#define MSG_LBL_CODEPAGE    PMMsg::get(3   )
#define MSG_BTN_CLEAR       PMMsg::get(4   )
#define MSG_BTN_ABOUT       PMMsg::get(5   )
#define MSG_BTN_SPECS       PMMsg::get(6   )

#define MSG_LOADING         PMMsg::get(20  )
#define MSG_RENDERING       PMMsg::get(21  )
#define MSG_RENDERED        PMMsg::get(22  )
#define MSG_SELECTED        PMMsg::get(23  )
#define MSG_ADDED           PMMsg::get(24  )
#define MSG_CLEARED         PMMsg::get(25  )

#define MSG_OTH_OUTLINE     PMMsg::get(40  )
#define MSG_OTH_SYSTEM      PMMsg::get(41  )

#define MSG_MNU_ADD         PMMsg::get(50  )
#define MSG_MNU_CLEAR       PMMsg::get(51  )
#define MSG_MNU_SPECS       PMMsg::get(52  )
#define MSG_MNU_RTF         PMMsg::get(53  )
#define MSG_MNU_CLOSE       PMMsg::get(54  )

#define MSG_SPC_FONT        PMMsg::get(60  )
#define MSG_SPC_FAMILY      PMMsg::get(61  )
#define MSG_SPC_CODEPAGE    PMMsg::get(62  )
#define MSG_SPC_ALL         PMMsg::get(63  )
#define MSG_SPC_NONE        PMMsg::get(64  )
#define MSG_SPC_WEIGHT      PMMsg::get(65  )
#define MSG_WGT_ULTRALIGHT  PMMsg::get(66  )
#define MSG_WGT_EXTRALIGHT  PMMsg::get(67  )
#define MSG_WGT_LIGHT       PMMsg::get(68  )
#define MSG_WGT_SEMILIGHT   PMMsg::get(69  )
#define MSG_WGT_MEDIUM      PMMsg::get(70  )
#define MSG_WGT_SEMIBOLD    PMMsg::get(71  )
#define MSG_WGT_BOLD        PMMsg::get(72  )
#define MSG_WGT_EXTRABOLD   PMMsg::get(73  )
#define MSG_WGT_ULTRABOLD   PMMsg::get(74  )
#define MSG_WGT_UNKNOWN     PMMsg::get(75  )
#define MSG_SPC_WIDTH       PMMsg::get(76  )
#define MSG_WDT_ULTRACOND   PMMsg::get(77  )
#define MSG_WDT_EXTRACOND   PMMsg::get(78  )
#define MSG_WDT_CONDENSED   PMMsg::get(79  )
#define MSG_WDT_SEMICOND    PMMsg::get(80  )
#define MSG_WDT_SEMIEXPAND  PMMsg::get(81  )
#define MSG_WDT_MEDIUM      PMMsg::get(82  )
#define MSG_WDT_EXPANDED    PMMsg::get(83  )
#define MSG_WDT_EXTRAEXPAND PMMsg::get(84  )
#define MSG_WDT_ULTRAEXPAND PMMsg::get(85  )
#define MSG_WDT_UNKNOWN     PMMsg::get(86  )
#define MSG_SPC_MINPOINTS   PMMsg::get(87  )
#define MSG_SPC_NOMPOINTS   PMMsg::get(88  )
#define MSG_SPC_MAXPOINTS   PMMsg::get(89  )
#define MSG_SPC_RESOLUTION  PMMsg::get(90  )

#define SPC_COUNT 9

// Grid window notification messages

#define CMGR_RENDERING  (WM_USER+1) // This message occurs when the font grid rendering
                                    // process is started or finished.
                                    //
                                    // mp1: FALSE - The rendering is started.
                                    //      TRUE  - The rendering is finished.

#define CMGR_SELECT     (WM_USER+2) // This message occurs if the user selected a
                                    // character. Character code is placed in mp1.

#define CMGR_CLICKED    (WM_USER+3) // This message occurs if the user pressing the
                                    // ENTER key or the double-clicking on the grid
                                    // window. Selected character code is placed in mp1.

// Font specifications window notification messages

#define CMFS_INIT       (WM_USER+4)
#define CMFS_DESTROY    (WM_USER+5)

