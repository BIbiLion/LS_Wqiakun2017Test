/****************************************************************************
** Meta object code from reading C++ file 'FeedbackDialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Feedback/FeedbackDialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FeedbackDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_FeedbackDialog_t {
    QByteArrayData data[9];
    char stringdata[130];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_FeedbackDialog_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_FeedbackDialog_t qt_meta_stringdata_FeedbackDialog = {
    {
QT_MOC_LITERAL(0, 0, 14),
QT_MOC_LITERAL(1, 15, 25),
QT_MOC_LITERAL(2, 41, 0),
QT_MOC_LITERAL(3, 42, 21),
QT_MOC_LITERAL(4, 64, 14),
QT_MOC_LITERAL(5, 79, 15),
QT_MOC_LITERAL(6, 95, 12),
QT_MOC_LITERAL(7, 108, 13),
QT_MOC_LITERAL(8, 122, 6)
    },
    "FeedbackDialog\0on_lineEdit_returnPressed\0"
    "\0on_pushButton_pressed\0UploadFinished\0"
    "ResetButtonText\0DownloadText\0EnginioReply*\0"
    "pReply\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FeedbackDialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x08,
       3,    0,   40,    2, 0x08,
       4,    0,   41,    2, 0x08,
       5,    0,   42,    2, 0x08,
       6,    1,   43,    2, 0x08,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    8,

       0        // eod
};

void FeedbackDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FeedbackDialog *_t = static_cast<FeedbackDialog *>(_o);
        switch (_id) {
        case 0: _t->on_lineEdit_returnPressed(); break;
        case 1: _t->on_pushButton_pressed(); break;
        case 2: _t->UploadFinished(); break;
        case 3: _t->ResetButtonText(); break;
        case 4: _t->DownloadText((*reinterpret_cast< EnginioReply*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject FeedbackDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_FeedbackDialog.data,
      qt_meta_data_FeedbackDialog,  qt_static_metacall, 0, 0}
};


const QMetaObject *FeedbackDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FeedbackDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FeedbackDialog.stringdata))
        return static_cast<void*>(const_cast< FeedbackDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int FeedbackDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
