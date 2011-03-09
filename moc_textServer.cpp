/****************************************************************************
** Meta object code from reading C++ file 'textServer.h'
**
** Created: Tue Mar 8 15:59:04 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "textServer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'textServer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TextServer[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      54,   50,   11,   11, 0x0a,
      79,   50,   11,   11, 0x0a,
     107,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_TextServer[] = {
    "TextServer\0\0signalClientConnected(ClientConnect*)\0"
    "str\0slotClientConnect(char*)\0"
    "slotClientDisconnect(char*)\0Start()\0"
};

const QMetaObject TextServer::staticMetaObject = {
    { &Component::staticMetaObject, qt_meta_stringdata_TextServer,
      qt_meta_data_TextServer, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TextServer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TextServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TextServer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TextServer))
        return static_cast<void*>(const_cast< TextServer*>(this));
    return Component::qt_metacast(_clname);
}

int TextServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Component::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: signalClientConnected((*reinterpret_cast< ClientConnect*(*)>(_a[1]))); break;
        case 1: slotClientConnect((*reinterpret_cast< char*(*)>(_a[1]))); break;
        case 2: slotClientDisconnect((*reinterpret_cast< char*(*)>(_a[1]))); break;
        case 3: Start(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void TextServer::signalClientConnected(ClientConnect * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
