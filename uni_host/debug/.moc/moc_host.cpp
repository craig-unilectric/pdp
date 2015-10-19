/****************************************************************************
** Meta object code from reading C++ file 'host.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../host.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'host.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Host_t {
    QByteArrayData data[18];
    char stringdata[219];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Host_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Host_t qt_meta_stringdata_Host = {
    {
QT_MOC_LITERAL(0, 0, 4), // "Host"
QT_MOC_LITERAL(1, 5, 19), // "comms_open_selected"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(4, 43, 4), // "item"
QT_MOC_LITERAL(5, 48, 10), // "comms_open"
QT_MOC_LITERAL(6, 59, 10), // "comms_test"
QT_MOC_LITERAL(7, 70, 8), // "read_DMA"
QT_MOC_LITERAL(8, 79, 15), // "read_channelDMA"
QT_MOC_LITERAL(9, 95, 24), // "read_channelDMA_filtered"
QT_MOC_LITERAL(10, 120, 13), // "write_command"
QT_MOC_LITERAL(11, 134, 11), // "read_status"
QT_MOC_LITERAL(12, 146, 19), // "read_status_channel"
QT_MOC_LITERAL(13, 166, 7), // "channel"
QT_MOC_LITERAL(14, 174, 14), // "relay_selected"
QT_MOC_LITERAL(15, 189, 10), // "relay_read"
QT_MOC_LITERAL(16, 200, 8), // "relay_on"
QT_MOC_LITERAL(17, 209, 9) // "relay_off"

    },
    "Host\0comms_open_selected\0\0QListWidgetItem*\0"
    "item\0comms_open\0comms_test\0read_DMA\0"
    "read_channelDMA\0read_channelDMA_filtered\0"
    "write_command\0read_status\0read_status_channel\0"
    "channel\0relay_selected\0relay_read\0"
    "relay_on\0relay_off"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Host[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   79,    2, 0x08 /* Private */,
       5,    0,   82,    2, 0x08 /* Private */,
       6,    0,   83,    2, 0x08 /* Private */,
       7,    0,   84,    2, 0x08 /* Private */,
       8,    0,   85,    2, 0x08 /* Private */,
       9,    0,   86,    2, 0x08 /* Private */,
      10,    0,   87,    2, 0x08 /* Private */,
      11,    0,   88,    2, 0x08 /* Private */,
      12,    1,   89,    2, 0x08 /* Private */,
      14,    0,   92,    2, 0x08 /* Private */,
      15,    0,   93,    2, 0x08 /* Private */,
      16,    0,   94,    2, 0x08 /* Private */,
      17,    0,   95,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Bool,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::UChar,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Host::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Host *_t = static_cast<Host *>(_o);
        switch (_id) {
        case 0: _t->comms_open_selected((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 1: { bool _r = _t->comms_open();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 2: _t->comms_test(); break;
        case 3: _t->read_DMA(); break;
        case 4: _t->read_channelDMA(); break;
        case 5: _t->read_channelDMA_filtered(); break;
        case 6: _t->write_command(); break;
        case 7: _t->read_status(); break;
        case 8: _t->read_status_channel((*reinterpret_cast< quint8(*)>(_a[1]))); break;
        case 9: _t->relay_selected(); break;
        case 10: _t->relay_read(); break;
        case 11: _t->relay_on(); break;
        case 12: _t->relay_off(); break;
        default: ;
        }
    }
}

const QMetaObject Host::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Host.data,
      qt_meta_data_Host,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Host::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Host::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Host.stringdata))
        return static_cast<void*>(const_cast< Host*>(this));
    return QWidget::qt_metacast(_clname);
}

int Host::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
