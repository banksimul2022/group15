#include "atmwindow.h"

#include <QApplication>
#include <QDir>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::applicationDirPath());
    ATMWindow w;
    w.show();
    return a.exec();
}
