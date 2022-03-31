#ifndef PINUIWIDGET_H
#define PINUIWIDGET_H

#include <QWidget>

namespace Ui {
    class PinUIWidget;
}

class PinUIWidget : public QWidget {
    Q_OBJECT

    public:
        explicit PinUIWidget(QWidget *parent = nullptr);
        ~PinUIWidget();

    private:
        Ui::PinUIWidget *ui;
};

#endif // PINUIWIDGET_H
