#include "mainwindow.h"

#include <stdlib.h>
#include <ctime>
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <math.h>
#include <TuioServer.h>
#include <TuioCursor.h>
#include <TuioTime.h>
#include <TuioClient.h>
#include <tinyxml.h>
#include <tinystr.h>
#include <string.h>
#include <QMouseEvent>
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
#include <QDebug>

using namespace cv;
using namespace TUIO;
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_exit(false)
{
    m_maxBlob=50;
    m_thre=0;
    m_mult=1;
    m_var=300;
    m_save=true;
    m_calZona=0;
    m_showFondo=0;
    m_showMask=0;
    m_showPlano=0;
    m_agrandar=false;
    m_host;
    m_port=3333;
    m_maxNumBlobs=100;
    m_numPoints=5;
    m_auto2=0;
    m_framesAuto=0;
    m_escala=6000;
    m_fisheye=0;
    m_k=0;
    m_method=METHOD_BACKGROUND;
    m_CAL=NULL;
    m_pressedButton = Qt::NoButton;
    ui.setupUi(this);
    ui.distSlider->setMaximum(1500);
    ui.distSlider->setUpperValue(230);
    ui.distSlider->setLowerValue(55);
    ui.MinSlider->setMaximum(10000);
    ui.MinSlider->setValue(80);
    ui.planeSlider->setMaximum(4000);
    ui.planeSlider->setValue(2000);
    ui.maskSlider->setMaximum(1000);
    ui.maskSlider->setValue(100);
    ui.image_depthSlider->setMaximum(100);
    ui.image_depthSlider->setValue(100);

    ui.rgbImage->installEventFilter(this); // to capture mouse events

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

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if(object != ui.rgbImage)
        return false;
    if( event->type() ==  QEvent::KeyPress){
        if( static_cast<QKeyEvent*>(event)->key() == Qt::Key_Escape){
            if(ui.rgbImage->isFullScreen())
                ui.rgbImage->setFullscreen(false);
            if(m_CAL) m_CAL->abortCal();
            return true;

        }
        return false;
    }
    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
    //        map x and y to image
//    qDebug() << "Mouse Pos: " << mouseEvent->pos();
    QPoint mapped = ui.rgbImage->mapToImage(mouseEvent->pos());
    int cvEvent;

    if( event->type() == QEvent::MouseButtonRelease ||
            event->type() == QEvent::MouseButtonPress){
        switch (event->type()) {

        case QEvent::MouseButtonPress:
            m_pressedButton = mouseEvent->buttons();
            if(m_pressedButton&Qt::LeftButton)
                cvEvent = EVENT_LBUTTONDOWN;
            else if(m_pressedButton&Qt::RightButton)
                cvEvent = EVENT_RBUTTONDOWN;
            else
                return false;
                break;
        case QEvent::MouseButtonRelease:
            if(m_pressedButton&Qt::LeftButton)
                cvEvent = EVENT_LBUTTONUP;
            else if(m_pressedButton&Qt::RightButton)
                cvEvent = EVENT_RBUTTONUP;
            else
                return false;
            break;

            //        case QEvent::MouseMove:
            //            cvEvent = EVENT_MOUSEMOVE;
            //            break;
        default:
            return false;
        }
        // send event
        int flags = mouseEvent->buttons();
        if(m_CAL) m_CAL->MouseEv(cvEvent, mapped.x(), mapped.y(), flags);
//        qDebug() << "Mouse Event sent, Event: " << cvEvent;
        return true;
    }
    return false;

}

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
            read=blobDet->QueryIntAttribute("Background-Accepted-Variance",&m_var);
            read=blobDet->QueryIntAttribute("Maximum-Number-Of-Blobs",&m_maxNumBlobs);
            int _alpha = ui.image_depthSlider->value();
            read=affineVal->QueryIntAttribute("Alpha",&_alpha);
            ui.image_depthSlider->setValue(_alpha);
            read=tuioInfo->QueryIntAttribute("Port",&m_port);
            read=pointCal->QueryIntAttribute("Number-Of-Calibration-Points",&m_numPoints);
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
    blobDet->SetAttribute("Background-Accepted-Variance", m_var);
    blobDet->SetAttribute("Maximum-Number-Of-Blobs",m_maxNumBlobs);
    affineVal->SetAttribute("Alpha", ui.image_depthSlider->value());
    tuioInfo->SetAttribute("Host",loadParameters(false).c_str());
    tuioInfo->SetAttribute("Port",m_port);
    pointCal->SetAttribute("Number-Of-Calibration-Points",m_numPoints);
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

void MainWindow::MouseEvWrapper(int event,int x,int y,int flag,void* param)
{}

