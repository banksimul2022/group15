#ifndef ATMENGINE_GLOBAL_H
#define ATMENGINE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ATMENGINE_LIBRARY)
#  define ATMENGINE_EXPORT Q_DECL_EXPORT
#else
#  define ATMENGINE_EXPORT Q_DECL_IMPORT
#endif

#endif // ATMENGINE_GLOBAL_H
