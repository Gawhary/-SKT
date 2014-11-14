#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
private slots:
    int run();

    void on_setPlaneButton_clicked();

    void on_maskSlider_valueChanged(int value);

    void on_planeSlider_valueChanged(int value);

    void on_saveButton_clicked();

    void on_calibrationButton_clicked();

private:
    Ui::MainWindow ui;
    bool m_exit;
    calibrator* m_CAL;

    void affineWarperAndMixer(CvMat *warp, IplImage *iRGB, IplImage *aux1, IplImage *aux2, float alpha, IplImage *gray);
    void hacer0bordes(IplImage *in);
    std::string loadParameters(bool all);
    void saveParameters(int a, void *param);
    void callBackPoint(int a, void *param);
    void callBackAgrandar(int a);
    void callBackSetValue(int a, void *param);
    void callBackButton0(int a, void *param);
    void MouseEvWrapper(int event, int x, int y, int flag, void *param);
    void callBackButton(int a, void *param);


    enum {
        METHOD_BACKGROUND,
        METHOD_PLANE,
        METHOD_THRETHOLD
    } m_method;
};

#endif // MAINWINDOW_H
