#ifndef CVIMAGEWIDGETGL_H
#define CVIMAGEWIDGETGL_H

#include <QPixmap>
#include <QLabel>
#include <QGLWidget>
#include <QVBoxLayout>
#include <QImage>
#include <QLayout>
#include <QDebug>
#include <cv.h>
#include "cv.h"

class CvImageWidgetGL;

class CvImageDockableWidgetGL : public QWidget{
    Q_OBJECT

public:
    explicit CvImageDockableWidgetGL(QWidget *parent = 0);

    bool isFullScreen();
    QPixmap toPixmap(IplImage *);
    void showImage(IplImage *pImg, bool color = true);
    void showImage(cv::Mat img, bool col = true);
    QPoint mapToImage(const QPoint &pointToMap);

public slots:
    void setFullscreen(bool fullscreen = true);

protected:
    virtual bool eventFilter(QObject *, QEvent *event);
private:
    CvImageWidgetGL *m_imageWidget;
    bool m_isFullscreen;
    QLayout *m_layout;
};

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

protected:
    virtual void paintGL();
    virtual void initializeGL();
    virtual void resizeGL(int width, int height);
private:
    cv::Mat image;
    bool useglfix;
    bool color;
    float widthI,heightI;
    bool isResized;
    QSize m_imageSize;
}; 

#endif //CVIMAGEWIDGETGL_H
