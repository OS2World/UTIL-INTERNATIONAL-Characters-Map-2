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

#include "pm_thread.h"
#include "pm_gui.h"
#include "pm_error.h"

/*--------------------------------------------------
 * Constructor
 *-------------------------------------------------*/
PMThread::PMThread()

: run_alive( FALSE ),
  run_id   ( -1    )
{}

/*--------------------------------------------------
 * Thread launch routine
 *-------------------------------------------------*/
void PMThread::launch( PMThread* runable )
{
  PMGUI initialize;

  runable->run_alive = TRUE;
  runable->run();
  runable->run_alive = FALSE;
  runable->run_id    = -1;
}

/*--------------------------------------------------
 * Start the thread
 *-------------------------------------------------*/
PMThread& PMThread::start()
{
  if( !is_alive())
    run_id = _beginthread( (void(*_Optlink)(void*))launch, NULL, 64*1024, this );

  if( run_id == -1 )
    PM_THROW_CLBERROR();

  return *this;
}

/*--------------------------------------------------
 * Stops the thread
 *-------------------------------------------------*/
PMThread& PMThread::stop()
{
  if( is_alive() && tid() != -1 )
  {
    APIRET rc = DosKillThread( tid());

    if( rc != NO_ERROR && rc != ERROR_INVALID_THREADID )
      PM_THROW_OS2ERROR(rc);

    run_alive = FALSE;
    run_id    = -1;
  }
  return *this;
}

/*--------------------------------------------------
 * Wait the thread
 *-------------------------------------------------*/
PMThread& PMThread::join()
{
  if( run_id != -1 )
    DosWaitThread( &run_id, DCWW_WAIT );

  return *this;
}

/*--------------------------------------------------
 * Suspend the thread
 *-------------------------------------------------*/
PMThread& PMThread::suspend()
{
  if( run_id != -1 )
  {
    APIRET rc = DosSuspendThread( run_id );

    if( rc != NO_ERROR )
      PM_THROW_OS2ERROR(rc);
  }
  return *this;
}

/*--------------------------------------------------
 * Resume the thread
 *-------------------------------------------------*/
PMThread& PMThread::resume()
{
  if( run_id != -1 )
  {
    APIRET rc = DosResumeThread( run_id );

    if( rc != NO_ERROR && rc != ERROR_NOT_FROZEN )
      PM_THROW_OS2ERROR(rc);
  }
  return *this;
}

