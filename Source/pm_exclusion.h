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
#ifndef _PM_EXCLUSION_H
#define _PM_EXCLUSION_H

#include "pm_noncopyable.h"
/**@#+*/

/**
 * Serialize access to resources.
 *
 * A mutual exclusion class PMExclusion protects resources
 * (such as files, data in memory, and peripheral devices) from
 * simultaneous access by several processes. Class also enable threads
 * to serialize their access to resources.
 *
 * None of the functions in this class throws exceptions because
 * an exception probably has been thrown already or is about
 * to be thrown.
 *
 * @author Dmitry A Steklenev
 */

class PMExclusion : public PMNonCopyable
{
  public:
    /** Constructs the exclusion object. */
    PMExclusion();
    /** Destructs the exclusion object. */
   ~PMExclusion();

    /**
     * Request access to resource.
     *
     * Requests ownership of a resource.
     * Blocks the calling thread indefinitely.
     *
     * @return true, if ownership established.
     */

    BOOL request();

    /**
     * Request access to resource with wait timeout.
     *
     * Requests ownership of a resource.
     * Blocks the calling thread.
     *
     * @param mseq this is the maximum amount of time the
     *             user wants to allow the thread to be blocked.
     *
     * @return true, if ownership established.
     */

    BOOL request( unsigned long msec );

    /**
     * Relinquishes ownership of a resource that was requested by
     * <i>request</i>.
     *
     * Only the thread that owns the resource can issue release().
     *
     * @return true, if ownership relinquished.
     */

    BOOL release();

  private:
    HMTX exc_handle;
};

/**@#-*/
#endif
