/*calibrator.cpp -
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

#include "calibrator.h"

calibrator::calibrator(int i,int wi,int he,int le,int up)
{
    //ctor
    nSet=i;
    int k=i*i;
    nPoints=k;
    nZones=(i-1)*(i-1);
    int difx=(wi-2*le)/(i-1);
    int dify=(he-2*up)/(i-1);
    calibradoPoint=false;
    calibrandoPoint=false;
    remapearPointCal=false;
    /*Point calibration variables */
    Xmed=cvCreateMat(3,k,CV_32FC1);
    Xscr=cvCreateMat(2,k,CV_32FC1);
    esqPoint=new CvPoint3D32f[k];
    esqPointSet=new CvPoint[k];
    esqcP=new bool[k];
    esqcPG=new bool[k];
    esqCalID=new int[k];
    numeroRepsCal=new int[k];
    for(int j=0; j<k; j++)
    {
        esqPoint[j]=cvPoint3D32f(0,0,0);
        esqcP[j]=false;
        esqcPG[j]=false;
        esqCalID[j]=100;
        numeroRepsCal[j]=0;
    }
    int u=0;
    for(int j=0; j<i; j++)
    {
        for(int w=0; w<i; w++)
        {
            int xVal=le+difx*w;
            int yVal=up+dify*j;
            esqPointSet[u]=cvPoint(xVal,yVal);
            u++;
        }
    }
    zona0=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,1);
//////////////////////////////////////////////////////////////////////
//Calibration Zone variables.
    calibrado=false;
    calibrando=false;
    remapear=false;
    for(int j=0; j<4; j++)
    {
        esq[j]=cvPoint(0,0);
        esqMask[j]=cvPoint(0,0);
    }
    bool esqc[4];
    double a[4];
    double b[4];
    double di[4];
    // @Gawhary fixing screen resolution to 1024x768 as required
	// screenInfo(&width,&height);
	width = 1024;
	height = 768;

}

calibrator::~calibrator()
{
    //dtor
    delete esqPoint;
    delete esqPointSet;
    delete esqcP;
    delete esqcPG;
    delete esqCalID;
    delete numeroRepsCal;
}

