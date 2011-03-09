/****************************************************************************
** Meta object code from reading C++ file 'dispatcher.h'
**
** Created: Wed Mar 9 11:41:58 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../dispatcher.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dispatcher.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Dispatcher[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      12,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,
      40,   11,   11,   11, 0x05,
      67,   11,   11,   11, 0x05,
      94,   11,   11,   11, 0x05,
     120,   11,   11,   11, 0x05,
     147,   11,   11,   11, 0x05,
     173,   11,   11,   11, 0x05,
     201,   11,   11,   11, 0x05,
     228,   11,   11,   11, 0x05,
     264,   11,   11,   11, 0x05,
     299,   11,   11,   11, 0x05,
     334,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
     370,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Dispatcher[] = {
    "Dispatcher\0\0signalStartComponentAudio()\0"
    "signalStopComponentAudio()\0"
    "signalStartComponentFile()\0"
    "signalStopComponentFile()\0"
    "signalStartComponentText()\0"
    "signalStopComponentText()\0"
    "signalStartComponentVoice()\0"
    "signalStopComponentVoice()\0"
    "signalPacketToComponentAudio(void*)\0"
    "signalPacketToComponentFile(void*)\0"
    "signalPacketToComponentText(void*)\0"
    "signalPacketToComponentVoice(void*)\0"
    "slotPacketRecieved(void*)\0"
};

const QMetaObject Dispatcher::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Dispatcher,
      qt_meta_data_Dispatcher, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Dispatcher::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Dispatcher::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Dispatcher::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Dispatcher))
        return static_cast<void*>(const_cast< Dispatcher*>(this));
    return QObject::qt_metacast(_clname);
}

int Dispatcher::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: signalStartComponentAudio(); break;
        case 1: signalStopComponentAudio(); break;
        case 2: signalStartComponentFile(); break;
        case 3: signalStopComponentFile(); break;
        case 4: signalStartComponentText(); break;
        case 5: signalStopComponentText(); break;
        case 6: signalStartComponentVoice(); break;
        case 7: signalStopComponentVoice(); break;
        case 8: signalPacketToComponentAudio((*reinterpret_cast< void*(*)>(_a[1]))); break;
        case 9: signalPacketToComponentFile((*reinterpret_cast< void*(*)>(_a[1]))); break;
        case 10: signalPacketToComponentText((*reinterpret_cast< void*(*)>(_a[1]))); break;
        case 11: signalPacketToComponentVoice((*reinterpret_cast< void*(*)>(_a[1]))); break;
        case 12: slotPacketRecieved((*reinterpret_cast< void*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void Dispatcher::signalStartComponentAudio()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void Dispatcher::signalStopComponentAudio()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void Dispatcher::signalStartComponentFile()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void Dispatcher::signalStopComponentFile()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void Dispatcher::signalStartComponentText()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void Dispatcher::signalStopComponentText()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void Dispatcher::signalStartComponentVoice()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void Dispatcher::signalStopComponentVoice()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}

// SIGNAL 8
void Dispatcher::signalPacketToComponentAudio(void * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void Dispatcher::signalPacketToComponentFile(void * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void Dispatcher::signalPacketToComponentText(void * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void Dispatcher::signalPacketToComponentVoice(void * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}
QT_END_MOC_NAMESPACE
