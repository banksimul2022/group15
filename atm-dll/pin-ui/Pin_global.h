#ifndef PIN_GLOBAL_H
#define PIN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PIN_LIBRARY)
#  define PIN_EXPORT Q_DECL_EXPORT
#else
#  define PIN_EXPORT Q_DECL_IMPORT
#endif

#endif // PIN_GLOBAL_H
