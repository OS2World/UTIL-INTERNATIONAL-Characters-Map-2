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
#define  INCL_ERRORS
#include <os2.h>
#include <stdlib.h>

#include "pm_gui.h"
#include "pm_error.h"

HAB PMGUI::gui_hab[PM_MAX_THREADS];
HMQ PMGUI::gui_hmq[PM_MAX_THREADS];

/*--------------------------------------------------
 * Initializes GUI
 *--------------------------------------------------*/
PMGUI::PMGUI()
{
  TID cur_tid = tid();

  if( cur_tid > PM_MAX_THREADS )
    PM_THROW_ERROR( 0, "Too many threads" );

  gui_hab[cur_tid] = NULLHANDLE;
  gui_hmq[cur_tid] = NULLHANDLE;

  if( !(gui_hab[cur_tid] = WinInitialize(0)))
    PM_THROW_ERROR( 0, "Unexpected error in WinInitialize()" );

  if( !(gui_hmq[cur_tid] = WinCreateMsgQueue( gui_hab[cur_tid], 0 )))
    PM_THROW_GUIERROR();

  gui_wrapped = FALSE;
}

/*--------------------------------------------------
 * Wraps initialized GUI
 *--------------------------------------------------*/
PMGUI::PMGUI( HAB hab )
{
  TID cur_tid = tid();

  if( cur_tid > PM_MAX_THREADS )
    PM_THROW_ERROR( 0, "Too many threads" );

  gui_hab[cur_tid] = hab;
  gui_hmq[cur_tid] = HMQ_CURRENT;
  gui_wrapped      = TRUE;
}

/*--------------------------------------------------
 * Terminates GUI
 *--------------------------------------------------*/
PMGUI::~PMGUI()
{
  TID cur_tid = tid();

  if( !gui_wrapped )
  {
    if( gui_hmq[cur_tid] ) WinDestroyMsgQueue( gui_hmq[cur_tid] );
    if( gui_hab[cur_tid] ) WinTerminate      ( gui_hab[cur_tid] );

    gui_hab[cur_tid] = NULLHANDLE;
    gui_hmq[cur_tid] = NULLHANDLE;
  }
}

/*--------------------------------------------------
 * Returns the current thread identifier
 *--------------------------------------------------*/
TID PMGUI::tid()
{
  PTIB ptib;
  DosGetInfoBlocks( &ptib, NULL );
  return ptib->tib_ptib2->tib2_ultid;
}

/*--------------------------------------------------
 * Returns the current process identifier
 *--------------------------------------------------*/
PID PMGUI::pid()
{
  PPIB ppib;
  DosGetInfoBlocks( NULL, &ppib );
  return ppib->pib_ulpid;
}

/*--------------------------------------------------
 * Returns anchor block handle for current thread
 *--------------------------------------------------*/
HAB PMGUI::hab()
{
  return gui_hab[tid()];
}

/*--------------------------------------------------
 * Returns message queue handle for current thread
 *--------------------------------------------------*/
HMQ PMGUI::hmq()
{
  return gui_hmq[tid()];
}

/*--------------------------------------------------
 * Dispatch messages
 *--------------------------------------------------*/
void PMGUI::run()
{
  QMSG qms;
  HAB  cur_hab = hab();

  while( WinGetMsg( cur_hab, &qms, 0, 0, 0 ))
    WinDispatchMsg( cur_hab, &qms );
}
