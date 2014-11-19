/*calibrator.h -
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
#ifndef CALIBRATOR_H
#define CALIBRATOR_H

#include <stdlib.h>
#include <ctime>
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <vector>
#include <TuioServer.h>
#include <TuioCursor.h>
#include <TuioTime.h>
#include <TuioClient.h>
#include <tinyxml.h>
#include <tinystr.h>

#include "cvimagewidgetgl.h"

#ifndef WIN32
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#else
#include <Windows.h>
#endif

#define CAL_INTERVAL_TIME 0.5

using namespace cv;
using namespace TUIO;
using namespace std;

class calibrator
{
public:
    calibrator(int i,int wi,int he,int le,int up);
    virtual ~calibrator();
    unsigned int Getxxx(){return xxx;}
    void Setxxx(unsigned int val){xxx = val;}
    void screenInfo(unsigned int* wi,unsigned int* he);
    bool solveMat(CvMat* Xin,CvMat* Xth);
    void setValGot(CvMat* Xin, CvPoint3D32f p, int numP);
    void setValPre(CvMat* Xs, CvPoint p, int numP,int xoff, int yoff,double xsc,double ysc);
    float getXY(int x_or_y,int cX,int cY,int wi,int he,bool X_Y,bool flipX,bool flipY,double depth);
    void MouseEv(int event,int x,int y,int flag);
    void zoneCalBegin();
    bool zoneCal(IplImage *iRGB,IplImage* maskZona,IplImage* maskBack,CvFont *letra,int maskMult);
    void enlargeMask(IplImage* maskZona,int maskMul);
    void drawZone(bool show,IplImage* iRGB);
    void pointCalBegin(CvImageDockableWidgetGL *win);
    void pointCal(IplImage* iRGB, CvFont *letra, CvImageDockableWidgetGL *win);
    void pointCalUpdate(int ID,float x, float y,double profundidad);
    void pointCalNew(int ID,float x, float y,double profundidad);
    void pointCalRemove(int ID);
    void abortCal(){
            calibradoPoint=false;
            calibrandoPoint=false;
            remapearPointCal=false;
    }
    unsigned int GetWidth(){
        return width;
    }
    unsigned int GetHeight(){
        return height;
    }
    bool occupied(){
        return (calibrando || calibrandoPoint);
    }
    double getMatVal(CvMat* m,int x, int y);
    void setMatVal(CvMat* m,int x, int y, double val);
    void includeZone(CvMat* m, int zoneNum);
    void subMatrix(CvMat* mEntrada,CvMat* mPre, CvMat* salidaMed, CvMat* salidaPre, int numZona);
    void solveMatZone(CvMat* Xin, CvMat* Xth, int zoneNum);
    bool isInZone(CvPoint p,int zoneNum);
    double distanciaPP(CvPoint p,CvPoint p2);
    bool isInZoneCalPoint(int punto, int zoneNum);
    IplImage* zona0;
protected:
private:
    unsigned int xxx;
    int nPoints;
    int nZones;
    int nSet;
    clock_t inicioCal,finCal;
    vector<IplImage> zonas;
    double aP[1000][3];
    double bP[1000][3];
    bool calibradoPoint;
    bool calibrandoPoint;
    bool remapearPointCal;
    /*Point calibration variables */
    CvMat* Xmed;
    CvMat* Xscr;
    CvPoint3D32f *esqPoint;
    CvPoint *esqPointSet;
    bool *esqcP;
    bool *esqcPG;
    int *esqCalID;
    int *numeroRepsCal;
    unsigned int width,height;
//////////////////////////////////////////////////////////////////////
//Calibration Zone variables.
    CvPoint esq[4];
    CvPoint esqMask[4];
    bool calibrado;
    bool calibrando;
    bool remapear;
    bool esqc[4];
    double a[4];
    double b[4];
    double di[4];
};

#endif // CALIBRATOR_H