int MainWindow::run(){
    m_host=loadParameters(true).c_str();
    if(m_numPoints>10 || m_numPoints<2){
        m_numPoints=5;
        cout << "Invalid value for number of calibration points. Using 5." << endl;
    }
    if(m_maxNumBlobs>1000 || m_maxNumBlobs<0){
        cout << "Invalid value for max number of blobs. Using 50." << endl;
        m_maxNumBlobs=50;}
    cout << "Host : " << m_host << endl;
    cout << "Port : " << m_port << endl;

    TuioServer *server=new TuioServer(m_host,m_port);
    virtualplane *VP=new virtualplane();
    m_CAL=new calibrator(m_numPoints,640,480,5,5);
    blobmapper* BM=new blobmapper(m_maxNumBlobs);
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
    catch(...)
    {
        cout << endl << "Could not find any device." << endl;
        delete letra;
        delete server;
        delete m_CAL;m_CAL = NULL;
        delete VP;
        delete BM;
        delete BACK;
        QTimer::singleShot(1000, this, SLOT(run())); // try again later
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
        delete m_CAL;m_CAL = NULL;
        delete VP;
        delete BM;
        delete BACK;
        QTimer::singleShot(1000, this, SLOT(run())); // try again later
        return -1;
    }

    /*Several image definitions needed...*/

    Mat imgMap,maskMap,imgRGB;
    CvMat iMap;
    IplImage* nue=cvCreateImage(cvSize(640,480),IPL_DEPTH_32F,1);
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

    m_showMask=0;
    m_showFondo=0;
    m_inicio=clock();
    while(!m_exit)
    {

        //Zone calibration starting point.
        if(m_calZona==1)
        {
            m_calZona=0;
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

            if(BACK->proc((double)m_var/100000,nue,mask,Base,maskBack,iRGB,letra))
            {
                m_showMask=1;
                m_showFondo=1;
            }
            ///////////////////////////////////////////////////////////

            /*Calibration for valid zone. This reduces noise by selecting the interaction zone.
             *It also calibrates valid range for the output coordinates of the blobs.*/
            if(m_CAL->zoneCal(iRGB,maskZona,maskBack,letra,ui.maskSlider->value()))
            {
                m_showMask=1;
                if(m_method == METHOD_PLANE){
                    VP->calcPlano(nue,maskZona,mask);
                    ////VP->calcPlano(Base,maskZona,maskBack);
                    VP->hacerPlano(plano,maskZona);
                    m_showPlano=1;}
            }
            ///////////////////////////////////////////////////////////////////////
            ////ZONE ENLARGMENT AND PLANE MOVER FOR PLANE METHOD TO USE EVERY POINT////
            if(m_agrandar)
            {
                VP->SetPl(2,(double)(VP->GetPl(3)*((double)ui.planeSlider->value()/1000-1.0)));
                m_CAL->enlargeMask(maskZona,ui.maskSlider->value());
                m_agrandar=false;
                m_showPlano=1;
                cvSetZero(plano);
                VP->hacerPlano(plano,maskZona,m_fisheye,(double)m_k/10000000);
            }
            //////////////////////////////////////////////////////////
            /*Point calibration.*/

            m_CAL->pointCal(iRGB,letra,ui.rgbImage);
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
                    m_showPlano=0;
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
                if(m_showPlano==1)
                {
                    //cvShowImage("Mask",maskZona);
                    m_showPlano=0;
                }
                cvZero(sal);
                cvAnd(maskZona,mask,mask);
                cvCopy(nue,sal,mask);
            }
            /////////////////////
            cvThreshold(sal,sal,(double)ui.distSlider->lowerValue()/10000.0,1.0,CV_THRESH_TOZERO);
            cvThreshold(sal,sal,(double)ui.distSlider->upperValue()/10000.0,1.0,CV_THRESH_TOZERO_INV);
            cvConvertScale(sal,paBlobs,255*m_mult);
            ///////////////////////////////////////////////////////////////////////

            /*This part prepares the selected pixels for blob detection by selecting only the valid values and
             *then procedes to calculate the blobs. Then a min area filter is performed on the blobs.*/
            hacer0bordes(paBlobs);
            cvThreshold(paBlobs,paBlobs,m_thre,255,CV_THRESH_BINARY);

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

            if(m_showFondo==1)
            {
                //cvShowImage( "Background", Base);
                m_showFondo=0;
            }
            if(m_showMask==1 && m_method != METHOD_PLANE)
            {
                //cvShowImage("Mask", maskBack);
                m_showMask=0;
            }
            if(timing==99)
            {
                m_fin=clock();
#ifndef WIN32
                cout << "FPS calculated : " << (double)100000000/(double)(fin-inicio) << endl;
#else
                cout << "FPS calculated : " << (double)100000/(double)(m_fin-m_inicio) << endl;
#endif
                timing=0;
                m_inicio=clock();
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
    return 0;
}

void MainWindow::on_setPlaneButton_clicked()
{
    m_calZona = 1;
    m_method = METHOD_PLANE;
}

void MainWindow::on_maskSlider_valueChanged(int value)
{
    m_agrandar=true;

}

void MainWindow::on_planeSlider_valueChanged(int value)
{
    m_agrandar=true;
}

void MainWindow::on_saveButton_clicked()
{
    saveParameters(-1, NULL);
}

void MainWindow::on_calibrationButton_clicked()
{
    if(!m_CAL)
        return;
    //    ui.rgbImage->setFullscreen();
    m_CAL->pointCalBegin(ui.rgbImage);
}