/*Mouse events, mostly for calibration purposes*/
void calibrator::MouseEv(int event,int x,int y,int flag)
{
    if(event==CV_EVENT_LBUTTONDOWN)
    {
    }
    if(event==CV_EVENT_LBUTTONUP)
    {
        if(calibrando)
        {
            for(int g=0; g<4; g++)
            {
                if(!esqc[g])
                {
                    esq[g]=cvPoint(x,y);
                    esqc[g]=true;
                    if(g==3)
                    {
                        calibrando=false;
                        calibrado=true;
                    }
                    g=4;
                }
            }
        }
    }
}
void calibrator::zoneCalBegin()
{
    calibrando=true;
    calibrado=false;
    for(int r=0; r<4; r++)
    {
        esqc[r]=false;
    }
}
bool calibrator::zoneCal(IplImage *iRGB,IplImage* maskZona,IplImage* maskBack,CvFont *letra,int maskMult)
{
    bool done=false;
    if(calibrando && !calibradoPoint)
    {
        char text[100]="";
        char text2[100]="";
        int n=sprintf(text,"Points: 0->TopLeft/1->TopRight/2->BotRight/3->BotLeft");
        cvPutText(iRGB,text,cvPoint(40,40),letra,cvScalar(0,0,255));
        int zonaActual=0;
        for(int r=0; r<4; r++)
        {
            if(esqc[r])
            {
                cvCircle(iRGB,esq[r],20,cvScalar(0,0,(int)(60*(r+1))),5);
            }
            else
            {
                zonaActual=r;
                r=4;
            }
        }
        n=sprintf(text2,"Click on point : %d",zonaActual);
        cvPutText(iRGB,text2,cvPoint(40,80),letra,cvScalar(0,255,255));
    }
    if(calibrado && !calibradoPoint)
    {
        int centroX=0;
        int centroY=0;
        for(int u=0; u<4; u++)
        {
            centroX+=esq[u].x;
            centroY+=esq[u].y;
        }
        centroX=centroX/4;
        centroY=centroY/4;
        for(int u=0; u<4; u++)
        {
            esqMask[u].x=(esq[u].x-centroX)*((double)maskMult/100)+centroX;
            esqMask[u].y=(esq[u].y-centroY)*((double)maskMult/100)+centroY;
            esqMask[u].x=(esqMask[u].x>640) ? 640:esqMask[u].x;
            esqMask[u].y=(esqMask[u].y>480) ? 480:esqMask[u].y;
            esqMask[u].x=(esqMask[u].x<0) ? 0:esqMask[u].x;
            esqMask[u].y=(esqMask[u].y<0) ? 0:esqMask[u].y;
        }
        cvSetZero(maskZona);
        cvFillConvexPoly(maskZona,esqMask,4,cvScalar(255));
        cvAnd(maskZona,maskBack,maskBack);
        a[0]=(double)(esq[1].y-esq[0].y)/(esq[1].x-esq[0].x);
        b[0]=esq[1].y-a[0]*esq[1].x;
        a[1]=(double)(esq[3].y-esq[2].y)/(esq[3].x-esq[2].x);
        b[1]=esq[2].y-a[1]*esq[2].x;
        a[2]=(double)(esq[2].x-esq[1].x)/(esq[2].y-esq[1].y);
        b[2]=esq[2].x-a[2]*esq[2].y;
        a[3]=(double)(esq[3].x-esq[0].x)/(esq[3].y-esq[0].y);
        b[3]=esq[0].x-a[3]*esq[0].y;
        remapear=true;
        calibrado=false;
        done=true;
    }
    if(done)
    {
        return true;
    }
    else
    {
        return false;
    }
}
void calibrator::enlargeMask(IplImage* maskZona,int maskMul)
{
    int centroX=0;
    int centroY=0;
    for(int u=0; u<4; u++)
    {
        centroX+=esq[u].x;
        centroY+=esq[u].y;
    }
    centroX=centroX/4;
    centroY=centroY/4;
    for(int u=0; u<4; u++)
    {
        esqMask[u].x=(esq[u].x-centroX)*((double)maskMul/100)+centroX;
        esqMask[u].y=(esq[u].y-centroY)*((double)maskMul/100)+centroY;
        esqMask[u].x=(esqMask[u].x>640) ? 640:esqMask[u].x;
        esqMask[u].y=(esqMask[u].y>480) ? 480:esqMask[u].y;
        esqMask[u].x=(esqMask[u].x<0) ? 0:esqMask[u].x;
        esqMask[u].y=(esqMask[u].y<0) ? 0:esqMask[u].y;
    }
    cvSetZero(maskZona);
    cvFillConvexPoly(maskZona,esqMask,4,cvScalar(255));
}
void calibrator::drawZone(bool show,IplImage* iRGB)
{
    if((remapear || remapearPointCal) && show)
    {
        int *num=new int[1];
        num[0]=4;
        CvPoint **e=new CvPoint*[1];
        e[0]=esq;
        cvPolyLine(iRGB,e,num,1,1,cvScalar(255),2);
        delete num;
        delete e;
    }
}
void calibrator::screenInfo(unsigned int* wi,unsigned int* he)
{
#ifndef WIN32
    Display *dis = XOpenDisplay(NULL);
    Window draw;
    int mmln1,mmln2;
    unsigned int xxmc1,xxmc2;
    if(dis == NULL)
    {
        cout << "Could not open display..." << endl;
    }
    XGetGeometry(dis, RootWindow(dis, DefaultScreen(dis)), &draw, &mmln1, &mmln2, wi, he, &xxmc1,&xxmc2);
    XFlush(dis);
    XCloseDisplay(dis);
#else
    RECT Screen,WorkAreaRect;
    GetWindowRect(GetDesktopWindow(), &Screen);
    SystemParametersInfo(SPI_GETWORKAREA, NULL, &WorkAreaRect, NULL);
    *wi=(double)(Screen.right-Screen.left);
    *he=(double)(Screen.bottom-Screen.top);
#endif
}
bool calibrator::solveMat(CvMat* Xin,CvMat* Xth)
{
    CvMat* Inv=cvCreateMat(nPoints,4,CV_32FC1);
    CvMat* MatCal=cvCreateMat(2,4,CV_32FC1);

    double in=cvInv(Xin,Inv,CV_SVD);
    if(in!=0.0)
    {
        cvMatMul(Xth,Inv,MatCal);
        float* ptr = (float*)(MatCal->data.ptr);
        aP[0][0]=ptr[0];
        aP[0][1]=ptr[1];
        aP[0][2]=ptr[2];
        aP[0][3]=ptr[3];
        /*aP[4]=ptr[4];
        aP[5]=ptr[5];
        aP[6]=ptr[6];
        aP[7]=ptr[7];
        aP[8]=ptr[8];
        aP[9]=ptr[9];*/
        ptr=NULL;
        ptr = (float*)(MatCal->data.ptr + MatCal->step);
        bP[0][0]=ptr[0];
        bP[0][1]=ptr[1];
        bP[0][2]=ptr[2];
        bP[0][3]=ptr[3];
        /*bP[4]=ptr[4];
        bP[5]=ptr[5];
        bP[6]=ptr[6];
        bP[7]=ptr[7];
        bP[8]=ptr[8];
        bP[9]=ptr[9];*/
        ptr=NULL;
        return true;
    }
    else
    {
        return false;
    }
}
void calibrator::solveMatZone(CvMat* Xin, CvMat* Xth, int zoneNum)
{
    CvMat* med=cvCreateMat(3,4,CV_32FC1);
    CvMat* pre=cvCreateMat(2,4,CV_32FC1);
    CvMat* Inv=cvCreateMat(4,3,CV_32FC1);
    CvMat* MatCal=cvCreateMat(2,3,CV_32FC1);
    subMatrix(Xin,Xth,med,pre,zoneNum);
    double in=cvInv(med,Inv,CV_SVD);
    cvMatMul(pre,Inv,MatCal);
    float* ptr = (float*)(MatCal->data.ptr);
    aP[zoneNum][0]=ptr[0];
    aP[zoneNum][1]=ptr[1];
    aP[zoneNum][2]=ptr[2];
    //aP[zoneNum][3]=ptr[3];
    ptr=NULL;
    ptr = (float*)(MatCal->data.ptr + MatCal->step);
    bP[zoneNum][0]=ptr[0];
    bP[zoneNum][1]=ptr[1];
    bP[zoneNum][2]=ptr[2];
    //bP[zoneNum][3]=ptr[3];
    ptr=NULL;
    //cout << Mat(MatCal) << endl;
    includeZone(Xin,zoneNum);
}
void calibrator::setValGot(CvMat* Xin, CvPoint3D32f p, int numP)
{
    float* ptr = (float*)(Xin->data.ptr);
    ptr[numP]=(double)p.x;
    ptr=NULL;
    ptr = (float*)(Xin->data.ptr + Xin->step);
    ptr[numP]=(double)p.y;
    ptr=NULL;
   /*ptr = (float*)(Xin->data.ptr + Xin->step*2);
    ptr[numP]=(double)p.z;
    ptr=NULL;*/
    ptr = (float*)(Xin->data.ptr + Xin->step*2);
    ptr[numP]=1.0;
    ptr=NULL;
    cout << "Point "<< numP <<" Saved : (" <<  p.x << "," << p.y << "," << p.z << ")" << endl;
}
void calibrator::setValPre(CvMat* Xs, CvPoint p, int numP,int xoff, int yoff,double xsc,double ysc)
{
    float* ptr = (float*)(Xs->data.ptr);
    ptr[numP]=(double)((p.x)*xsc+xoff);//width;
    ptr=NULL;
    ptr = (float*)(Xs->data.ptr +Xs->step);
    ptr[numP]=(double)((p.y)*ysc+yoff);//height;
    ptr=NULL;
}
float calibrator::getXY(int x_or_y,int cX,int cY,int wi,int he,bool X_Y,bool flipX,bool flipY,double depth)
{
    float curX=0.0;
    float curY=0.0;
    if(remapear && !remapearPointCal && !calibrandoPoint)
    {
        di[0]=fabs(cY-a[0]*cX-b[0]);
        di[1]=fabs(cY-a[1]*cX-b[1]);
        di[2]=fabs(cX-a[2]*cY-b[2]);
        di[3]=fabs(cX-a[3]*cY-b[3]);
        if(X_Y)
        {
            curX=di[0]/(di[0]+di[1]);
            curY=di[3]/(di[2]+di[3]);
        }
        else
        {
            curY=di[0]/(di[0]+di[1]);
            curX=di[3]/(di[2]+di[3]);
        }
        if(flipX)
        {
            curX=1.0-curX;
        }
        if(flipY)
        {
            curY=1.0-curY;
        }
    }
    else if (calibrandoPoint)
    {
        curX=(double)cX;//wi;
        curY=(double)cY;//he;
    }
    else if (remapearPointCal)
    {
        double XXX=(double)cX;//wi;
        double YYY=(double)cY;//he;
        bool estaEnZona=false;
        for(int kk=0;kk<nZones;kk++){
            if(isInZone(cvPoint(XXX,YYY),kk)){
        curX=aP[kk][0]*XXX+aP[kk][1]*YYY+aP[kk][2];//*depth+aP[kk][3];//*XXX*XXX+aP[4]*YYY*YYY+aP[5]*depth*depth+aP[6]*XXX*YYY+aP[7]*XXX*depth+aP[8]*YYY*depth+aP[9];
        curY=bP[kk][0]*XXX+bP[kk][1]*YYY+bP[kk][2];//*depth+bP[kk][3];//*XXX*XXX+bP[4]*YYY*YYY+bP[5]*depth*depth+bP[6]*XXX*YYY+bP[7]*XXX*depth+bP[8]*YYY*depth+bP[9];
        curX=(curX > 0.0) ? curX:0.0;
        curX=(curX > width) ? width:curX;
        curY=(curY > 0.0) ? curY:0.0;
        curY=(curY > height) ? height:curY;
        curX=curX/width;
        curY=curY/height;
        kk=nZones;
        estaEnZona=true;
            }
        }
        if(!estaEnZona){
            //GET CLOSEST POINT
            int puntoCercano=12000;
            double distanciaMin=10000000.0;
            for(int kk=0;kk<nPoints;kk++){
                if(distanciaPP(cvPoint(XXX,YYY),cvPoint(getMatVal(Xmed,kk,0),getMatVal(Xmed,kk,1)))<distanciaMin){
                    puntoCercano=kk;
                    distanciaMin=distanciaPP(cvPoint(XXX,YYY),cvPoint(getMatVal(Xmed,kk,0),getMatVal(Xmed,kk,1)));
                }
            }
            //LOOK FOR CLOSEST POINT ZONE
            //cout << "Punto cercano : " << getMatVal(Xmed,puntoCercano,0) << "," << getMatVal(Xmed,puntoCercano,1) << endl;
            for(int kk=0;kk<nZones;kk++){
            if(isInZone(cvPoint(getMatVal(Xmed,puntoCercano,0),getMatVal(Xmed,puntoCercano,1)),kk)){
            //USE THAT ZONE PARAMETERS
            curX=aP[kk][0]*XXX+aP[kk][1]*YYY+aP[kk][2];//*depth+aP[kk][3];//*XXX*XXX+aP[4]*YYY*YYY+aP[5]*depth*depth+aP[6]*XXX*YYY+aP[7]*XXX*depth+aP[8]*YYY*depth+aP[9];
            curY=bP[kk][0]*XXX+bP[kk][1]*YYY+bP[kk][2];//*depth+bP[kk][3];//*XXX*XXX+bP[4]*YYY*YYY+bP[5]*depth*depth+bP[6]*XXX*YYY+bP[7]*XXX*depth+bP[8]*YYY*depth+bP[9];
            curX=(curX > 0.0) ? curX:0.0;
            curX=(curX > width) ? width:curX;
            curY=(curY > 0.0) ? curY:0.0;
            curY=(curY > height) ? height:curY;
            curX=curX/width;
            curY=curY/height;
            kk=nZones;
            }
        }
        }
    }
    else
    {
        if(X_Y)
        {
            curX=(double)cX/wi;
            curY=(double)cY/he;
        }
        else
        {
            curY=(double)cX/wi;
            curX=(double)cY/he;
        }
        if(flipX)
        {
            curX=1.0-curX;
        }
        if(flipY)
        {
            curY=1.0-curY;
        }
    }
    if(x_or_y==0)
    {
        return curX;
    }
    else
    {
        return curY;
    }
}
void calibrator::pointCalBegin(CvImageWidgetGL* win)
{
    calibrandoPoint=true;
    for(int r=0; r<nPoints; r++)
    {
        esqcP[r]=false;
        esqcPG[r]=false;
        esqCalID[r]=100;
        numeroRepsCal[r]=0;
    }
//    setWindowProperty(win,CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    if(win) win->setFullscreen();
    zonas.clear();
}
void calibrator::pointCal(IplImage* iRGB,CvFont *letra,CvImageWidgetGL* win)
{
    if(calibrandoPoint)
    {
//        char text[100]="";
//        int n=sprintf(text,"Touch the center of the white circle. Press F to abort.");
//        cvPutText(iRGB,text,cvPoint(20,240),letra,cvScalar(0,0,255));
        for(int r=0; r<nPoints; r++)
        {
            if(esqcP[r])
            {
                if(!esqcPG[r])
                {
                    setValGot(Xmed, esqPoint[r], r);
                    esqcPG[r]=true;
                }
                cvCircle(iRGB,esqPointSet[r],20,cvScalar(0,255,0),5);
                if(r==(nPoints-1))
                {
                    for(int hh=0; hh<nPoints; hh++)
                    {
                        setValPre(Xscr, esqPointSet[hh], hh,0.0,0.0,(double)width/640,(double)height/480);
                    }
                    //bool funco=solveMat(Xmed,Xscr);
                    bool funco =true; //IT ALWAYS WORKS JEJEJE :)
                    for(int jj=0;jj<nZones;jj++){
                    solveMatZone(Xmed, Xscr, jj);
                    }
                    if(funco)
                    {
                        cout << "Calibration completed" << endl;
                        calibrandoPoint=false;
                        calibradoPoint=true;
                    }
                    else
                    {
                        cout << "Calibration could not be performed." << endl;
                        calibrandoPoint=false;
                        calibradoPoint=false;
                        remapearPointCal=false;
                    }
                }
            }
            else
            {
                if(esqCalID[r]!=100)
                {
                    cvCircle(iRGB,esqPointSet[r],20,cvScalar((int)(numeroRepsCal[r]*5),(int)(numeroRepsCal[r]*5),(int)(numeroRepsCal[r]*5)),5);
                    cvCircle(iRGB,esqPointSet[r],2,cvScalar(0,0,255),2);
                }

                else
                {
                    cvCircle(iRGB,esqPointSet[r],20,cvScalar(255,255,255),5);
                    cvCircle(iRGB,esqPointSet[r],2,cvScalar(0,0,255),2);
                }

                r=nPoints;
            }
        }
    }
    if(calibradoPoint)
    {
//        setWindowProperty(win,CV_WND_PROP_FULLSCREEN, CV_WINDOW_NORMAL);
        if(win) win->setFullscreen(false);
        remapear=false;
        calibradoPoint=false;
        remapearPointCal=true;
    }
}
void calibrator::pointCalNew(int ID,float x, float y,double profundidad)
{
    if(calibrandoPoint)
    {
        for(int r=0; r<nPoints; r++)
        {
            if(!esqcP[r])
            {
                esqCalID[r]=ID;
                inicioCal=clock();
                esqPoint[r].x=x;
                esqPoint[r].y=y;
                esqPoint[r].z=profundidad;
                numeroRepsCal[r]=1;
                r=nPoints;
            }
        }
    }
}
void calibrator::pointCalRemove(int ID)
{
    if(calibrandoPoint)
    {
        for(int r=0; r<nPoints; r++)
        {
            if(!esqcP[r])
            {
                if(esqCalID[r]==ID)
                {
                    esqCalID[r]=100;
                }
                r=nPoints;
            }
        }
    }
}
void calibrator::pointCalUpdate(int ID,float x, float y, double profundidad)
{
    if(calibrandoPoint)
    {
        for(int r=0; r<nPoints; r++)
        {
            if(!esqcP[r])
            {
                if(esqCalID[r]==ID)
                {
                    finCal=clock();
#ifndef WIN32
                    double intervalo=(double)(finCal-inicioCal)/1000000;
#else
                    double intervalo=(double)(finCal-inicioCal)/1000;
#endif
                    if(intervalo>CAL_INTERVAL_TIME)
                    {
                        esqPoint[r].x=esqPoint[r].x/numeroRepsCal[r];
                        esqPoint[r].y=esqPoint[r].y/numeroRepsCal[r];
                        esqPoint[r].z=esqPoint[r].z/numeroRepsCal[r];
                        esqcP[r]=true;
                    }
                    else
                    {
                        esqPoint[r].x+=x;
                        esqPoint[r].y+=y;
                        esqPoint[r].z+=profundidad;
                        numeroRepsCal[r]++;
                    }
                }
                r=nPoints;
            }
        }
    }
}
double calibrator::getMatVal(CvMat* m,int x, int y)
{
    float *ptr=(float*)(m->data.ptr + m->step*y);
    return (double)ptr[x];
}
void calibrator::setMatVal(CvMat* m,int x, int y, double val)
{
    float *ptr=(float*)(m->data.ptr + m->step*y);
    ptr[x]=val;
}
void calibrator::includeZone(CvMat* m, int zoneNum)
{
    int a=(int)(zoneNum/(nSet-1))*nSet+zoneNum%(nSet-1);
    IplImage* MM=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,1);
    CvPoint puntos[4];
    puntos[0].x=getMatVal(m,a,0);
    puntos[0].y=getMatVal(m,a,1);
    puntos[1].x=getMatVal(m,a+1,0);
    puntos[1].y=getMatVal(m,a+1,1);
    puntos[3].x=getMatVal(m,a+nSet,0);
    puntos[3].y=getMatVal(m,a+nSet,1);
    puntos[2].x=getMatVal(m,a+nSet+1,0);
    puntos[2].y=getMatVal(m,a+nSet+1,1);
    cvFillConvexPoly(MM,puntos,4,cvScalar(255));
    zonas.push_back(*MM);
}

