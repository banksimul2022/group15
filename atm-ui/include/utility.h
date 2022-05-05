#ifndef UTILITY_H
#define UTILITY_H

#include <QTranslator>
#include <QWidget>

class Utility {
    public:
        Utility() = delete;
        static void retainSizeWhenHidden(QWidget *widget);
        static void updateVisualProperty(const char *name, QVariant value, QWidget *widget);
        static QString restErrorToText(uint errorCode);
        template<class Target> static inline bool isOfType(QObject *what) { return qobject_cast<Target*>(what) != nullptr; };
        static void switchLanguage(const char *lang);

    private:
        static QTranslator translator;
};

#endif // UTILITY_H
