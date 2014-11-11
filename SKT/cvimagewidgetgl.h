#ifndef CVIMAGEWIDGETGL_H
#define CVIMAGEWIDGETGL_H

#include <QPixmap>
#include <QLabel>
#include <QtOpenGL\QGLWidget>
#include <QVBoxLayout>
#include <QImage>
#include <QDebug>
#include <cv.h>
#include "HiResTimer.h"
#include "cv.h"
class CvImageWidgetGL : public QGLWidget
{
    Q_OBJECT

public:
    explicit CvImageWidgetGL (QWidget *parent = 0);
	~CvImageWidgetGL();
    QPixmap toPixmap(IplImage *img);
    void putFrame(IplImage *img, bool color = true);
    void putFrame(cv::Mat img, bool col = true);
protected:
    virtual void paintGL();
    virtual void initializeGL();
    virtual void resizeGL(int width, int height);
private:
    CHiResTimer timer;
    cv::Mat image;
    bool useglfix;
    bool color;
    float widthI,heightI;
    bool isResized;

}; 

#endif //CVIMAGEWIDGETGL_H
