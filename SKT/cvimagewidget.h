#ifndef CVIMAGEWIDGET_H
#define CVIMAGEWIDGET_H

#include <QWidget>
#include <QImage>
#include <QPainter>
#include <cv.h>

#include "HiResTimer.h"

class CvImageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CvImageWidget(QWidget *parent = 0);

    QSize sizeHint() const;
    QSize minimumSizeHint() const;

public slots:

    void showImage(const cv::Mat& image);
    void showImage(IplImage *image);

protected:
    void paintEvent(QPaintEvent* /*event*/);

    QImage _qimage;
    cv::Mat _tmp;
    CHiResTimer timer;
};

#endif // CVIMAGEWIDGET_H
