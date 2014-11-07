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
#ifndef _PM_ERROR_H
#define _PM_ERROR_H

#include <string.h>
#include <errno.h>
#include <stdio.h>
#include "pm_noncopyable.h"
/**@#+*/

/**
 * Base error class.
 *
 * The PMError class is the base class from which all
 * exception objects thrown in the library are derived.
 * These classes retrieve error information and text that
 * you can subsequently use to create an exception object.
 * <p>
 * None of the functions in this class throws exceptions because
 * an exception probably has been thrown already or is about to be
 * thrown.
 *
 * @author Dmitry A Steklenev
 */

class PMError
{
  public:
    /** Constructs error from current GUI error. */
    PMError( const char* file, const char* func, int line );
    /** Constructs error from specified error id and information. */
    PMError( int code, const char* info,
                       const char* file, const char* func, int line );

    /** Constructs error object from another error. */
    PMError( const PMError& );
    /** Assigns the value of one error object to another. */
    PMError& operator=( const PMError& );
    /** Destructs the error object */
   ~PMError();

    /** Returns source file name. */
    const char* file() const { return err_file; }
    /** Returns the name of the function. */
    const char* func() const { return err_func; }
    /** Returns the line number. */
    int         line() const { return err_line; }
    /** Returns the error message. */
    const char* info() const { return err_info; }
    /** Returns the error id. */
    int         code() const { return err_code; }

    /** Display error information. */
    PMError& display( FILE* out = 0 );

  private:
    char* err_file;
    char* err_func;
    int   err_line;
    char* err_info;
    int   err_code;
};

/**@#-*/
#define PM_ERROR_LOCATION  __FILE__, __FUNCTION__, __LINE__

#define PM_THROW_ERROR( id, info ) \
        throw( PMError( id, info, PM_ERROR_LOCATION ).display( stdout ))

#define PM_THROW_GUIERROR()   throw( PMError( PM_ERROR_LOCATION ).display( stdout ))
#define PM_THROW_CLBERROR()   PM_THROW_ERROR( errno  , strerror( errno ))
#define PM_THROW_OS2ERROR(rc) PM_THROW_ERROR( rc     , NULL )

#endif
