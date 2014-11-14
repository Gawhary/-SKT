#include "mainwindow.h"

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
#include "mainwindow.h"

#ifndef WIN32
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#endif

#include <virtualplane.h>
#include <calibrator.h>
#include <blobmapper.h>
#include <backmethod.h>
#include <mouseactions.h>
#include <QTimer>
using namespace cv;
using namespace TUIO;
using namespace std;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_exit(false)
{
    m_method=METHOD_BACKGROUND;
    m_CAL=NULL;
    ui.setupUi(this);
    ui.distSlider->setMaximum(1000);
    ui.distSlider->setMinimum(0);
    ui.distSlider->setUpperValue(230);
    ui.distSlider->setLowerValue(55);
    ui.MinSlider->setValue(80);
    ui.image_depthSlider->setValue(100);
    QTimer::singleShot(0,this, SLOT(run()));

}

MainWindow::~MainWindow()
{

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    m_exit = true;
    QApplication::exit(0);
}


int g_imageDisplayMargin=10; // @Gawhary: Margins between multi-images in one window
//int g_min=55;         //Min threeshold for selection of pixels near interaction zone. Divided by 10000.
//int g_max=230;         //Max threeshold for selection of pixels near interaction zone. Divided by 10000.
int g_maxBlob=50;       //Max blob area for filter. NOT IMPLEMENTED
int g_thre=0;           //Threeshold for last image. Almost not used...
int g_mult=1;           //Multiplier for scale adjust of images. Not used...
int g_var=300;         //Minimum variance for pixels in the background allowed. Divided by 100000 after.
bool g_save=true;
int g_calZona=0;        //Zone calibration toggle.
int g_showFondo=0;
int g_showMask=0;
int g_showPlano=0;
int g_espacio=100;
bool g_agrandar=false;
const char* g_host;
int g_port=3333;
int g_maxNumBlobs=100;
int g_numPoints=5;
int g_auto2=0;
int g_framesAuto=0;
int g_movPlano=2000;
int g_escala=6000;
int g_fisheye=0;
int g_k=0;
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
void MainWindow::affineWarperAndMixer(CvMat* warp,IplImage* iRGB,IplImage* aux1,IplImage* aux2,float alpha,IplImage* gray)
{
    //Deprecated, now OpenCV does the fit.
    //cvWarpAffine(iRGB,aux2,warp);
    cvCopy(iRGB,aux2);
    cvConvertImage(gray,aux1,CV_GRAY2RGB);
    cvAddWeighted(aux2,alpha,aux1,(1.0-alpha),0.0,iRGB);
}

/*hacer0bordes : Function to set the pixels in the border of the image to 0 for blobs to close*/
void MainWindow::hacer0bordes(IplImage* in)
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
std::string MainWindow::loadParameters(bool all)
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
            int _min = ui.distSlider->lowerValue();
            int _max = ui.distSlider->upperValue();
            read=blobDet->QueryIntAttribute("Min-Distance",&_min);
            read=blobDet->QueryIntAttribute("Max-Distance",&_max);
            int _minBlob = ui.MinSlider->value();
            read=blobDet->QueryIntAttribute("Min-Blob-Area",&_minBlob);
            ui.MinSlider->setValue(_minBlob);
            read=blobDet->QueryIntAttribute("Background-Accepted-Variance",&g_var);
            read=blobDet->QueryIntAttribute("Maximum-Number-Of-Blobs",&g_maxNumBlobs);
            int _alpha = ui.image_depthSlider->value();
            read=affineVal->QueryIntAttribute("Alpha",&_alpha);
            ui.image_depthSlider->setValue(_alpha);
            read=tuioInfo->QueryIntAttribute("Port",&g_port);
            read=pointCal->QueryIntAttribute("Number-Of-Calibration-Points",&g_numPoints);
            cout << "Parameter file: settings.xml - Loaded!" << endl;

            ui.distSlider->setLowerValue(_min);
            ui.distSlider->setUpperValue(_max);
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
void MainWindow::saveParameters(int a,void* param)
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
    blobDet->SetAttribute("Min-Distance", ui.distSlider->lowerValue());
    blobDet->SetAttribute("Max-Distance", ui.distSlider->upperValue());
    blobDet->SetAttribute("Min-Blob-Area", ui.MinSlider->value());
    blobDet->SetAttribute("Background-Accepted-Variance", g_var);
    blobDet->SetAttribute("Maximum-Number-Of-Blobs",g_maxNumBlobs);
    affineVal->SetAttribute("Alpha", ui.image_depthSlider->value());
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
void MainWindow::callBackPoint(int a,void* param)
{
    calibrator *MyCal=(calibrator*) param;
    MyCal->pointCalBegin("Tracker");
}
void MainWindow::callBackAgrandar(int a)
{
    g_agrandar=true;
}
void MainWindow::callBackSetValue(int a,void* param)
{
    int* valueToSet=(int*)param;
    *valueToSet=a;
}
void MainWindow::callBackButton(int a,void* param)
{
    int* valueToSet=(int*)param;
    *valueToSet=1;
}
void MainWindow::callBackButton0(int a,void* param)
{
    int* valueToSet=(int*)param;
    *valueToSet=0;
}
void MainWindow::MouseEvWrapper(int event,int x,int y,int flag,void* param)
{
    calibrator *MyCal=(calibrator*) param;
    MyCal->MouseEv(event,x,y,flag);
}

