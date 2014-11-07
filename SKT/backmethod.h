/*backmethod.h -
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
#ifndef BACKMETHOD_H
#define BACKMETHOD_H

#include <stdlib.h>
#include <ctime>
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <math.h>
#include <calibrator.h>
using namespace cv;
using namespace TUIO;
using namespace std;

class backmethod
{
    public:
        backmethod();
        virtual ~backmethod();
        unsigned int Getreps() { return nback; }
        void Setreps(unsigned int val) { nback = val; }
        void repIncr(){nback++;}
        void start(){cont=true;nback=0;}
        bool contVal(){return cont;}
        bool proc(double varVal,IplImage* nue,IplImage * mask,IplImage* Base,IplImage* maskBack,IplImage* iRGB,CvFont* letra);
    protected:
    private:
        unsigned int nback;
        bool cont;
        IplImage* ACC;
        IplImage* sqACC;
        IplImage* ACCsq;
        IplImage* varianza2;
        IplImage* maskVar;
        IplImage* maskACC;
        int n;
        char text[100];
};

#endif // BACKMETHOD_H
