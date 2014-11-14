#include "cvimagewidget.h"
#include <qdebug.h>


CvImageWidget::CvImageWidget(QWidget *parent) : QWidget(parent) {
}

QSize CvImageWidget::sizeHint() const { return _qimage.size(); }

QSize CvImageWidget::minimumSizeHint() const { return _qimage.size(); }

void CvImageWidget::showImage(const cv::Mat &image) {
    // Convert the image to the RGB888 format
    switch (image.type()) {
    case CV_8UC1:
        cvtColor(image, _tmp, CV_GRAY2RGB);
        break;
    case CV_8UC3:
        cvtColor(image, _tmp, CV_BGR2RGB);
        break;
    }

    // QImage needs the data to be stored continuously in memory
    assert(_tmp.isContinuous());
    // Assign OpenCV's image buffer to the QImage. Note that the bytesPerLine parameter
    // (http://qt-project.org/doc/qt-4.8/qimage.html#QImage-6) is 3*width because each pixel
    // has three bytes.
    _qimage = QImage(_tmp.data, _tmp.cols, _tmp.rows, _tmp.cols*3, QImage::Format_RGB888);

    this->setFixedSize(image.cols, image.rows);

    repaint();
}

void CvImageWidget::showImage(IplImage *image)
{
    showImage(cv::Mat(image, false));
}

void CvImageWidget::paintEvent(QPaintEvent *) {
    // Display the image
    QPainter painter(this);
    painter.drawImage(QPoint(0,0), _qimage);
    painter.end();
}
