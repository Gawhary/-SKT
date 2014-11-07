/*ransacfit.cpp -
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


#include "ransacfit.h"

ransacfit::ransacfit()
{

    nMin=10;
    iterations=500;
    thresholdModelo=1.0;
    thresholdIdeal=0.00015;
    numValid=5;

    currentTestThreshold=10000000000.0;
    currentGoodThreshold=10000000000.0;
    srand(time(0));
    //ctor
}

ransacfit::~ransacfit()
{
    //dtor
}

void ransacfit::selectRandomBla(vector<CvPoint3D32f> datos, double porcentaje)
{
    vector<CvPoint3D32f> sample;
	vector<CvPoint3D32f> notSample;
    int numDat=(porcentaje*datos.size()>200) ? 200:porcentaje*datos.size();
	//cout << numDat << endl;
    bool va=false;
    int num=10;
    for(int h=0; h<numDat; h++)
    {
        while(!va)
        {
            num=(rand()%(datos.size()));
            va=true;
            for(int k=0; k<h; k++)
            {
                if(pos[k]==num )//|| distanciaPP(datos[pos[k]],datos[num])<10.0)
                {
                    va=false;
                }
            }
        }
		//cout << num << endl;
        pos[h]=num;
        sample.push_back(datos[num]);
        va=false;
    }
	
    bool nono=true;
    for(int h=0; h<datos.size(); h++)
    {
        for(int k=0; k<numDat; k++)
        {
            if(h==pos[k])
            {
                nono=false;
            }
        }
        if(nono)
        {
            notSample.push_back(datos[h]);
        }
        nono=true;
    }
    currentTestData=sample;
    currentTestOut=notSample;

}
void ransacfit::setData(vector<CvPoint3D32f> datos)
{

    cout << "Num : " << datos.size() << endl;
    data=datos;
    doRansac();
}
double ransacfit::minimumSquares(vector<CvPoint3D32f> dat)
{
    CvMat *Mat1=cvCreateMat(4,4,CV_32FC1);
    CvMat *Xresultado=cvCreateMat(4,1,CV_32FC1);
    float* ptr;
    double sXX=0,sXY=0,sYY=0,sZX=0,sZY=0,sX=0,sY=0,sZ=0,sZZ=0;

    for(int j=0; j<dat.size(); j++)
    {
        sXX+=dat[j].x*dat[j].x;
        sXY+=dat[j].x*dat[j].y;
        sYY+=dat[j].y*dat[j].y;
        sZX+=dat[j].z*dat[j].x;
        sZY+=dat[j].z*dat[j].y;
        sX+=dat[j].x;
        sY+=dat[j].y;
        sZ+=dat[j].z;
        sZZ+=dat[j].z*dat[j].z;
    }

    ptr = (float*)(Mat1->data.ptr);
    ptr[0]=sXX;
    ptr[1]=sXY;
    ptr[2]=sZX;
    ptr[3]=sX;
    ptr=NULL;
    ptr = (float*)(Mat1->data.ptr + Mat1->step);
    ptr[0]=sXY;
    ptr[1]=sYY;
    ptr[2]=sZY;
    ptr[3]=sY;
    ptr=NULL;
    ptr = (float*)(Mat1->data.ptr + 2*Mat1->step);
    ptr[0]=sZX;
    ptr[1]=sZY;
    ptr[2]=sZZ;
    ptr[3]=sZ;
    ptr=NULL;
    ptr = (float*)(Mat1->data.ptr + 3*Mat1->step);
    ptr[0]=sX;
    ptr[1]=sY;
    ptr[2]=sZ;
    ptr[3]=dat.size();
    ptr=NULL;
    double pll[4];
    Mat MM1(Mat1);
    Mat Xres(Xresultado);
    cv::SVD::solveZ(MM1,Xres);
    *Xresultado=Xres;
    //Retriving result
    ptr = (float*)(Xresultado->data.ptr);
    pll[0]=ptr[0];
    ptr=NULL;
    ptr = (float*)(Xresultado->data.ptr+Xresultado->step);
    pll[1]=ptr[0];
    ptr=NULL;
    ptr = (float*)(Xresultado->data.ptr+2*Xresultado->step);
    pll[2]=ptr[0];
    ptr=NULL;
    ptr = (float*)(Xresultado->data.ptr+3*Xresultado->step);
    pll[3]=ptr[0];
    ptr=NULL;
    double pT[5];
    pT[0]=-pll[0]/pll[2];
    pT[1]=-pll[1]/pll[2];
    pT[2]=-pll[3]/pll[2];
    pT[3]=pT[2];
    pT[4]=sqrt(pT[0]*pT[0]+pT[1]*pT[1]+1);
    //cout << "VIRTUAL PLANE FROM RANSAC ITERATION :" << endl;
    //cout << -pll[0]/pll[2] << "," << -pll[1]/pll[2] << "," << -pll[2]/pll[2] << "," << -pll[3]/pll[2] << endl;
    double error=0;
    for(int j=0; j<dat.size(); j++)
    {
        error+=abs(pT[0]*dat[j].x+pT[1]*dat[j].y-dat[j].z+pT[2])/pT[4];
    }
    //cout << "MEDIA FROM ITERATION: " << (double)error/dat.size() << endl;
    for (int o=0;o<5;o++){
    currentTestModel[o]=pT[o];}
    return (double)error/dat.size();
}
void ransacfit::prueba(vector<CvPoint3D32f> dat)
{
    IplImage* imagen=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,1);
    cvZero(imagen);
    uchar* ptr;
    for(int j=0; j<dat.size(); j++)
    {
        ptr=(uchar*)(imagen->imageData + (int)(imagen->widthStep*dat[j].y));
        ptr[(int)dat[j].x]=255;//255*datos[j].z/6000;
        ptr=NULL;
    }
    cvSaveImage("test.jpg" ,imagen);
}
bool ransacfit::doRansac()
{
    bool terminar=false;
    int repets=0;
    while(!terminar)
    {
        selectRandomBla(data,0.01);
        double threS=minimumSquares(currentTestData);
        for(int j=0; j<currentTestOut.size(); j++)
        {
            if(distanciaPM(currentTestOut[j])<threS)
            {
                currentTestData.push_back(currentTestOut[j]);
            }
        }
        threS=minimumSquares(currentTestData);
        if(threS<currentGoodThreshold && currentTestData.size()>data.size()*0.5)
        {
            currentGoodThreshold=threS;
            currentGoodData=currentTestData;
            for (int o=0;o<5;o++){
            currentGoodModel[o]=currentTestModel[o];}

            if(threS<thresholdIdeal)
            {
                terminar=true;
                //cout << "ERROR LOGRADO : " << currentGoodThreshold << endl;
                //cout << "Plano Logrado : " << currentGoodModel[0] << "," << currentGoodModel[1] << "," << currentGoodModel[2] << endl;
            }
        }
        repets++;
        if(repets>iterations){
            terminar=true;
            //cout << "FIN DE REPETICIONES" << endl;
            //cout << "ERROR LOGRADO : " << currentGoodThreshold << endl;
            //cout << "Plano Logrado : " << currentGoodModel[0] << "," << currentGoodModel[1] << "," << currentGoodModel[2] << endl;
        }
    }
    return true;
}
double ransacfit::distanciaPM(CvPoint3D32f p)
{
    return abs(currentTestModel[0]*p.x+currentTestModel[1]*p.y-p.z+currentTestModel[2])/currentTestModel[4];
}
double ransacfit::distanciaPP(CvPoint3D32f p,CvPoint3D32f p2)
{
    return sqrt((p.x-p2.x)*(p.x-p2.x)+(p.y-p2.y)*(p.y-p2.y)+(p.z-p2.z)*(p.z-p2.z));
}

