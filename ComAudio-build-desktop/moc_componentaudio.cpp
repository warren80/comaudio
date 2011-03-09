/****************************************************************************
** Meta object code from reading C++ file 'componentaudio.h'
**
** Created: Wed Mar 9 11:14:43 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../componentaudio.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'componentaudio.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ComponentAudio[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ComponentAudio[] = {
    "ComponentAudio\0\0slotStart()\0"
};

const QMetaObject ComponentAudio::staticMetaObject = {
    { &Component::staticMetaObject, qt_meta_stringdata_ComponentAudio,
      qt_meta_data_ComponentAudio, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ComponentAudio::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ComponentAudio::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ComponentAudio::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ComponentAudio))
        return static_cast<void*>(const_cast< ComponentAudio*>(this));
    return Component::qt_metacast(_clname);
}

int ComponentAudio::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Component::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: slotStart(); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
