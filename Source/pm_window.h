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
#ifndef _PM_WINDOW_H
#define _PM_WINDOW_H

#include "pm_error.h"
#include "pm_noncopyable.h"
/**@#+*/

/** Window rectangular shape. */
struct SHAPE {
  LONG x, y, cx, cy;
};

/**
 * Base window class.
 *
 * The PMWindow class is the base window class and provides
 * behavior common to all windows. Although this class contains
 * behavior requiring the existence of a presentation window,
 * you must construct the presentation window itself using a
 * derived class.
 * <p>
 * Note: Although you can construct objects of this class
 * directly, you do not generally do so.
 *
 * @author Dmitry A Steklenev
 */

class PMWindow : public PMNonCopyable
{
  public:
    /** Constructs the object from presentation window handle. */
    PMWindow( HWND handle = 0 );

    /** Constructs the presenttion window. */
    PMWindow( const char* class_name ,
              SHORT       id         ,
              const char* caption    ,
              HWND        parent     ,
              HWND        owner      ,
              long        x          ,
              long        y          ,
              long        cx         ,
              long        cy         ,
              ULONG       style      ,
              PVOID       class_data = NULL );

    /** Constructs the presenttion window. */
    PMWindow( SHORT       id         ,
              const char* caption    ,
              HWND        parent     ,
              HWND        owner      ,
              long        x          ,
              long        y          ,
              long        cx         ,
              long        cy         ,
              ULONG       style      );

    /** Destructs the window. */
    virtual ~PMWindow();

    /**
     * Destroys the window.
     *
     * Destroying the associated presentation window
     * for this object.
     */

    virtual PMWindow& destroy();

  public:
    /** Returns the window handle. */
    HWND  handle() const { return win_handle; }
    /** Returns the window identifier. */
    SHORT id    () const { return win_id;     }

    /**
     * Returns the parent window handle.
     *
     * The parent window provides the coordinate system used for
     * positioning a child window. A window whose parent is the desktop window,
     * is called a top-level or overlapped window.
     */

    HWND parent() const;

    /**
     * Returns the owner window handle.
     *
     * Windows that send messages send them to their owner.
     * When an owner window is destroyed, all windows owned by it
     * are also destroyed. The owner window must belong to the
     * current thread.
     */

    HWND owner() const;

    /** Sets the window caption. */
    virtual PMWindow& set_caption( const char* caption );

  public:
    /** Returns the window shape. */
    virtual SHAPE shape() const;
    /** Sets the window shape. */
    virtual PMWindow& reshape( const SHAPE& rect );
    /** Center the window concerning his owner window. */
    virtual PMWindow& center_at();
    /** Makes the window visible. */
    virtual PMWindow& show( BOOL state = TRUE );
    /** Sets the input focus to the window. */
    virtual PMWindow& set_focus();

    /**
     * Has focus.
     *
     * If the window has the input focus, true is returned.
     * Otherwise, false is returned.
     */

    virtual BOOL has_focus() const;

    /** Enables the window to accept keyboard and mouse input. */
    virtual PMWindow& enable( BOOL state = TRUE );
    /** Prevents keyboard and mouse input from being sent to the window. */
    virtual PMWindow& disable();

    /** Sets a new font to be used by the window. */
    virtual PMWindow& set_font( const char* font );
    /** Returns the background color value of the window. */
    virtual LONG background() const;
    /** Returns the foreground color value of the window. */
    virtual LONG foreground() const;

  public:

    /**
     * This method adds a region to a window's update region.
     *
     * The update region is a subregion of a window that is
     * deemed "invalid" or incorrect in visual terms and is
     * in need of redrawing.
     *
     * @exception: GLGUIError
     */

    virtual PMWindow& invalidate( const SHAPE& rect );

    /**
     * This method adds the whole window to a window's update region.
     *
     * The update region is a subregion of a window that is
     * deemed "invalid" or incorrect in visual terms and is
     * in need of redrawing.
     */

    virtual PMWindow& invalidate();

  public:

    /**
     * Capture mouse pointer.
     *
     * If capture is true, pointer events are sent only to this window
     * even if the pointer is outside of this window. If capture is false,
     * the window releases the pointer capture. If you attempt to capture
     * the pointer when another window currently is capturing the pointer,
     * an exception is thrown.
     */

    virtual PMWindow& capture_pointer( BOOL capture = TRUE );

    /**
     * Release mouse pointer.
     *
     * Causes the window to release the pointer capture (pointer capture
     * is set with the function capture_pointer).
     * <p>
     * This function causes mouse events to again be dispatched to the window
     * underneath the mouse pointer.
     */

    virtual PMWindow& release_pointer();

    /**
     * Has pointer captured.
     *
     * If this window is currently capturing pointer events, true is
     * returned. Otherwise, false is returned.
     */

    virtual BOOL has_pointer_captured() const;

    /** Replaces the accelerator keys used by the window. */
    virtual PMWindow& attach_accelerators( SHORT id );

  protected:

    /**
     * Handle events.
     *
     * Attaches the handler to the PMWindow object. The windows's
     * dispatch function is called to process all events sent or posted to the
     * window.
     */

    PMWindow& handle_events();

    /**
     * Stop handling events.
     *
     * Detaches the handler from the PMWindow object. The window's
     * dispatch function is no longer called to process events sent or
     * posted to the window.
     */

    PMWindow& stop_handling_events();

    /**
     * Dispatch system native events.
     *
     * GLWindow calls this function when a system native window
     * event occurs.
     */

    virtual MRESULT dispatch( ULONG msg, MPARAM mp1, MPARAM mp2 );

    /**
     * Dispatch all not catched events.
     *
     * A non-virtual method that provides default implementation for
     * all events that occur on every window.
     */

    MRESULT dispatch_everything( ULONG msg, MPARAM mp1, MPARAM mp2 );

  public:
    /** Draws the 3D border. */
    static void draw_3dborder( HPS ps, const SHAPE& rect, long thickness = 2 );

  private:
    PFNWP  win_default_handler;
    HWND   win_handle;
    SHORT  win_id;
    BOOL   win_wrapper;

    static MRESULT _System win_proc( HWND, ULONG, MPARAM,MPARAM );
};

/**@#-*/
#endif
