#ifndef UTILITY_H
#define UTILITY_H

#include <QWidget>

class Utility {
    public:
        Utility() = delete;
        static void retainSizeWhenHidden(QWidget *widget);
};

#endif // UTILITY_H
