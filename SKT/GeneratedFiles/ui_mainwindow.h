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
    QSlider *horizontalSlider_4;
    QLabel *label_6;
    QHBoxLayout *distLayout;
    QLabel *label;
    QxtSpanSlider *distSlider;
    QHBoxLayout *checkBoxLayout;
    QCheckBox *flipCheckBox;
    QCheckBox *checkBox_2;
    QHBoxLayout *buttonLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *minLayout;
    QLabel *label_2;
    QSlider *horizontalSlider;
    QHBoxLayout *planLayout;
    QLabel *label_3;
    QSlider *horizontalSlider_2;
    QHBoxLayout *maskLayout;
    QLabel *label_4;
    QSlider *horizontalSlider_3;

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
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        imageDepthLayout = new QHBoxLayout();
        imageDepthLayout->setObjectName(QString::fromUtf8("imageDepthLayout"));
        label_5 = new QLabel(container);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setMinimumSize(QSize(30, 0));

        imageDepthLayout->addWidget(label_5);

        horizontalSlider_4 = new QSlider(container);
        horizontalSlider_4->setObjectName(QString::fromUtf8("horizontalSlider_4"));
        horizontalSlider_4->setEnabled(false);
        horizontalSlider_4->setOrientation(Qt::Horizontal);

        imageDepthLayout->addWidget(horizontalSlider_4);

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
        flipCheckBox->setEnabled(false);

        checkBoxLayout->addWidget(flipCheckBox);

        checkBox_2 = new QCheckBox(container);
        checkBox_2->setObjectName(QString::fromUtf8("checkBox_2"));
        checkBox_2->setEnabled(false);

        checkBoxLayout->addWidget(checkBox_2);


        verticalLayout->addLayout(checkBoxLayout);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setObjectName(QString::fromUtf8("buttonLayout"));
        pushButton = new QPushButton(container);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setEnabled(false);

        buttonLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(container);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setEnabled(false);

        buttonLayout->addWidget(pushButton_2);

        pushButton_3 = new QPushButton(container);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setEnabled(false);

        buttonLayout->addWidget(pushButton_3);


        verticalLayout->addLayout(buttonLayout);


        horizontalLayout->addLayout(verticalLayout);

        horizontalSpacer = new QSpacerItem(30, 20, QSizePolicy::Preferred, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        minLayout = new QHBoxLayout();
        minLayout->setObjectName(QString::fromUtf8("minLayout"));
        label_2 = new QLabel(container);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(30, 0));

        minLayout->addWidget(label_2);

        horizontalSlider = new QSlider(container);
        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setEnabled(false);
        horizontalSlider->setMinimumSize(QSize(100, 0));
        horizontalSlider->setOrientation(Qt::Horizontal);

        minLayout->addWidget(horizontalSlider);


        verticalLayout_2->addLayout(minLayout);

        planLayout = new QHBoxLayout();
        planLayout->setObjectName(QString::fromUtf8("planLayout"));
        label_3 = new QLabel(container);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(30, 0));

        planLayout->addWidget(label_3);

        horizontalSlider_2 = new QSlider(container);
        horizontalSlider_2->setObjectName(QString::fromUtf8("horizontalSlider_2"));
        horizontalSlider_2->setEnabled(false);
        horizontalSlider_2->setOrientation(Qt::Horizontal);

        planLayout->addWidget(horizontalSlider_2);


        verticalLayout_2->addLayout(planLayout);

        maskLayout = new QHBoxLayout();
        maskLayout->setObjectName(QString::fromUtf8("maskLayout"));
        label_4 = new QLabel(container);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setMinimumSize(QSize(30, 0));

        maskLayout->addWidget(label_4);

        horizontalSlider_3 = new QSlider(container);
        horizontalSlider_3->setObjectName(QString::fromUtf8("horizontalSlider_3"));
        horizontalSlider_3->setEnabled(false);
        horizontalSlider_3->setOrientation(Qt::Horizontal);

        maskLayout->addWidget(horizontalSlider_3);


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
        flipCheckBox->setText(QApplication::translate("MainWindow", "Flip", 0, QApplication::UnicodeUTF8));
        checkBox_2->setText(QApplication::translate("MainWindow", "Plane", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("MainWindow", "Set Plane", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("MainWindow", "Calipration", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("MainWindow", "Save", 0, QApplication::UnicodeUTF8));
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