void calibrator::subMatrix(CvMat* mEntrada,CvMat* mPre, CvMat* salidaMed, CvMat* salidaPre, int zoneNum)
{
    int a[4];
    a[0]=(int)(zoneNum/(nSet-1))*nSet+zoneNum%(nSet-1);
    a[1]=a[0]+1;
    a[3]=a[0]+nSet;
    a[2]=a[0]+nSet+1;
    for(int j=0; j<4; j++)
    {
        for(int k=0; k<3; k++)
        {
            setMatVal(salidaMed,j,k,getMatVal(mEntrada,a[j],k));
            if (k<2)
            {
                setMatVal(salidaPre,j,k,getMatVal(mPre,a[j],k));
            }
        }
    }
}

bool calibrator::isInZone(CvPoint p,int zoneNum){
    uchar* puntero=(uchar*)((&(zonas[zoneNum]))->imageData + ((&(zonas[zoneNum]))->widthStep)*p.y);
    //cout << "Valor del punto ("<< p.x<<","<< p.y<<") en zona " << zoneNum << " : " << (int)puntero[p.x] << endl;
    if((int)puntero[p.x]==255){
return true;}
    else{
    return false;
    }
}
double calibrator::distanciaPP(CvPoint p,CvPoint p2)
{
    return sqrt((double)((p.x-p2.x)*(p.x-p2.x)+(p.y-p2.y)*(p.y-p2.y)));
}
bool calibrator::isInZoneCalPoint(int punto, int zoneNum){
    int a;
    a=(int)(zoneNum/(nSet-1))*nSet+zoneNum%(nSet-1);
    if(a==punto || (a+1)==punto || (a+nSet)==punto || (a+nSet+1)==punto){
        return true;
    }
    else{
        return false;
    }
}
