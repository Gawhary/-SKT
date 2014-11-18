#ifndef CVIMAGEWIDGETGL_H
#define CVIMAGEWIDGETGL_H

#include <QPixmap>
#include <QLabel>
#include <QGLWidget>
#include <QVBoxLayout>
#include <QImage>
#include <QDebug>
#include <cv.h>
#include "cv.h"
class CvImageWidgetGL : public QGLWidget
{
    Q_OBJECT

public:
    explicit CvImageWidgetGL (QWidget *parent = 0);
    ~CvImageWidgetGL();
    QPixmap toPixmap(IplImage *);
    void showImage(IplImage *, bool color = true);
    void showImage(cv::Mat img, bool col = true);
    QPoint mapToImage(const QPoint &pointToMap);
    void setFullscreen(bool fullscreen = true);
    bool isFullScreen();

protected:
    virtual void paintGL();
    virtual void initializeGL();
    virtual void resizeGL(int width, int height);
    void keyPressEvent(QKeyEvent *event);
private:
    cv::Mat image;
    bool useglfix;
    bool color;
    float widthI,heightI;
    bool isResized;
    bool m_isFullscreen;
    QSize m_imageSize;
}; 

#endif //CVIMAGEWIDGETGL_H
