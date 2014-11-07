/*SKT.cpp -
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
#include <string.h>

#ifndef WIN32
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#endif

#include <virtualplane.h>
#include <calibrator.h>
#include <blobmapper.h>
#include <backmethod.h>
#include <mouseactions.h>
using namespace cv;
using namespace TUIO;
using namespace std;

int g_imageDisplayMargin=10; // @Gawhary: Margins between multi-images in one window
int g_min=55;         //Min threeshold for selection of pixels near interaction zone. Divided by 10000.
int g_max=230;         //Max threeshold for selection of pixels near interaction zone. Divided by 10000.
int g_maxBlob=50;       //Max blob area for filter. NOT IMPLEMENTED
int g_minBlob=80;      //Min blob area for blob filter.
int g_thre=0;           //Threeshold for last image. Almost not used...
int g_mult=1;           //Multiplier for scale adjust of images. Not used...
int g_var=300;         //Minimum variance for pixels in the background allowed. Divided by 100000 after.
bool g_save=true;
int g_calBack=0;        //Background calibration toggle.
int g_calZona=0;        //Zone calibration toggle.
int g_showRGB=0;
int g_showPaBlobs=0;
int g_showFondo=0;
int g_showMask=0;
int g_showHelp=0;
int g_showBlobs=1;
int g_showArea=1;
int g_outInfo=1;
int g_XY=0;
int g_XF=0;
int g_YF=0;
int g_alpha=100;
int g_plano=0;
int g_showPlano=0;
int g_espacio=100;
int g_mod=0;
int g_neg=0;
int g_mouse=0;
int g_threshold=0;
bool g_agrandar=false;
const char* g_host;
int g_port=3333;
int g_maxNumBlobs=100;
int g_numPoints=5;
int g_auto=0;
int g_auto2=0;
int g_framesAuto=0;
int g_autoReps=0;
int g_delta=50;
int g_undist=0;
int g_movPlano=2000;
int g_escala=6000;
int g_fisheye=0;
int g_k=0;
int g_asTouch=0;
clock_t inicio,fin;
char* help="Press Ctrl+p for settings.\n"
		   "To change TUIO info edit settings.xml file\n"
           "Min Dist -> Min distance from interaction zone.\n"
           "Max Dist -> Max distance from interaction zone.\n"
           "Min Blob -> Min blob area.\n"
           "Deviation-> Min standard deviation of background pixs.\n"
           "Module -> Absolute difference for blob extraction. \n"
           "Mod-Zone -> Only negative differences (Module must be checked). \n"
		   "Use Plane -> Change extraction base to virtual plane (Created using Set Zone)\n"
		   "Threshold needs a zone before working.\n"
		   "Use Auto Min-Max when you feel confident you might get good blobs\n"
		   "at some value of Min-Max (good back or plane for example).\n"
		   "Auto Min-Max tries to find the best values where two blobs are visible,\n"
		   "so be sure to have two fingers (or hands) on the touch surface before activating it.\n"
		   "Mouse checkbox starts native mouse click and press events.\n"
		   "Blob ID=0 moves and ID=1 presses the left button.\n"
		   "The number of blobs and calibration points can be modified in settings.xml\n"
           "Press ESC on any window to exit.";
void affineWarperAndMixer(CvMat* warp,IplImage* iRGB,IplImage* aux1,IplImage* aux2,float alpha,IplImage* gray)
{
    //Deprecated, now OpenCV does the fit.
    //cvWarpAffine(iRGB,aux2,warp);
    cvCopy(iRGB,aux2);
    cvConvertImage(gray,aux1,CV_GRAY2RGB);
    cvAddWeighted(aux2,alpha,aux1,(1.0-alpha),0.0,iRGB);
}

/*hacer0bordes : Function to set the pixels in the border of the image to 0 for blobs to close*/
void hacer0bordes(IplImage* in)
{
    int x=0,y=0,wi=in->width,he=in->height;
    uchar* ptr = ((uchar*)(in->imageData + in->widthStep*(y+he-1)));
    for(int col=x; col<(x+wi); col++)
    {
        ptr[col]=0;
    }
    ptr=NULL;
    ptr = ((uchar*)(in->imageData+in->widthStep*(y)));
    for(int col=x; col<(x+wi); col++)
    {
        ptr[col]=0;
    }
    for(int row=y; row<(y+he); row++)
    {
        ptr=NULL;
        ptr=((uchar*)(in->imageData + in->widthStep*row));
        ptr[x]=0;
        ptr[x+wi-1]=0;
    }
}
/*Function to load parameters*/
std::string loadParameters(bool all)
{
    int read=0;
    TiXmlDocument doc( "settings.xml" );
    bool loadOkay=doc.LoadFile();
    if(loadOkay)
    {
        TiXmlElement * root=doc.FirstChildElement();
        TiXmlElement * valores=root->FirstChildElement();
        TiXmlElement * blobDet=valores->FirstChildElement("Blob-Detection");
        TiXmlElement * affineVal=valores->FirstChildElement("Affine-Calibration");
        TiXmlElement * tuioInfo=valores->FirstChildElement("Tuio-Info");
        TiXmlElement * pointCal=valores->FirstChildElement("Point-Calibration");
        if(all){
        read=blobDet->QueryIntAttribute("Min-Distance",&g_min);
        read=blobDet->QueryIntAttribute("Max-Distance",&g_max);
        read=blobDet->QueryIntAttribute("Min-Blob-Area",&g_minBlob);
        read=blobDet->QueryIntAttribute("Background-Accepted-Variance",&g_var);
        read=blobDet->QueryIntAttribute("Maximum-Number-Of-Blobs",&g_maxNumBlobs);
        ///////////////////////////////////////////////////////////////////////////////
        /*DEPRECATED
        read=affineVal->QueryIntAttribute("X-X",&g_xx);
        read=affineVal->QueryIntAttribute("Y-Y",&g_yy);
        read=affineVal->QueryIntAttribute("X-X-X",&g_xxx);
        read=affineVal->QueryIntAttribute("Y-Y-Y",&g_yyy);
        read=affineVal->QueryIntAttribute("X-Y",&g_xy);
        read=affineVal->QueryIntAttribute("Y-X",&g_yx);*/
        ///////////////////////////////////////////////////////////////////////////////
        read=affineVal->QueryIntAttribute("Alpha",&g_alpha);
		read=tuioInfo->QueryIntAttribute("Port",&g_port);
        read=pointCal->QueryIntAttribute("Number-Of-Calibration-Points",&g_numPoints);
        cout << "Parameter file: settings.xml - Loaded!" << endl;
        }
		return tuioInfo->Attribute("Host");
    }
    else
    {
        if(all){cout << "Parameter file: settings.xml - not found. Using default values." << endl;}
    }
	return "localhost";
}
/*FUNCTION TO SAVE PARAMETERS*/
void saveParameters(int a,void* param)
{
    TiXmlDocument doc;
    TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
    doc.LinkEndChild( decl );
    TiXmlElement * root = new TiXmlElement( "Parameters" );
    TiXmlElement * valores = new TiXmlElement( "Values" );
    doc.LinkEndChild( root );
    root->LinkEndChild( valores );
    TiXmlElement * blobDet= new TiXmlElement( "Blob-Detection" );
    TiXmlElement * affineVal= new TiXmlElement( "Affine-Calibration" );
    TiXmlElement * tuioInfo= new TiXmlElement( "Tuio-Info" );
    TiXmlElement * pointCal= new TiXmlElement( "Point-Calibration" );
    valores->LinkEndChild( blobDet );
    valores->LinkEndChild( affineVal );
    valores->LinkEndChild( tuioInfo );
    valores->LinkEndChild( pointCal );
    blobDet->SetAttribute("Min-Distance", g_min);
    blobDet->SetAttribute("Max-Distance", g_max);
    blobDet->SetAttribute("Min-Blob-Area", g_minBlob);
    blobDet->SetAttribute("Background-Accepted-Variance", g_var);
    blobDet->SetAttribute("Maximum-Number-Of-Blobs",g_maxNumBlobs);
    affineVal->SetAttribute("Alpha", g_alpha);
    tuioInfo->SetAttribute("Host",loadParameters(false).c_str());
    tuioInfo->SetAttribute("Port",g_port);
    pointCal->SetAttribute("Number-Of-Calibration-Points",g_numPoints);
    bool saveOk=doc.SaveFile( "settings.xml" );
    if(saveOk)
    {
        cout << "Parameters saved!" << endl;
    }
    else
    {
        cout << "Parameters could not be saved." << endl;
    }
}
void callBackPoint(int a,void* param)
{
    calibrator *MyCal=(calibrator*) param;
    MyCal->pointCalBegin("Tracker");
}
void callBackAgrandar(int a)
{
    g_agrandar=true;
}
void callBackSetValue(int a,void* param)
{
    int* valueToSet=(int*)param;
    *valueToSet=a;
}
void callBackButton(int a,void* param)
{
    int* valueToSet=(int*)param;
    *valueToSet=1;
}
void callBackButton0(int a,void* param)
{
    int* valueToSet=(int*)param;
    *valueToSet=0;
}
void MouseEvWrapper(int event,int x,int y,int flag,void* param)
{
    calibrator *MyCal=(calibrator*) param;
    MyCal->MouseEv(event,x,y,flag);
}
int main( int argc, char* argv[] )
{
	g_host=loadParameters(true).c_str();
    if(g_numPoints>10 || g_numPoints<2){g_numPoints=5;
    cout << "Invalid value for number of calibration points. Using 5." << endl;
    }
    if(g_maxNumBlobs>1000 || g_maxNumBlobs<0){
        cout << "Invalid value for max number of blobs. Using 50." << endl;
        g_maxNumBlobs=50;}
    cout << "Host : " << g_host << endl;
    cout << "Port : " << g_port << endl;

    TuioServer *server=new TuioServer(g_host,g_port);
    virtualplane *VP=new virtualplane();
    calibrator* CAL=new calibrator(g_numPoints,640,480,5,5);
    blobmapper* BM=new blobmapper(g_maxNumBlobs);
    backmethod* BACK=new backmethod();

    cout << "Screen : " << CAL->GetWidth() << ","<<  CAL->GetHeight() << endl;
    TuioTime time;
    CvMemStorage* g_storage = NULL;
    g_storage=cvCreateMemStorage(0);
    CvSeq* blobs=0;
    CvFont *letra=new CvFont;
    cvInitFont(letra,CV_FONT_HERSHEY_SIMPLEX,0.5,0.5,0.0,2);
    CvMat* warpAffin=cvCreateMat(2,3,CV_32FC1);

    int timing=0;

    cout << "Kinect opening ..." << endl;
    VideoCapture capture;
    try
    {
        capture.open(CV_CAP_OPENNI);
    }
    catch(exception e)
    {
        cout << endl << "Could not find any device." << endl;
        delete letra;
        delete server;
        delete CAL;
        delete VP;
        delete BM;
        delete BACK;
        return -1;
    }
    cout << "done." << endl;
    cout << "FPS Given by Device : " << capture.get(CV_CAP_PROP_FPS) << endl;
    capture.set( CV_CAP_OPENNI_DEPTH_GENERATOR_REGISTRATION_ON, 1 );

    if( !capture.isOpened() )
    {
        cout << "Could not open the device." << endl;
        delete letra;
        delete server;
        delete CAL;
        delete VP;
        delete BM;
        delete BACK;
        return -1;
    }

    namedWindow("Tracker",(0 | CV_GUI_NORMAL));
    //namedWindow("To Blobs",(0 | CV_GUI_NORMAL));
    //namedWindow("Background",(0 | CV_GUI_NORMAL));
    //namedWindow("Mask",(0 | CV_GUI_NORMAL));
    //namedWindow("Plano",(0 | CV_GUI_NORMAL));
	
    //cvResizeWindow("RGB",480,360);
    cvResizeWindow("Tracker",980,360);
    //cvResizeWindow("To Blobs",320,240);
    //cvResizeWindow("To Blobs",640,240);
    //cvResizeWindow("Background",320,240);
    //cvResizeWindow("Mask",320,240);
    //cvResizeWindow("Plano",320,240);

    cvMoveWindow("Tracker",520,0);
    //cvMoveWindow("To Blobs",0,500);
    //cvMoveWindow("Background",320,500);
    //cvMoveWindow("Mask",640,500);
    //cvMoveWindow("Plano",960,500);

    cvCreateButton("Show Help",callBackSetValue,&g_showHelp,CV_CHECKBOX,1);

    /*Parameters for processing*/
    cvCreateButton("Calibrate Background",callBackButton,&g_calBack,CV_PUSH_BUTTON);
    cvCreateButton("Set Zone",callBackButton,&g_calZona,CV_PUSH_BUTTON);
    cvCreateButton("Save Parameters",saveParameters,NULL,CV_PUSH_BUTTON);
    cvCreateTrackbar("Alpha",NULL,&g_alpha,100);
    cvCreateButton("Background Method",NULL,NULL,CV_RADIOBOX,1);
    cvCreateButton("Plane Method",callBackSetValue,&g_plano,CV_RADIOBOX,0);
    cvCreateButton("Threshold ",callBackSetValue,&g_threshold,CV_RADIOBOX,0);
    cvCreateTrackbar("Min Blob",NULL,&g_minBlob,10000);
    createButton("Module",callBackSetValue,&g_mod,CV_CHECKBOX,0);
    createButton("Mod-Zone",callBackSetValue,&g_neg,CV_CHECKBOX,0);
    //cvCreateTrackbar("Min Dist.","Tracker",&g_min,10000);
    //cvCreateTrackbar("Max Dist.","Tracker",&g_max,10000);
    cvCreateTrackbar("Deviation",NULL,&g_var,100000);
    createButton("Update Main",callBackSetValue,&g_showRGB,CV_CHECKBOX,1);
    createButton("Update To Blobs",callBackSetValue,&g_showPaBlobs,CV_CHECKBOX,1);
    createButton("Show Blobs",callBackSetValue,&g_showBlobs,CV_CHECKBOX,1);
    createButton("Show Valid Area",callBackSetValue,&g_showArea,CV_CHECKBOX,1);
    cvCreateTrackbar("Valid Mask Scale",NULL,&g_espacio,1000,callBackAgrandar);
    createButton("X <-> Y",callBackSetValue,&g_XY,CV_CHECKBOX,0);
    createButton("X Flip",callBackSetValue,&g_XF,CV_CHECKBOX,0);
    createButton("Y Flip",callBackSetValue,&g_YF,CV_CHECKBOX,0);
    createButton("Mouse",callBackSetValue,&g_mouse,CV_CHECKBOX,0);
    createButton("AsTouch",callBackSetValue,&g_asTouch,CV_CHECKBOX,0);
    createButton("Cal-RGB-Depth",callBackSetValue,&g_undist,CV_CHECKBOX,1);
    cvCreateButton("Start Point Calibration",callBackPoint,(void*) CAL,CV_PUSH_BUTTON);
    cvCreateTrackbar("Move Plane",NULL,&g_movPlano,4000,callBackAgrandar);
    cvCreateTrackbar("Delta-Auto",NULL,&g_delta,1000,NULL);
    cvCreateButton("Auto Min-Max",callBackButton,&g_auto,CV_PUSH_BUTTON);
	cvCreateButton("Stop Auto",callBackButton0,&g_auto2,CV_PUSH_BUTTON);
	//createButton("FishEye",callBackSetValue,&g_fisheye,CV_CHECKBOX,0);
	//cvCreateTrackbar("K",NULL,&g_k,100000,callBackAgrandar);
	createButton("OutTuioInfo",callBackSetValue,&g_outInfo,CV_CHECKBOX,1);
    cvSetMouseCallback("Tracker",MouseEvWrapper,(void*) CAL);


    /*Several image definitions needed...*/


    Mat imgMap,maskMap,imgRGB,disp,imgMapRemaped,imgRGBRemaped;
    CvMat iMap;
    IplImage* nue=cvCreateImage(cvSize(640,480),IPL_DEPTH_32F,1);
    IplImage* nue2=cvCreateImage(cvSize(640,480),IPL_DEPTH_32F,1);
    IplImage* plano=cvCreateImage(cvSize(640,480),IPL_DEPTH_32F,1);
    IplImage* Base=cvCreateImage(cvSize(640,480),IPL_DEPTH_32F,1);
    IplImage* paBlobs=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,1);
    IplImage* mask=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,1);
    IplImage* maskBack=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,1);
    IplImage* maskZona=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,1);

    IplImage* iRGB=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,3);
    IplImage* iRGBaux=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,3);
    IplImage* iRGBaux2=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,3);
    IplImage* sal=cvCreateImage(cvSize(640,480),IPL_DEPTH_32F,1);
    IplImage* sal0=cvCreateImage(cvSize(640,480),IPL_DEPTH_32F,1);
    	
	// @Gawhary: Make large container image for iRGB, paBlobs and thin (20 pixel) margin
	IplImage* iRGB_paBlobs=cvCreateImage(
		cvSize(iRGB->width + paBlobs->width + g_imageDisplayMargin,max(iRGB->height, paBlobs->height)),
		IPL_DEPTH_8U, 3);
	cvSet(iRGB_paBlobs, cvScalar(200,200,2));

	/////////////////////////////////////////////////////////////////


    g_showMask=0;
    g_showFondo=0;
    inicio=clock();
    int matchRGB=1;
    for(;;)
    {
        //RGB-DEPTH MATCH BY OPENCV
        if(matchRGB!=g_undist){
            capture.set( CV_CAP_OPENNI_DEPTH_GENERATOR_REGISTRATION_ON,g_undist);
            matchRGB=g_undist;
        }
        //Background calibration starting point.
        if(g_calBack==1)
        {
            g_calBack=0;
            BACK->start();
        }
        ////////////////////////////////////////

        //Zone calibration starting point.
        if(g_calZona==1)
        {
            g_calZona=0;
            CAL->zoneCalBegin();
            for(int r=0; r<4; r++)
            {
                VP->SetPl(r,0.0);
            }

        }
        ////////////////////////////////////////

        //Image grabbing and first conversions.
        if( !capture.grab() )
        {
            cout << "Couldn't grab images." << endl;
        }
        else
        {
            capture.retrieve( imgMap, CV_CAP_OPENNI_DEPTH_MAP);
            capture.retrieve( maskMap, CV_CAP_OPENNI_VALID_DEPTH_MASK);
            iMap=imgMap;
            *mask=maskMap;
            cvConvertScale(&iMap,nue,(double)1/6000);

            if(g_showRGB==1)
            {
                capture.retrieve(imgRGB,CV_CAP_OPENNI_BGR_IMAGE);
				//@ Gawhary: Check retrieved image size and recreate container if needed
				IplImage temp = imgRGB;
				if(imgRGB.cols > iRGB->width		||
					imgRGB.rows > iRGB->height		||
					temp.depth != iRGB->depth	||
					temp.nChannels != iRGB->nChannels	){
				// ToDo: check image depth and channels

						// ToDo: delete and recreate iRGB_paBlobs
						return -1;
					
				}
				*iRGB=temp;
                affineWarperAndMixer(warpAffin,iRGB,iRGBaux,iRGBaux2,(float)g_alpha/100,nue);
                CAL->drawZone((g_showArea==1),iRGB);
            }
            /////////////////////////////////////////////

            /*Background setting with sampling over 100 frames. Background mask
             *restricts pixels with less deviation than parameter set in "Parametros"
             *window divided by 10000. It also accumulates valid depth valid pixels.*/

            if(BACK->proc((double)g_var/100000,nue,mask,Base,maskBack,iRGB,letra))
            {
                g_showMask=1;
                g_showFondo=1;
            }
            ///////////////////////////////////////////////////////////

            /*Calibration for valid zone. This reduces noise by selecting the interaction zone.
             *It also calibrates valid range for the output coordinates of the blobs.*/
            if(CAL->zoneCal(iRGB,maskZona,maskBack,letra,g_espacio))
            {
                g_showMask=1;
                if(g_plano==1){
                VP->calcPlano(nue,maskZona,mask);
                ////VP->calcPlano(Base,maskZona,maskBack);
                VP->hacerPlano(plano,maskZona);
                g_showPlano=1;}
            }
            ///////////////////////////////////////////////////////////////////////
            ////ZONE ENLARGMENT AND PLANE MOVER FOR PLANE METHOD TO USE EVERY POINT////
            if(g_agrandar)
            {
                VP->SetPl(2,(double)(VP->GetPl(3)*((double)g_movPlano/1000-1.0)));
                CAL->enlargeMask(maskZona,g_espacio);
                g_agrandar=false;
                g_showPlano=1;
                cvSetZero(plano);
                VP->hacerPlano(plano,maskZona,g_fisheye,(double)g_k/10000000);
            }
            //////////////////////////////////////////////////////////
            /*Point calibration.*/

            CAL->pointCal(iRGB,letra,"Tracker");
            ///////////////////////////////////////////////////////////////////////
            /*This part calculates the difference between background (or the simulated plane) and new depth image
             *and normalizes the difference according to depth. Then a min and max threeshold
             *is used to select pixels near the interaction zone. */

            cvSetZero(sal0);
            if(g_plano==1)
            {
                //g_delta=50;
                if(g_showPlano==1)
                {
                    //cvShowImage("Plano",plano);
                    //cvShowImage("Mask",maskZona);
                    g_showPlano=0;
                }
                VP->distPlano(nue,sal,g_mod,g_neg);
                //cvSub(plano,nue,sal);
            }
            ///////////////////////
            else if(g_threshold==0)
            {
                //g_delta=50;
                cvAnd(mask,maskBack,mask);
                if(g_mod==1)
                {
                    if(g_neg==0){
                    cvAbsDiff(Base,nue,sal0);}
                    else{
                    cvSub(nue,Base,sal0);
                    }
                }
                else
                {
                    cvSub(Base,nue,sal0);
                }
                cvDiv(sal0,Base,sal);
            }
            else
            {
                if(g_showPlano==1)
                {
                    //cvShowImage("Mask",maskZona);
                    g_showPlano=0;
                }
                cvZero(sal);
                cvAnd(maskZona,mask,mask);
                cvCopy(nue,sal,mask);
                //g_delta=100;
            }
            /////////////////////
            cvThreshold(sal,sal,(double)g_min/10000,1.0,CV_THRESH_TOZERO);
            cvThreshold(sal,sal,(double)g_max/10000,1.0,CV_THRESH_TOZERO_INV);
            cvConvertScale(sal,paBlobs,255*g_mult);
            ///////////////////////////////////////////////////////////////////////

            /*This part prepares the selected pixels for blob detection by selecting only the valid values and
             *then procedes to calculate the blobs. Then a min area filter is performed on the blobs.*/
            hacer0bordes(paBlobs);
            cvThreshold(paBlobs,paBlobs,g_thre,255,CV_THRESH_BINARY);

            if(g_plano==1)
            {
                cvAnd(maskZona,paBlobs,paBlobs);
            }
            else
            {
                cvAnd(mask,paBlobs,paBlobs);
            }



            if(g_showPaBlobs==1 && !CAL->occupied() && !BACK->contVal())
            {
				// @Gawhary Copy To Blobs image to container
				cvSetImageROI(iRGB_paBlobs, cvRect(iRGB->width + g_imageDisplayMargin,
					0,paBlobs->width,paBlobs->height) );
				// ToDo: convert image channels
				//cvCopy(paBlobs,iRGB_paBlobs,NULL);
				cvCvtColor(paBlobs, iRGB_paBlobs, CV_GRAY2RGB);
				cvResetImageROI(iRGB_paBlobs);
				cvShowImage("Tracker", iRGB_paBlobs);
            }
            blobs=0;
            cvClearMemStorage( g_storage );
            cvFindContours(paBlobs, g_storage, &blobs ,sizeof(CvContour),CV_RETR_LIST,CV_CHAIN_APPROX_NONE);
            ///////////////////////////////////////////////////////////////////////

            /*This part makes the correspondance between old blobs and new ones (though distance) and
             *adds new ones, removes old ones not related to new ones and updates new blobs related
             *to old ones. Each part adds to the final TUIO message.*/
            int nBlobsFound=0;
            if(!BM->noBlobs(blobs,server,&time,(g_outInfo==1),g_asTouch==1,g_mouse==1))
            {
                nBlobsFound=BM->blobRemap(blobs,server,&time,g_minBlob,iRGB,CAL,letra,(g_XY!=0),(g_XF!=0),(g_YF!=0),(g_showRGB==1 && g_showBlobs==1),g_mouse==1,nue,(g_outInfo==1),g_asTouch==1);
            }
            ////////////////////////////////////////////////////////////////////////////////
            /*This part is for auto calibration of min and max distance*/
            if(g_auto==1){
                g_auto2=1;
                g_auto=0;
                g_autoReps=0;
                g_framesAuto=0;
            }
            if(g_auto2==1 && g_autoReps==0){
                cout << "AUTO CALIBRATION STARTED" << endl;
                g_autoReps++;
                g_framesAuto=0;
                setTrackbarPos("Min Dist.","Tracker", 0);
                setTrackbarPos("Max Dist.","Tracker", g_delta);
            }
            else if(g_auto2==1 && nBlobsFound==2){
                g_framesAuto++;
                if(g_framesAuto>10){
                    g_auto2=0;
                    g_framesAuto=0;
                    g_autoReps=0;
                    cout << "Auto calibration done." << endl;
                }
                g_autoReps++;
            }
            else if(g_auto2==1){
                g_framesAuto=0;
                int minA=g_min;
                if((minA+10+g_delta)<10000){
                setTrackbarPos("Min Dist.", "Tracker", (minA+20));
                setTrackbarPos("Max Dist.", "Tracker", (minA+20+g_delta));}
                g_autoReps++;
            }
            if(g_autoReps>500){
                g_auto2=0;
                g_framesAuto=0;
                g_autoReps=0;
                cout << "Auto calibration could not be performed... \n Try adjusting other parameters first." << endl;
            }
            ///////////////////////////////////////////////////////////////////////////////////////////////

            if(g_showRGB==1)
            {
				if(g_showHelp==1 && !CAL->occupied() && !BACK->contVal())
                {
                 //cvDisplayOverlay("Tracker",help , 100);
                }
				
				// @Gawhary Copy iRGB image to container
				cvSetImageROI(iRGB_paBlobs, cvRect(0, 0,iRGB->width,iRGB->height) );
				cvCopy(iRGB,iRGB_paBlobs,NULL);
				cvResetImageROI(iRGB_paBlobs);

				cvShowImage("Tracker", iRGB_paBlobs);
            }
            if(g_showFondo==1)
            {
                //cvShowImage( "Background", Base);
                g_showFondo=0;
            }
            if(g_showMask==1 && g_plano==0)
            {
                //cvShowImage("Mask", maskBack);
                g_showMask=0;
            }
            if(timing==99)
            {
                fin=clock();
#ifndef WIN32
                cout << "FPS calculated : " << (double)100000000/(double)(fin-inicio) << endl;
#else
				cout << "FPS calculated : " << (double)100000/(double)(fin-inicio) << endl;
#endif
                timing=0;
                inicio=clock();
            }
            else
            {
                timing++;
            }
            //////////////////////////////////////////////////////////////////////////////////////////////
        }
        int keypressed=cvWaitKey( 1 );
        if( keypressed == 27 )
        {
            break;
        }
        else if(keypressed == 102)
        {
            setWindowProperty("Tracker",CV_WND_PROP_FULLSCREEN, CV_WINDOW_NORMAL);
            CAL->abortCal();
        }
    }

    delete letra;
    delete server;
    delete CAL;
    delete VP;
    delete BM;
    delete BACK;
    cvDestroyWindow("Tracker");
    //cvDestroyWindow("To Blobs");
    //cvDestroyWindow("Background");
    //cvDestroyWindow("Mask");
    //cvDestroyWindow("Plano");
    return 0;
}
