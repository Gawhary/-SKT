#include "cvimagewidgetgl.h"
#include <QKeyEvent>
#include <QApplication>
#include <QDesktopWidget>
#include <QScreen>
#include <qdebug.h>
using namespace cv;

// Constructor
CvImageWidgetGL::CvImageWidgetGL(QWidget *parent)
    : QGLWidget(parent)
{

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

void CvImageWidgetGL::showImage(IplImage *pImg, bool col){


    cv::Mat img(pImg,false);
    showImage(img, col);

}

void CvImageWidgetGL::showImage(Mat img, bool col)
{

    m_imageSize.setHeight(img.rows);
    m_imageSize.setWidth(img.cols);
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

QPoint CvImageWidgetGL::mapToImage(const QPoint& pointToMap)
{
	double ww = this->width();
	double wh = this->height();
    if((ww == 0 || wh == 0))
    {
        return QPoint();
        qDebug() << "################################";
    }
    QPoint mapped(
                (pointToMap.x() / ww)*m_imageSize.width(),
                (pointToMap.y() / wh)*m_imageSize.height());
    return mapped;
}

void CvImageDockableWidgetGL::setFullscreen(bool fullscreen)
{
//    return; // ToDo: delete
    static Qt::WindowFlags origWindowFlags;
    static QSize orgSize;
    static QWidget *orgParent;
    if(fullscreen)
    {
//        m_imageWidget->hide();
        origWindowFlags = m_imageWidget->windowFlags();
        orgSize = m_imageWidget->size();
        m_layout->removeWidget(m_imageWidget);
        m_imageWidget->setParent(0);
//         m_imageWidget->setWindowFlags( Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
        m_imageWidget->showFullScreen();
        QApplication::setActiveWindow(m_imageWidget);
    }
    else
    {
//        m_imageWidget->hide();
        m_imageWidget->setParent(orgParent);
        m_layout->addWidget(m_imageWidget);
        m_imageWidget->resize(orgSize);
//        m_imageWidget->overrideWindowFlags(origWindowFlags);
        m_imageWidget->showNormal();
    }
}

CvImageDockableWidgetGL::CvImageDockableWidgetGL(QWidget *parent)
{
    m_layout = new QGridLayout(this);
    m_layout->setSpacing(0);
    m_layout->setMargin(0);
    m_imageWidget = new CvImageWidgetGL(this);
    m_layout->addWidget(m_imageWidget);
    setLayout(m_layout);
    m_imageWidget->installEventFilter(this);
}

bool CvImageDockableWidgetGL::isFullScreen(){
    return m_isFullscreen;
}

QPixmap CvImageDockableWidgetGL::toPixmap(IplImage *img){return m_imageWidget->toPixmap(img);}

void CvImageDockableWidgetGL::showImage(IplImage *pImg, bool color){m_imageWidget->showImage(pImg, color);}

void CvImageDockableWidgetGL::showImage(Mat img, bool col){m_imageWidget->showImage(img, col);}

QPoint CvImageDockableWidgetGL::mapToImage(const QPoint &pointToMap){return m_imageWidget->mapToImage(pointToMap);}



QPixmap CvImageWidgetGL::toPixmap(IplImage *cvimage) {

    unsigned char* qImageBuffer = (unsigned char*)(cvimage->imageData);
    QImage tempImage((const unsigned char*)qImageBuffer, cvimage->width,
                     cvimage->height, cvimage->widthStep, QImage::Format_RGB888);
    QPixmap tempPixmap;
    tempPixmap.convertFromImage( (tempImage).rgbSwapped() );
    return tempPixmap;
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


    if(color)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.cols, image.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, image.data);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, image.cols, image.rows, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, image.data);

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

bool CvImageDockableWidgetGL::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == m_imageWidget && event->type() == QEvent::KeyPress){
        if( static_cast<QKeyEvent*>(event)->key() == Qt::Key_Escape){
            if(m_isFullscreen){
                setFullscreen(false);
                return true;
            }
        }
    }
    return false;
}


