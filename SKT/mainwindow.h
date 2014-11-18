#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <time.h>

#include <QMainWindow>
#include <QWidget>

#include "ui_mainwindow.h"
#include "calibrator.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    virtual void closeEvent(QCloseEvent * event);
    virtual bool eventFilter(QObject *object, QEvent *event);
private slots:
    int run();

    void on_setPlaneButton_clicked();

    void on_maskSlider_valueChanged(int value);

    void on_planeSlider_valueChanged(int value);

    void on_saveButton_clicked();

    void on_calibrationButton_clicked();

private:

    void affineWarperAndMixer(
            CvMat *warp, IplImage *iRGB, IplImage *aux1,
            IplImage *aux2, float alpha, IplImage *gray);
    void hacer0bordes(IplImage *in);
    std::string loadParameters(bool all);
    void saveParameters(int a, void *param);
    void MouseEvWrapper(int event, int x, int y, int flag, void *param);
    Ui::MainWindow ui;
    bool m_exit;
    calibrator* m_CAL;

    int m_maxBlob;       //Max blob area for filter. NOT IMPLEMENTED
    int m_thre;           //Threeshold for last image. Almost not used...
    int m_mult;           //Multiplier for scale adjust of images. Not used...
    int m_var;         //Minimum variance for pixels in the background allowed. Divided by 100000 after.
    bool m_save;
    int m_calZona;        //Zone calibration toggle.
    int m_showFondo;
    int m_showMask;
    int m_showPlano;
    int m_espacio;
    bool m_agrandar;
    const char* m_host;
    int m_port;
    int m_maxNumBlobs;
    int m_numPoints;
    int m_auto2;
    int m_framesAuto;
    int m_movPlano;
    int m_escala;
    int m_fisheye;
    int m_k;
    clock_t m_inicio,m_fin;

    Qt::MouseButtons m_pressedButton;

    enum {
        METHOD_BACKGROUND,
        METHOD_PLANE,
        METHOD_THRETHOLD
    } m_method;
};

#endif // MAINWINDOW_H
