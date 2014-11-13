#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include "ui_mainwindow.h"

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

private:
    Ui::MainWindow ui;
    bool m_exit;

};

#endif // MAINWINDOW_H
