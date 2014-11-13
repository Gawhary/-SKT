/****************************************************************************
** Meta object code from reading C++ file 'cvimagewidget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../cvimagewidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cvimagewidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CvImageWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      21,   15,   14,   14, 0x0a,
      40,   15,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CvImageWidget[] = {
    "CvImageWidget\0\0image\0showImage(cv::Mat)\0"
    "showImage(IplImage*)\0"
};

void CvImageWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CvImageWidget *_t = static_cast<CvImageWidget *>(_o);
        switch (_id) {
        case 0: _t->showImage((*reinterpret_cast< const cv::Mat(*)>(_a[1]))); break;
        case 1: _t->showImage((*reinterpret_cast< IplImage*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CvImageWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CvImageWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CvImageWidget,
      qt_meta_data_CvImageWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CvImageWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CvImageWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CvImageWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CvImageWidget))
        return static_cast<void*>(const_cast< CvImageWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int CvImageWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
