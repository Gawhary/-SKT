/*ransacfit.h -
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

#ifndef RANSACFIT_H
#define RANSACFIT_H


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

using namespace cv;
using namespace TUIO;
using namespace std;

class ransacfit
{
    public:
        ransacfit();
        virtual ~ransacfit();
        /*
        Funciones
        */
        void setData(vector<CvPoint3D32f> datos);
        void selectRandomBla(vector<CvPoint3D32f> datos,double porcentaje);
        double minimumSquares(vector<CvPoint3D32f> dat);
        bool doRansac();
        void prueba(vector<CvPoint3D32f> datos);
        double distanciaPM(CvPoint3D32f p);
        double distanciaPP(CvPoint3D32f p,CvPoint3D32f p2);
        double currentGoodModel[5];
    protected:
    private:
        /*input:
    data - a set of observations  - Set de puntos 3D
    plVal - a model that can be fitted to data - Cuatro valores del plano (doubles)
    nMin - the minimum number of data required to fit the model - Numero de puntos random a tomar (quizas se puede elegir random tambien)
    iterations - the number of iterations performed by the algorithm  -  Limite de tiempo del algoritmo
    threshold - a threshold value for determining when a datum fits a model - Double min squares del modelo random
    numValid - the number of close data values required to assert that a model fits well to data - Porcentual con respecto al numero de puntos ingresados*/
        vector<CvPoint3D32f> data;
        vector<CvPoint3D32f> currentTestData;
        vector<CvPoint3D32f> currentTestOut;
        vector<CvPoint3D32f> currentGoodData;

        double plVal[5];
        double currentTestModel[5];

        int nMin;//=10;
        int iterations;//=10;
        double thresholdModelo;//=1.0;
        double thresholdIdeal;//=0.001;
        double currentTestThreshold;//=10000000000000000.0;
        double currentGoodThreshold;//=10000000000000000.0;
        int numValid;//=5;
		int pos[200];









};

#endif // RANSACFIT_H
