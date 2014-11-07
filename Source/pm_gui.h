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
#ifndef _PM_GUI_H
#define _PM_GUI_H

#ifndef PM_MAX_THREADS
#define PM_MAX_THREADS 20
#endif

#ifndef RGB_DARKGRAY
#define RGB_DARKGRAY   0x00808080L
#endif
#ifndef RGB_PALEGRAY
#define RGB_PALEGRAY   0x00CCCCCCL
#endif

#include "pm_noncopyable.h"
/**@#+*/

/**
 * GUI initialization and information class.
 *
 * Initialize GUI in current thread. Automatically called
 * from <i>PMThread</i>.
 *
 * @author Dmitry A Steklenev
 */

class PMGUI : public PMNonCopyable
{
  public:
    /** Initializes the GUI facilities for use by an application. */
    PMGUI();
    /** Constructs the object from a existing GUI environment. */
    PMGUI( HAB hab );
    /** Releases all of associated GUI resources */
   ~PMGUI();

    /** Returns the current thread identifier. */
    static TID  tid();
    /** Returns the current process identifier. */
    static PID  pid();
    /** Returns anchor block handle for current thread. */
    static HAB  hab();
    /** Returns message queue handle for current thread. */
    static HMQ  hmq();
    /** Dispatch messages. */
    static void run();

  private:
    BOOL   gui_wrapped;
    static HAB gui_hab[PM_MAX_THREADS];
    static HMQ gui_hmq[PM_MAX_THREADS];
};

/**@#-*/
#endif