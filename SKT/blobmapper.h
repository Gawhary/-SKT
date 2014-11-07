/*blobmapper.h -
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
#ifndef BLOBMAPPER_H
#define BLOBMAPPER_H

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
#include <calibrator.h>
#include <mouseactions.h>
using namespace cv;
using namespace TUIO;
using namespace std;

class blobmapper
{
    public:
        blobmapper(int maxB);
        virtual ~blobmapper();
        bool noBlobs(CvSeq* blobs,TuioServer * server,TuioTime* time, bool info=true,bool asTouch=false,bool mouse=false);
        int blobRemap(CvSeq* blobs,TuioServer * server,TuioTime* time,int minBlbSize,IplImage* iRGB,calibrator* CAL,CvFont* letra,bool X_Y,bool X_F,bool Y_F,bool show,bool mouse,IplImage* prof, bool info=true,bool asTouch=false);
        mouseactions* mouseAc;
    protected:
    private:
        float nuevos[100][5];
        bool nuevo[100];
        bool viejo[100];
        int vN[100];
        float distancias[100][100];
        list< TuioCursor * >::iterator cursor;
        list< TuioCursor * > anteriores;
        CvMoments moments;
        int maxBlobs;
};

#endif // BLOBMAPPER_H
