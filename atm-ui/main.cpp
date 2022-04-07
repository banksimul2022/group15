#include "atmwindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    ATMWindow w;
    w.show();
    return a.exec();
}
