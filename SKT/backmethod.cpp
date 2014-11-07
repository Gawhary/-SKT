/*backmethod.cpp -
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

#include "backmethod.h"

backmethod::backmethod()
{
    //ctor
    nback=0;
    cont=true;
    ACC=cvCreateImage(cvSize(640,480),IPL_DEPTH_32F,1);
    sqACC=cvCreateImage(cvSize(640,480),IPL_DEPTH_32F,1);
    ACCsq=cvCreateImage(cvSize(640,480),IPL_DEPTH_32F,1);
    varianza2=cvCreateImage(cvSize(640,480),IPL_DEPTH_32F,1);
    maskVar=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,1);
    maskACC=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,1);
}

backmethod::~backmethod()
{
    //dtor
}
bool backmethod::proc(double varVal,IplImage* nue,IplImage * mask,IplImage* Base,IplImage* maskBack,IplImage* iRGB,CvFont* letra){
if(cont)
            {
                if(nback==0)
                {
                    cvSet(maskACC,cvScalar(255));
                    cvSet(ACC,cvScalar(0.0));
                    cvSet(maskVar,cvScalar(255));
                    cvSet(sqACC,cvScalar(0.0));
                    cvSet(ACCsq,cvScalar(0.0));
                }
                if(nback>99)
                {
                    cvConvertScale(ACC,ACC,(double)1/100);
                    cvConvertScale(sqACC,sqACC,(double)1/100);
                    cvMul(ACC,ACC,ACCsq);
                    cvAbsDiff(sqACC,ACCsq,varianza2);
                    cvPow(varianza2,varianza2,0.5);
                    cvDiv(varianza2,ACC,varianza2);
                    cvThreshold(varianza2,maskVar,varVal,255,CV_THRESH_BINARY_INV);
                    cvAnd(maskACC,maskVar,maskACC);
                    cvCopy(ACC,Base,maskACC);
                    cvCopy(maskACC,maskBack);
                    cont=false;
                    nback=-1;
                }
                else
                {
                    cvAnd(mask,maskACC,maskACC);
                    cvAcc(nue,ACC,maskACC);
                    cvSquareAcc(nue,sqACC,maskACC);
                    n=sprintf(text,"Calibrating Background... %d",nback);
                    cvPutText(iRGB,text,cvPoint(40,40),letra,cvScalar(0,0,255));
                }
                nback++;
                if(cont){return false;}
                else{return true;}
            }
    else{
            return false;
    }
}
