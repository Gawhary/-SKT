/*virtualplane.h -
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
#ifndef VIRTUALPLANE_H
#define VIRTUALPLANE_H
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
#include "ransacfit.h"
#include <vector>
using namespace cv;
using namespace TUIO;
using namespace std;


class virtualplane
{
public:
    virtualplane();
    virtualplane(double* p);
    virtual ~virtualplane();
    double GetPl(int indx)
    {
        if(indx<4 && created){
        return pl[indx];}
        else return 0.0;
    }
    void SetPl(int indx, double val)
    {
        if(indx<4 && created){pl[indx] = val;}
    }
    /*Functions for plane parameter calculation, virtual plane creation and distance to plane meassure*/
    void distPlano(IplImage* in,IplImage* out,int mod,int neg);
    void calcPlano(IplImage* depth,IplImage* MascaraZona,IplImage* Mascara);
    void hacerPlano(IplImage* im,IplImage* mascara,int fisheye=0, double k=0);
/////////////////////////////////////////////////////////////////////////////
protected:
private:
    double *pl;
    double normal;
    bool created;
};

#endif // VIRTUALPLANE_H