int MainWindow::run(){
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
    m_CAL=new calibrator(g_numPoints,640,480,5,5);
    blobmapper* BM=new blobmapper(g_maxNumBlobs);
    backmethod* BACK=new backmethod();

    cout << "Screen : " << m_CAL->GetWidth() << ","<<  m_CAL->GetHeight() << endl;
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
        delete m_CAL;
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
        delete m_CAL;
        delete VP;
        delete BM;
        delete BACK;
        return -1;
    }

    /*Parameters for processing*/
    //cvCreateButton("Set Zone",callBackButton,&g_calZona,CV_PUSH_BUTTON);
    //cvCreateButton("Save Parameters",saveParameters,NULL,CV_PUSH_BUTTON);
    //cvCreateButton("Background Method",NULL,NULL,CV_RADIOBOX,1);
    //cvCreateButton("Plane Method",callBackSetValue,&g_plano,CV_RADIOBOX,0);
    //cvCreateButton("Threshold ",callBackSetValue,&g_threshold,CV_RADIOBOX,0);
//    cvCreateTrackbar("Min Dist.","Tracker",&g_min,10000);
//    cvCreateTrackbar("Max Dist.","Tracker",&g_max,10000);
    //    cvCreateButton("Start Point Calibration",callBackPoint,(void*) CAL,CV_PUSH_BUTTON);
    //	//createButton("FishEye",callBackSetValue,&g_fisheye,CV_CHECKBOX,0);
    //	//cvCreateTrackbar("K",NULL,&g_k,100000,callBackAgrandar);
