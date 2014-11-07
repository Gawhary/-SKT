/*virtualplane.cpp -
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

#include "virtualplane.h"

virtualplane::virtualplane()
{
    //ctor
    pl=new double[4];
    pl[0]=0.0;
    pl[1]=0.0;
    pl[2]=0.0;
    pl[3]=0.0;
    normal=1.0;
    created=true;
}
virtualplane::virtualplane(double* p)
{
    //ctor
    pl=new double[4];
    pl[0]=p[0];
    pl[1]=p[1];
    pl[2]=p[2];
    pl[3]=p[3];
    normal=p[4];
    created=true;
}
virtualplane::~virtualplane()
{
    //dtor
    delete pl;
}

void virtualplane::distPlano(IplImage* in,IplImage* out,int mod,int neg)
{
    float *ptr,*ptr2;
    for(int y=0; y<in->height; y++)
    {
        ptr = ((float*)(in->imageData + in->widthStep*y));
        ptr2 = ((float*)(out->imageData + out->widthStep*y));
        for(int x=0; x<in->width; x++)
        {
            if(mod==1)
            {
                if(neg==0)
                {
                    ptr2[x]=abs(pl[0]*x+pl[1]*y-ptr[x]+pl[2])/normal;
                }
                else
                {
                    ptr2[x]=-(double)(pl[0]*x+pl[1]*y-ptr[x]+pl[2])/normal;
                }
            }
            else
            {
                ptr2[x]=(double)(pl[0]*x+pl[1]*y-ptr[x]+pl[2])/normal;
            }
        }
        ptr=NULL;
        ptr2=NULL;
    }
}
void virtualplane::calcPlano(IplImage* depth,IplImage* MascaraZona,IplImage* Mascara)
{

    IplImage* maskTot=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,1);
    IplImage* in=cvCreateImage(cvSize(640,480),IPL_DEPTH_32F,1);

    cvAnd(MascaraZona,Mascara,maskTot);
    cvCopy(depth,in,maskTot);
    int NumeroPuntos=0;

    cout << "VIRTUAL PLANE : PERFORMING FIT, PLEASE WAIT..." << endl;
    float* ptr;

    //FOR RANSAC
    vector<CvPoint3D32f> datos;
    ransacfit *ranfi=new ransacfit();
    ///////////////
    for(int y=0; y<in->height; y++)
    {
        ptr = ((float*)(in->imageData + in->widthStep*y));
        for(int x=0; x<in->width; x++)
        {
            if(ptr[x]>0)
            {
                datos.push_back(cvPoint3D32f(x,y,ptr[x]));
                NumeroPuntos++;
            }
        }
        ptr=NULL;
    }
    ranfi->setData(datos);
    pl[0]=ranfi->currentGoodModel[0];
    pl[1]=ranfi->currentGoodModel[1];
    pl[2]=ranfi->currentGoodModel[2];
    pl[3]=pl[2];
    //cout << pl[0] << "," << pl[1] << "," << pl[2] << endl;
    delete ranfi;
}
void virtualplane::hacerPlano(IplImage* im,IplImage* mascara,int fisheye, double k)
{
    /////////////FISH EYE ATTEMPT
    int xC=0,yC=0;
    int numP=0;
    int xMin=640,xMax=0,yMin=480,yMax=0;
    for(int y=0; y<480; y++)
    {
        uchar* ptr2=((uchar*)(mascara->imageData + mascara->widthStep*(y)));
        for(int x=0; x<640; x++)
        {
            if(ptr2[x]>0)
            {
                xC+=x;
                yC+=y;
                numP++;
                xMin=(xMin>x) ? x:xMin;
                xMax=(xMax<x) ? x:xMax;
                yMin=(yMin>y) ? y:yMin;
                yMax=(yMax<y) ? y:yMax;
            }
        }
    }
    xC=xC/numP;
    yC=yC/numP;


    double xx,yy,xxx,yyy;
    int xSal,ySal;
    double Modu=1.0,Modu2=1.0;
    double Px,Py,Px2,Py2;
    int wi=((xMax-xMin)>0) ? (xMax-xMin):640;
    int he=((yMax-yMin)>0) ? (yMax-yMin):480;
    ////////////////////////////////
    cvZero(im);
    for(int y=0; y<480; y++)
    {
        float* ptr=((float*)(im->imageData + im->widthStep*(y)));
        uchar* ptr2=((uchar*)(mascara->imageData + mascara->widthStep*(y)));
        for(int x=0; x<640; x++)
        {
            if(ptr2[x]>0)
            {
                if(fisheye==1) //FISHEYE ATTEMPT DIDN'T WORK.... :(
                {
                    Px=(double)(x-xC)/wi;
                    Py=(double)(y-yC)/he;
                    //Px=(double)(2*x-640)/640;
                    //Py=(double)(2*y-480)/480;
                    //Modu=((x-xC)*(x-xC)+(y-yC)*(y-yC));
                    Modu=Px*Px+Py*Py;
                    //xx=x-((x-xC)*k*Modu);
                    //yy=y-((y-yC)*k*Modu);
                    //Modu2=((x-xC)*(x-xC)+(y-yC)*(y-yC));
                    Modu2=Px/(1-k*Modu)*Px/(1-k*Modu)+Py/(1-k*Modu)*Py/(1-k*Modu);
                    Px2=Px/(1-k*Modu2);
                    Py2=Py/(1-k*Modu2);
                    xSal=(int)((Px2)*wi+xC);
                    ySal=(int)((Py2)*he+yC);
                    //cout << xSal << ","<< x << endl;
                    if(xSal<=640 && xSal>=0 && ySal<=480 && ySal>=0)
                    {
                        //ptr=((float*)(im->imageData + im->widthStep*(ySal)));
                        ptr[x]=(float)(pl[0]*xSal+pl[1]*ySal+pl[2]);
                    }
                    else
                    {
                        ptr[x]=1.0;
                    }
                }
                else
                {
                    ptr[x]=(float)(pl[0]*x+pl[1]*y+pl[2]);//+(k*((x-xC)*(x-xC)+(y-yC)*(y-yC))));
                }
            }
        }
    }
}
