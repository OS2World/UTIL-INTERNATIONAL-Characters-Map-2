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
#ifndef _PM_THREAD_H
#define _PM_THREAD_H

#include "pm_noncopyable.h"
/**@#+*/

/**
 * Thread class.
 *
 * The PMThread class represents threads of execution within
 * the current program. You can use this class to implement
 * multithreaded applications. Objects of this class give you
 * access to all of the tasking APIs of the operating system.
 * In addition, these objects can serve as the anchor for thread-specific
 * information.
 *
 * @author Dmitry A Steklenev
 */

class PMThread : public PMNonCopyable {

  public:

    /**
     * Constructs the thread object.
     *
     * Use this, the default constructor, to create an object of this
     * class without creating an additional thread of execution.
     * You can subsequently start execution of the thread using
     * PMThread::start.
     */

    PMThread();

    /**
     * Obtains the identifier of the thread.
     * @return
     *   A return value of -1 indicates that the thread is
     *   not started.
     */

    TID tid() const;

    /** Determines if this thread is currently active. */
    BOOL is_alive() const;
    /** This function causes the thread to start immediately. */
    virtual PMThread& start();

    /**
     * Waits the thread.
     *
     * Places the current thread into a wait state until thread,
     * represented of the PMThread object, has ended.
     */

    virtual PMThread& join();

    /**
     * Suspends the thread.
     *
     * Suspends the thread's execution, which can be undone later
     * with a call to resume(). Note that suspend() and resume() are
     * not recommended mechanisms for thread control. Resources and
     * conditions provide safer services for managing threads.
     */

    virtual PMThread& suspend();

    /**
     * Resumes the thread.
     *
     * Resumes a thread's execution after it has been suspended by a
     * call to suspend(). Note that suspend() and resume() are not
     * recommended mechanisms for thread control. Resources and
     * conditions provide safer services for managing threads.
     */

    virtual PMThread& resume();

    /**
     * Stops the thread.
     *
     * This function causes the thread to stop immediately, bypassing
     * some normal termination functions. Use it with extreme caution,
     * preferably when the state of the target thread is known.
     */

    virtual PMThread& stop();

  protected:

    /**
     * Runable interface.
     *
     * You must overwrite this metod.
     */

    virtual void run() = 0;

  private:
    /** Thread identifier */ TID  run_id;
    /** Is thread alive   */ BOOL run_alive;

    static void launch( PMThread* );
};

#if !defined(__ccdoc__)
/*--------------------------------------------------
 * Obtains the identifier of the thread
 *-------------------------------------------------*/
inline TID PMThread::tid() const
{
  return run_id;
}

/*--------------------------------------------------
 * Determines if this thread is currently active
 *-------------------------------------------------*/
inline BOOL PMThread::is_alive() const
{
  return run_alive;
}
#endif
/**@#-*/
#endif
