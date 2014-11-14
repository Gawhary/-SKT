#ifndef CVIMAGEWIDGET_H
#define CVIMAGEWIDGET_H

#include <QWidget>
#include <QImage>
#include <QPainter>
#include <cv.h>

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
};

#endif // CVIMAGEWIDGET_H
