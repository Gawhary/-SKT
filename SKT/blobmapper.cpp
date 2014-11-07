/*blobmapper.cpp -
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
#include "blobmapper.h"

blobmapper::blobmapper(int maxB)
{
    //ctor
    maxBlobs=maxB;
    mouseAc=new mouseactions();
}

blobmapper::~blobmapper()
{
    //dtor
    delete mouseAc;
}
bool blobmapper::noBlobs(CvSeq* blobs,TuioServer *server,TuioTime* time,bool info,bool asTouch,bool mouse)
{
    anteriores=server->getTuioCursors();
    if(!blobs)
    {
        *time=TuioTime::getSessionTime();
        server->initFrame(*time);
        for(cursor = anteriores.begin(); cursor != anteriores.end( ); cursor++ )
        {
            int ID=(*cursor)->getCursorID();
            server->removeTuioCursor(*cursor);
            if(info){
            cout << "REMOVE : " << ID << endl;}
			if(mouse && ID==0){
            mouseAc->mouseUp();
        }
        }
        
        server->commitFrame();
        return true;
    }
    else
    {
        return false;
    }
}
int blobmapper::blobRemap(CvSeq* blobs,TuioServer * server,TuioTime* time,int minBlbSize,IplImage* iRGB,calibrator* CAL,CvFont* letra,bool X_Y,bool X_F,bool Y_F,bool show,bool mouse,IplImage* prof, bool info, bool asTouch)
{
    int maxID=0;
    int BlbNum=0;
    int cX,cY;
    int n;
    char ind[4];
    float *depthFind; //PATCH FOR DEPTH
    //IplImage* mascara=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,1);
    float va;
    while(blobs)
    {
        cvMoments (blobs, &moments ,1);
        if(moments.m00>minBlbSize && BlbNum<maxBlobs)
        {
            nuevo[BlbNum]=true;
            //CvPoint2D32f centro;
            //float radio;
            //cvMinEnclosingCircle(blobs,&centro,&radio);
            cX=(moments.m10 / moments.m00);
            cY=(moments.m01 / moments.m00);
            //cvZero(mascara);
            //cvCircle(mascara,cvPoint(cX,cY),(int)radio,cvScalar(255),-1);
            //CvScalar profund=cvAvg(prof,mascara);
            //va=*(profund.val);
            depthFind=(float*)(prof->imageData + prof->widthStep*(cY));
            nuevos[BlbNum][0]=CAL->getXY(0,cX,cY,640,480,X_Y,X_F,Y_F,depthFind[cY]);
            nuevos[BlbNum][1]=CAL->getXY(1,cX,cY,640,480,X_Y,X_F,Y_F,depthFind[cY]);
            nuevos[BlbNum][2]=cX;
            nuevos[BlbNum][3]=cY;
            nuevos[BlbNum][4]=depthFind[cY];
            depthFind=NULL;
            for (cursor = anteriores.begin(); cursor != anteriores.end( ); cursor++ )
            {
                distancias[BlbNum][(*cursor)->getCursorID()]=sqrt(pow((nuevos[BlbNum][0]-(*cursor)->getX()),2)+pow((nuevos[BlbNum][1]-(*cursor)->getY()),2));
                maxID=maxID>(*cursor)->getCursorID() ? maxID:(*cursor)->getCursorID();
            }
            if(show)
            {
                cvCircle(iRGB,cvPoint(cX,cY),sqrt((double)moments.m00/3.14),cvScalar(255,0,0),-1);
            }
            BlbNum++;
        }
        blobs=blobs->h_next;
    }
    for(cursor = anteriores.begin(); cursor != anteriores.end( ); cursor++ )
    {
        viejo[(*cursor)->getCursorID()]=false;
    }
    bool buscar=true;

    while(buscar)
    {
        buscar=false;
        float minDist=10000;
        int indiceCercano=90000;
        int ID,IDD;
        for (int i = 0; i < BlbNum; i++ )
        {
            for(cursor = anteriores.begin(); cursor != anteriores.end( ); cursor++ )
            {
                ID=(int)(*cursor)->getCursorID();
                if(minDist>distancias[i][ID] && nuevo[i] && !viejo[ID])
                {
                    minDist=distancias[i][ID];
                    indiceCercano=i;
                    IDD=ID;
                    buscar=true;
                }
            }
        }
        if(buscar)
        {
            nuevo[indiceCercano]=false;
            viejo[IDD]=true;
            vN[IDD]=indiceCercano;
        }
    }
    *time=TuioTime::getSessionTime();
    server->initFrame(*time);
    int divisor=1000;
    for(cursor = anteriores.begin(); cursor != anteriores.end( ); cursor++ )
    {
        int ID=(*cursor)->getCursorID();
        if(viejo[ID])
        {
            if(info){
            // cout << "UPDATE : " << ID << " - (" << nuevos[vN[ID]][0] << "," << nuevos[vN[ID]][1] << ")"<< endl;
            }
            server->updateTuioCursor(*cursor,nuevos[vN[ID]][0],nuevos[vN[ID]][1]);
            if(mouse && ID==0)
            {
                mouseAc->mouseMove((double)((int)(nuevos[vN[ID]][0]*divisor))/divisor*CAL->GetWidth(),(double)((int)(nuevos[vN[ID]][1]*divisor))/divisor*CAL->GetHeight());
                if(asTouch){
                mouseAc->mouseDown();
                }
            }
            if(mouse && ID==1 && viejo[0]==true && !asTouch)
            {
                //mouseAc->mouseClick(1);
				mouseAc->mouseDown();
            }
            if(show)
            {
                n=sprintf(ind,"%d",ID);
                cvPutText(iRGB,ind,cvPoint((int)nuevos[vN[ID]][2],(int)nuevos[vN[ID]][3]),letra,cvScalar(0,255,0));
            }
            depthFind=(float*)(prof->imageData + prof->widthStep*(int)nuevos[vN[ID]][3]);
            CAL->pointCalUpdate(ID,(float)nuevos[vN[ID]][0], (float)nuevos[vN[ID]][1],(double)nuevos[vN[ID]][4]);
            //cout << "prof: " << depthFind[(int)nuevos[vN[ID]][2]] << endl;
            depthFind=NULL;
        }
    }
    for (int i = 0; i < BlbNum; i++ )
    {
        if(nuevo[i])
        {
            depthFind=(float*)(prof->imageData + prof->widthStep*((int)nuevos[i][3]));
            TuioCursor* tcur=server->addTuioCursor(nuevos[i][0],nuevos[i][1]);
            int ID=tcur->getCursorID();
            if(asTouch && mouse && ID==0){
                mouseAc->mouseDown();
            }
            if(mouse && ID==1 && viejo[0]==true && !asTouch)
            {
                //mouseAc->mouseClick(1);
				mouseAc->mouseDown();
            }
            if(show)
            {
                n=sprintf(ind,"%d",(int)tcur->getCursorID());
                cvPutText(iRGB,ind,cvPoint((int)nuevos[i][2],(int)nuevos[i][3]),letra,cvScalar(0,255,0));
            }
            if(info){
            //cout << "ADD : " << ID << endl;
            }
            CAL->pointCalNew(ID,(float)nuevos[i][0], (float)nuevos[i][1],(double)nuevos[i][4]);
            depthFind=NULL;
        }
    }
    for(cursor = anteriores.begin(); cursor != anteriores.end( ); cursor++ )
    {
        int ID=(*cursor)->getCursorID();
        if(!viejo[ID])
        {
            server->removeTuioCursor(*cursor);
            if(info){
            //cout << "REMOVE : " << ID << endl;
            }
            if(mouse && ID==1 && !asTouch)
            {
                mouseAc->mouseUp();
            }
            if(mouse && ID==0 && asTouch)
            {
                mouseAc->mouseUp();
            }
            CAL->pointCalRemove(ID);
        }
    }
    server->commitFrame();
    return BlbNum;
}