//    cvSetMouseCallback("Tracker",MouseEvWrapper,(void*) CAL);


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

    /////////////////////////////////////////////////////////////////


    g_showMask=0;
    g_showFondo=0;
    inicio=clock();
    while(!m_exit)
    {
        ////////////////////////////////////////

        //Zone calibration starting point.
        if(g_calZona==1)
        {
            g_calZona=0;
            m_CAL->zoneCalBegin();
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


            capture.retrieve(imgRGB,CV_CAP_OPENNI_BGR_IMAGE);
            *iRGB=imgRGB;
            affineWarperAndMixer(warpAffin,iRGB,iRGBaux,iRGBaux2,(float)ui.image_depthSlider->value()/100.0,nue);
            m_CAL->drawZone((ui.showPlaneCheckBox->isChecked()),iRGB);

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
            if(m_CAL->zoneCal(iRGB,maskZona,maskBack,letra,g_espacio))
            {
                g_showMask=1;
                if(m_method == METHOD_PLANE){
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
                m_CAL->enlargeMask(maskZona,g_espacio);
                g_agrandar=false;
                g_showPlano=1;
                cvSetZero(plano);
                VP->hacerPlano(plano,maskZona,g_fisheye,(double)g_k/10000000);
            }
            //////////////////////////////////////////////////////////
            /*Point calibration.*/

//            CAL->pointCal(iRGB,letra,"Tracker");
            ///////////////////////////////////////////////////////////////////////
            /*This part calculates the difference between background (or the simulated plane) and new depth image
             *and normalizes the difference according to depth. Then a min and max threeshold
             *is used to select pixels near the interaction zone. */

            cvSetZero(sal0);
            if(m_method == METHOD_PLANE)
            {
                if(m_method = METHOD_PLANE)
                {
                    //cvShowImage("Plano",plano);
                    //cvShowImage("Mask",maskZona);
                    g_showPlano=0;
                }
                VP->distPlano(nue,sal,0,0);
                //cvSub(plano,nue,sal);
            }
            ///////////////////////
            else if(m_method == METHOD_BACKGROUND )
            {
                cvAnd(mask,maskBack,mask);
                cvSub(Base,nue,sal0);
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
            }
            /////////////////////
            cvThreshold(sal,sal,(double)ui.distSlider->lowerValue()/10000.0,1.0,CV_THRESH_TOZERO);
            cvThreshold(sal,sal,(double)ui.distSlider->upperValue()/10000.0,1.0,CV_THRESH_TOZERO_INV);
            cvConvertScale(sal,paBlobs,255*g_mult);
            ///////////////////////////////////////////////////////////////////////

            /*This part prepares the selected pixels for blob detection by selecting only the valid values and
             *then procedes to calculate the blobs. Then a min area filter is performed on the blobs.*/
            hacer0bordes(paBlobs);
            cvThreshold(paBlobs,paBlobs,g_thre,255,CV_THRESH_BINARY);

            if(m_method == METHOD_PLANE)
            {
                cvAnd(maskZona,paBlobs,paBlobs);
            }
            else
            {
                cvAnd(mask,paBlobs,paBlobs);
            }



            if( !m_CAL->occupied() && !BACK->contVal())
            {
                ui.toBlobsImage->showImage(paBlobs, false);
            }
            blobs=0;
            cvClearMemStorage( g_storage );
            cvFindContours(paBlobs, g_storage, &blobs ,sizeof(CvContour),CV_RETR_LIST,CV_CHAIN_APPROX_NONE);
            ///////////////////////////////////////////////////////////////////////

            /*This part makes the correspondance between old blobs and new ones (though distance) and
             *adds new ones, removes old ones not related to new ones and updates new blobs related
             *to old ones. Each part adds to the final TUIO message.*/
            int nBlobsFound=0;
            if(!BM->noBlobs(blobs,server,&time,false,false,false))
            {
                nBlobsFound=BM->blobRemap(blobs,server,&time,ui.MinSlider->value(),iRGB,
                                          m_CAL,letra,false,
                                          ui.flipCheckBox->isChecked(),
                                          ui.flipCheckBox->isChecked(),
                                          ui.showPlaneCheckBox->isChecked(),
                                          false,nue,false,false);
            }

            ui.rgbImage->showImage(iRGB);

            if(g_showFondo==1)
            {
                //cvShowImage( "Background", Base);
                g_showFondo=0;
            }
            if(g_showMask==1 && m_method != METHOD_PLANE)
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

        QCoreApplication::processEvents();
    }


    delete letra;
    delete server;
    delete m_CAL;
    delete VP;
    delete BM;
    delete BACK;
//    cvDestroyWindow("Tracker");
    //cvDestroyWindow("To Blobs");
    //cvDestroyWindow("Background");
    //cvDestroyWindow("Mask");
    //cvDestroyWindow("Plano");
    return 0;
}


void MainWindow::on_setPlaneButton_clicked()
{
    g_calZona = 1;
    m_method = METHOD_PLANE;
}

void MainWindow::on_maskSlider_valueChanged(int value)
{
    g_espacio = value;
    callBackAgrandar(value);

}

void MainWindow::on_planeSlider_valueChanged(int value)
{
    g_movPlano = value;
    callBackAgrandar(value);
}

void MainWindow::on_saveButton_clicked()
{
    saveParameters(-1, NULL);
}

void MainWindow::on_calibrationButton_clicked()
{
    if(m_CAL)
        callBackPoint(-1, (void*) m_CAL);
}
