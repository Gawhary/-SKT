/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "cvimagewidgetgl.h"
#include "qxtspanslider.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *container;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    CvImageWidgetGL *rgbImage;
    CvImageWidgetGL *toBlobsImage;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *imageDepthLayout;
    QLabel *label_5;
    QSlider *image_depthSlider;
    QLabel *label_6;
    QHBoxLayout *distLayout;
    QLabel *label;
    QxtSpanSlider *distSlider;
    QHBoxLayout *checkBoxLayout;
    QCheckBox *flipCheckBox;
    QCheckBox *showPlaneCheckBox;
    QHBoxLayout *buttonLayout;
    QPushButton *setPlaneButton;
    QPushButton *calibrationButton;
    QPushButton *saveButton;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *minLayout;
    QLabel *label_2;
    QSlider *MinSlider;
    QHBoxLayout *planLayout;
    QLabel *label_3;
    QSlider *planeSlider;
    QHBoxLayout *maskLayout;
    QLabel *label_4;
    QSlider *maskSlider;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(850, 402);
        container = new QWidget(MainWindow);
        container->setObjectName(QString::fromUtf8("container"));
        gridLayout = new QGridLayout(container);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(10);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        rgbImage = new CvImageWidgetGL(container);
        rgbImage->setObjectName(QString::fromUtf8("rgbImage"));
        rgbImage->setMinimumSize(QSize(200, 200));

        horizontalLayout_2->addWidget(rgbImage);

        toBlobsImage = new CvImageWidgetGL(container);
        toBlobsImage->setObjectName(QString::fromUtf8("toBlobsImage"));
        toBlobsImage->setMinimumSize(QSize(200, 200));

        horizontalLayout_2->addWidget(toBlobsImage);


        verticalLayout_3->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(5, -1, 5, -1);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        imageDepthLayout = new QHBoxLayout();
        imageDepthLayout->setObjectName(QString::fromUtf8("imageDepthLayout"));
        label_5 = new QLabel(container);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setMinimumSize(QSize(30, 0));

        imageDepthLayout->addWidget(label_5);

        image_depthSlider = new QSlider(container);
        image_depthSlider->setObjectName(QString::fromUtf8("image_depthSlider"));
        image_depthSlider->setEnabled(true);
        image_depthSlider->setOrientation(Qt::Horizontal);

        imageDepthLayout->addWidget(image_depthSlider);

        label_6 = new QLabel(container);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        imageDepthLayout->addWidget(label_6);


        verticalLayout->addLayout(imageDepthLayout);

        distLayout = new QHBoxLayout();
        distLayout->setObjectName(QString::fromUtf8("distLayout"));
        label = new QLabel(container);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(30, 0));

        distLayout->addWidget(label);

        distSlider = new QxtSpanSlider(container);
        distSlider->setObjectName(QString::fromUtf8("distSlider"));
        distSlider->setEnabled(true);
        distSlider->setOrientation(Qt::Horizontal);

        distLayout->addWidget(distSlider);


        verticalLayout->addLayout(distLayout);

        checkBoxLayout = new QHBoxLayout();
        checkBoxLayout->setObjectName(QString::fromUtf8("checkBoxLayout"));
        flipCheckBox = new QCheckBox(container);
        flipCheckBox->setObjectName(QString::fromUtf8("flipCheckBox"));
        flipCheckBox->setEnabled(true);

        checkBoxLayout->addWidget(flipCheckBox);

        showPlaneCheckBox = new QCheckBox(container);
        showPlaneCheckBox->setObjectName(QString::fromUtf8("showPlaneCheckBox"));
        showPlaneCheckBox->setEnabled(true);
        showPlaneCheckBox->setChecked(true);

        checkBoxLayout->addWidget(showPlaneCheckBox);


        verticalLayout->addLayout(checkBoxLayout);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setObjectName(QString::fromUtf8("buttonLayout"));
        setPlaneButton = new QPushButton(container);
        setPlaneButton->setObjectName(QString::fromUtf8("setPlaneButton"));
        setPlaneButton->setEnabled(true);

        buttonLayout->addWidget(setPlaneButton);

        calibrationButton = new QPushButton(container);
        calibrationButton->setObjectName(QString::fromUtf8("calibrationButton"));
        calibrationButton->setEnabled(true);

        buttonLayout->addWidget(calibrationButton);

        saveButton = new QPushButton(container);
        saveButton->setObjectName(QString::fromUtf8("saveButton"));
        saveButton->setEnabled(true);

        buttonLayout->addWidget(saveButton);


        verticalLayout->addLayout(buttonLayout);


        horizontalLayout->addLayout(verticalLayout);

        horizontalSpacer = new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        minLayout = new QHBoxLayout();
        minLayout->setObjectName(QString::fromUtf8("minLayout"));
        label_2 = new QLabel(container);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(30, 0));

        minLayout->addWidget(label_2);

        MinSlider = new QSlider(container);
        MinSlider->setObjectName(QString::fromUtf8("MinSlider"));
        MinSlider->setEnabled(true);
        MinSlider->setMinimumSize(QSize(100, 0));
        MinSlider->setOrientation(Qt::Horizontal);

        minLayout->addWidget(MinSlider);


        verticalLayout_2->addLayout(minLayout);

        planLayout = new QHBoxLayout();
        planLayout->setObjectName(QString::fromUtf8("planLayout"));
        label_3 = new QLabel(container);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(30, 0));

        planLayout->addWidget(label_3);

        planeSlider = new QSlider(container);
        planeSlider->setObjectName(QString::fromUtf8("planeSlider"));
        planeSlider->setEnabled(true);
        planeSlider->setOrientation(Qt::Horizontal);

        planLayout->addWidget(planeSlider);


        verticalLayout_2->addLayout(planLayout);

        maskLayout = new QHBoxLayout();
        maskLayout->setObjectName(QString::fromUtf8("maskLayout"));
        label_4 = new QLabel(container);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setMinimumSize(QSize(30, 0));

        maskLayout->addWidget(label_4);

        maskSlider = new QSlider(container);
        maskSlider->setObjectName(QString::fromUtf8("maskSlider"));
        maskSlider->setEnabled(true);
        maskSlider->setOrientation(Qt::Horizontal);

        maskLayout->addWidget(maskSlider);


        verticalLayout_2->addLayout(maskLayout);


        horizontalLayout->addLayout(verticalLayout_2);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(2, 1);

        verticalLayout_3->addLayout(horizontalLayout);

        verticalLayout_3->setStretch(0, 1);

        gridLayout->addLayout(verticalLayout_3, 0, 0, 1, 1);

        MainWindow->setCentralWidget(container);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Tracker", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "Image", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MainWindow", "Depth", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Dist.", 0, QApplication::UnicodeUTF8));
        flipCheckBox->setText(QApplication::translate("MainWindow", "Flip input", 0, QApplication::UnicodeUTF8));
        showPlaneCheckBox->setText(QApplication::translate("MainWindow", "Show Plane", 0, QApplication::UnicodeUTF8));
        setPlaneButton->setText(QApplication::translate("MainWindow", "Set Plane", 0, QApplication::UnicodeUTF8));
        calibrationButton->setText(QApplication::translate("MainWindow", "Calibration", 0, QApplication::UnicodeUTF8));
        saveButton->setText(QApplication::translate("MainWindow", "Save", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Min", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "Plane", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "Mask", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
