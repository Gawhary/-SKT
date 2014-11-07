/*mouseactions.h -
  version 2.1, January, 2012

  Copyright (C) 2012 LudiqueLabs

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications,
and to alter it and redistribute it freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not claim that you wrote the
       original software. If you use this software in a product, an acknowledgment in the product
       documentation would be appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be misrepresented
       as being the original software.

    3. This notice may not be removed or altered from any source distribution.*/
#ifndef MOUSEACTIONS_H
#define MOUSEACTIONS_H

#include <stdlib.h>
#include <ctime>
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <TuioServer.h>
#include <TuioCursor.h>
#include <TuioTime.h>
#include <TuioClient.h>
#include <tinyxml.h>
#include <tinystr.h>
#include <iostream>
#ifndef WIN32
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#else
#include <Windows.h>
#endif

using namespace std;
class mouseactions
{
public:
    mouseactions();
    virtual ~mouseactions();
    #ifndef WIN32
        Display *display;
        bool mouseDwn;
        XEvent event;
    #endif
    void mouseDown()
    {
#ifndef WIN32
        if(!mouseDwn){

        if(display == NULL)
        {
            cout << "Could not open display..." << endl;
        }

        event.type = ButtonPress;
        event.xbutton.button = 1;
        event.xbutton.same_screen = True;
        event.xbutton.state = Button1Mask;
        //event.xbutton.window=RootWindow(display, DefaultScreen(display));
        XQueryPointer(display, RootWindow(display, DefaultScreen(display)), &event.xbutton.root, &event.xbutton.window, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);

        event.xbutton.subwindow = event.xbutton.window;

        while(event.xbutton.subwindow)
        {
            event.xbutton.window = event.xbutton.subwindow;

            XQueryPointer(display, event.xbutton.window, &event.xbutton.root, &event.xbutton.subwindow, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);
        }
        XSendEvent(display, PointerWindow, True, ButtonPressMask, &event);
        //XSendEvent(display, PointerWindow, False, Button1MotionMask, &event);
        cout << "DOWN" << endl;
        mouseDwn=true;
        XFlush(display);
        }
        else{
        event.type = MotionNotify;
        event.xbutton.state = Button1MotionMask;
        //event.xbutton.window=RootWindow(display, DefaultScreen(display));
        XQueryPointer(display, RootWindow(display, DefaultScreen(display)), &event.xbutton.root, &event.xbutton.window, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);

        event.xbutton.subwindow = event.xbutton.window;

        while(event.xbutton.subwindow)
        {
            event.xbutton.window = event.xbutton.subwindow;

            XQueryPointer(display, event.xbutton.window, &event.xbutton.root, &event.xbutton.subwindow, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);
        }
        XSendEvent(display, PointerWindow, True, Button1MotionMask, &event);
        cout << "DOWN UPDATE" << endl;
        XFlush(display);
        }
#else
        INPUT iClick[1];
        ZeroMemory(iClick, sizeof(iClick));
        iClick[0].type = INPUT_MOUSE;
        iClick[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

        unsigned int x=SendInput(1, iClick, sizeof(INPUT));
        cout << "Mouse down: " << x << endl;
#endif
    }
    void mouseUp()
    {
#ifndef WIN32
        if(mouseDwn){

        if(display == NULL)
        {
            cout << "Could not open display..." << endl;
        }

        event.type = ButtonRelease;
        event.xbutton.button = 1;
        event.xbutton.same_screen = True;
        event.xbutton.state = Button1Mask;
        //event.xbutton.window=RootWindow(display, DefaultScreen(display));
        XQueryPointer(display, RootWindow(display, DefaultScreen(display)), &event.xbutton.root, &event.xbutton.window, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);

        event.xbutton.subwindow = event.xbutton.window;

        while(event.xbutton.subwindow)
        {
            event.xbutton.window = event.xbutton.subwindow;

            XQueryPointer(display, event.xbutton.window, &event.xbutton.root, &event.xbutton.subwindow, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);
        }
        XSendEvent(display, PointerWindow,True, ButtonReleaseMask, &event);

        cout << "UP" << endl;
        XFlush(display);
        mouseDwn=false;}
#else
        INPUT iClick[1];
        ZeroMemory(iClick, sizeof(iClick));
        iClick[0].type = INPUT_MOUSE;
        iClick[0].mi.dwFlags = MOUSEEVENTF_LEFTUP;

        unsigned int x=SendInput(1, iClick, sizeof(INPUT));
        cout << "Mouse Up: " << x << endl;
#endif
    }
    void mouseClick(int button)
    {
#ifndef WIN32

        XEvent event;

        if(display == NULL)
        {
            cout << "Could not open display..." << endl;
        }

        memset(&event, 0x00, sizeof(event));

        event.type = ButtonPress;
        event.xbutton.button = button;
        event.xbutton.same_screen = True;

        XQueryPointer(display, RootWindow(display, DefaultScreen(display)), &event.xbutton.root, &event.xbutton.window, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);

        event.xbutton.subwindow = event.xbutton.window;

        while(event.xbutton.subwindow)
        {
            event.xbutton.window = event.xbutton.subwindow;

            XQueryPointer(display, event.xbutton.window, &event.xbutton.root, &event.xbutton.subwindow, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);
        }
        XSendEvent(display, PointerWindow, True, 0xfff, &event);

        XFlush(display);

        usleep(100000);

        event.type = ButtonRelease;
        event.xbutton.state = 0x100;

        XSendEvent(display, PointerWindow, True, 0xfff, &event);

        XFlush(display);

#else
        INPUT iClick[1];
        ZeroMemory(iClick, sizeof(iClick));
        iClick[0].type = INPUT_MOUSE;
        iClick[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

        unsigned int x=SendInput(1, iClick, sizeof(INPUT));
        cout << "Mouse down: " << x << endl;
        ZeroMemory(iClick, sizeof(iClick));
        iClick[0].type = INPUT_MOUSE;
        iClick[0].mi.dwFlags = MOUSEEVENTF_LEFTUP;
        x=SendInput(1, iClick, sizeof(INPUT));
        cout << "Mouse up: " << x << endl;
#endif

    }

    void mouseMove(int coorX,int coorY)
    {
#ifndef WIN32


        if(display == NULL)
        {
            cout << "Could not open display..." << endl;
        }

          XWarpPointer(display, None, RootWindow(display, DefaultScreen(display)), 0,0,0,0, coorX, coorY);
        //XWarpPointer(display, None, PointerWindow, 0,0,0,0, coorX, coorY);

          XFlush(display);

#else
        SetCursorPos(coorX,coorY);
#endif
    }
protected:
private:
};

#endif // MOUSEACTIONS_H

