/****************************************************************************
** Meta object code from reading C++ file 'cvimagewidgetgl.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../cvimagewidgetgl.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cvimagewidgetgl.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CvImageDockableWidgetGL[] = {

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
      36,   25,   24,   24, 0x0a,
      56,   24,   24,   24, 0x2a,

       0        // eod
};

static const char qt_meta_stringdata_CvImageDockableWidgetGL[] = {
    "CvImageDockableWidgetGL\0\0fullscreen\0"
    "setFullscreen(bool)\0setFullscreen()\0"
};

void CvImageDockableWidgetGL::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CvImageDockableWidgetGL *_t = static_cast<CvImageDockableWidgetGL *>(_o);
        switch (_id) {
        case 0: _t->setFullscreen((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->setFullscreen(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CvImageDockableWidgetGL::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CvImageDockableWidgetGL::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CvImageDockableWidgetGL,
      qt_meta_data_CvImageDockableWidgetGL, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CvImageDockableWidgetGL::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CvImageDockableWidgetGL::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CvImageDockableWidgetGL::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CvImageDockableWidgetGL))
        return static_cast<void*>(const_cast< CvImageDockableWidgetGL*>(this));
    return QWidget::qt_metacast(_clname);
}

int CvImageDockableWidgetGL::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
static const uint qt_meta_data_CvImageWidgetGL[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_CvImageWidgetGL[] = {
    "CvImageWidgetGL\0"
};

void CvImageWidgetGL::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData CvImageWidgetGL::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CvImageWidgetGL::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_CvImageWidgetGL,
      qt_meta_data_CvImageWidgetGL, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CvImageWidgetGL::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CvImageWidgetGL::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CvImageWidgetGL::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CvImageWidgetGL))
        return static_cast<void*>(const_cast< CvImageWidgetGL*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int CvImageWidgetGL::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
