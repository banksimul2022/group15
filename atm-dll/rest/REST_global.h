#ifndef REST_GLOBAL_H
#define REST_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(REST_LIBRARY)
#  define REST_EXPORT Q_DECL_EXPORT
#else
#  define REST_EXPORT Q_DECL_IMPORT
#endif

#endif // REST_GLOBAL_H
