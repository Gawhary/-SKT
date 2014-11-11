#include "cvimagewidgetgl.h"

#include <qdebug.h>
using namespace cv;

// Constructor
CvImageWidgetGL::CvImageWidgetGL(QWidget *parent)
    : QGLWidget(parent)
{
    timer.Init();

    image.create(480,640,CV_8UC3);
    color=true;
    initializeGL();
    useglfix=false;
    QGLFormat::OpenGLVersionFlags flags = QGLFormat::openGLVersionFlags();
    if(!(flags & QGLFormat::OpenGL_Version_2_0))
    {
        useglfix=true;
        image.create(1024,1024,CV_8UC3);
    }
}

CvImageWidgetGL::~CvImageWidgetGL()
{

}
void CvImageWidgetGL::initializeGL()
{
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
}

void CvImageWidgetGL::putFrame(IplImage *pImg, bool col){


    cv::Mat img(pImg,false);
    putFrame(img, col);

}

void CvImageWidgetGL::putFrame(Mat img, bool col)
{
    timer.GetElapsedMilliSeconds();
    //cout << "up 0" << endl;
    color=col;
    if(useglfix)
    {
       if(color)
           image.create(1024,1024,CV_8UC3);
       else
           image.create(1024,1024,CV_8UC1);
       // Define roi area (it has small image dimensions).
       Rect roi(0,0,img.cols,img.rows);

       // Take a sub-view of the large image
       Mat subView = image(roi);

       // Copy contents of the small image to large
       img.copyTo(subView);
       //img.copyTo(m_image.colRange(0,img.cols).rowRange(0,img.rows));
       widthI=img.cols;
       heightI=img.rows;
    }
    else
       image=img;
    //cout << "up 1" << endl;
    updateGL();
    //cout << "up 2" << endl;

}



QPixmap CvImageWidgetGL::toPixmap(IplImage *cvimage) {

    timer.GetElapsedMilliSeconds();
    unsigned char* qImageBuffer = (unsigned char*)(cvimage->imageData);
    QImage tempImage((const unsigned char*)qImageBuffer, cvimage->width, cvimage->height, cvimage->widthStep, QImage::Format_RGB888);
    QPixmap tempPixmap;
    tempPixmap.convertFromImage( (tempImage).rgbSwapped() );

    double et = timer.GetElapsedMilliSeconds();
    qDebug() << "Converting time: " << et;
    if(et)
        qDebug() << "FPS: " << 1000.0/et;

    return tempPixmap;

//    int cvIndex, cvLineStart;

//    switch (cvimage->depth) {
//        case IPL_DEPTH_8U:
//            switch (cvimage->nChannels) {
//                case 3:
//                    if ( (cvimage->width != m_image.width()) || (cvimage->height != m_image.height()) ) {
//                        QImage temp(cvimage->width, cvimage->height, QImage::Format_RGB32);
//                        m_image = temp;
//                    }
//                    cvIndex = 0; cvLineStart = 0;
//                    for (int y = 0; y < cvimage->height; y++) {
//                        unsigned char red,green,blue;
//                        cvIndex = cvLineStart;
//                        for (int x = 0; x < cvimage->width; x++) {
//                            red = cvimage->imageData[cvIndex+2];
//                            green = cvimage->imageData[cvIndex+1];
//                            blue = cvimage->imageData[cvIndex+0];
                            
//                            m_image.setPixel(x,y,qRgb(red, green, blue));
//                            cvIndex += 3;
//                        }
//                        cvLineStart += cvimage->widthStep;
//                    }
//                    break;
//                default:
//                    qWarning("This number of channels is not supported\n");
//                    break;
//            }
//            break;
//        default:
//            qWarning("This type of IplImage is not implemented in QOpenCVWidget\n");
//            break;
//    }

//    QPixmap tempPixmap = QPixmap::fromImage(m_image);

//    double et = timer.GetElapsedMilliSeconds();
//    qDebug() << "Converting time: " << et;
//    if(et)
//        qDebug() << "FPS: " << 1000.0/et;


//    return tempPixmap;
}


void CvImageWidgetGL::paintGL()
{
//    if(image.empty())
//        return QGLWidget::paintGL();

    glClear (GL_COLOR_BUFFER_BIT);		//clear the gl buffers
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S , GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

//        timer.GetElapsedMilliSeconds();

    if(color)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.cols, image.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, image.data);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, image.cols, image.rows, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, image.data);


//        double et = timer.GetElapsedMilliSeconds();
//        qDebug() << "Converting time: " << et;
//        if(et)
//            qDebug() << "FPS: " << 1000.0/et;


    GLfloat maxX=1.0,maxY=1.0;
    if(useglfix){
        maxX=widthI/1024;
        maxY=heightI/1024;
    }

    glBegin(GL_QUADS);

    // upper left
    glTexCoord2f(0, 0);
    glVertex2f(-1.0, 1.0);
    // upper right
    glTexCoord2f(maxX, 0);
    glVertex2f(1.0, 1.0);
    // bottom right
    glTexCoord2f(maxX, maxY);
    glVertex2f(1.0, -1.0);
    // bottom left
    glTexCoord2f(0, maxY);
    glVertex2f(-1.0, -1.0);

    glEnd();
}

void CvImageWidgetGL::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-width, width, height, -height, -1.0, 1.0);
    isResized=true;
}

