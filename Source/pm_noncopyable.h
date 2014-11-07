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
#ifndef _PM_NONCOPYABLE_H
#define _PM_NONCOPYABLE_H
/**@#+*/

/**
 * Prohibit copy construction and copy assignment.
 *
 * Class noncopyable is a base class. Derive your own class from
 * noncopyable when you want to prohibit copy construction and
 * copy assignment.
 * <p>
 * Some objects, particularly those which hold complex resources like
 * files or network connections, have no sensible copy semantics.
 * Sometimes there are possible copy semantics, but these would be
 * of very limited usefulness and be very difficult to implement
 * correctly.  Sometimes you're implementing a class that doesn't
 * need to be copied just yet and you don't want to take the time
 * to write the appropriate functions. Deriving from noncopyable
 * will prevent the otherwise implicitly-generated functions
 * (which don't have the proper semantics) from becoming a
 * trap for other programmers.
 * <p>
 * The traditional way to deal with these is to declare a private
 * copy constructor and copy assignment, and then document why this
 * is done. But deriving from noncopyable is simpler and
 * clearer, and doesn't require additional documentation.
 *
 * @author Dmitry A Steklenev
 */

class PMNonCopyable
{
  public:
    PMNonCopyable() {}
  private:
    PMNonCopyable( const PMNonCopyable& );
    PMNonCopyable& operator=( const PMNonCopyable& );
};

/**@#-*/
#endif
