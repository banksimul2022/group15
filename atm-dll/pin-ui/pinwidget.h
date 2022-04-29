#ifndef PINWIDGET_H
#define PINWIDGET_H

#include <QWidget>
#include "pininterface.h"
#include <QDebug>

namespace Ui {
    class PinWidget;
}

class PinWidget : public PinInterface
{
    Q_OBJECT

    public:
        explicit PinWidget(QWidget *parent = nullptr);
        ~PinWidget();
        void setPinMessage(QString message) override;

    private:
        Ui::PinWidget *ui;
        QTimer pinWidgetTimer;
        void pinTimerReset();
        void pinWidgetHandleNumber(int keyNumber);
        void updatePinWidgetTimerBar();

private slots:
        void on_pushButton_1_clicked();
        void on_pushButton_2_clicked();
        void on_pushButton_3_clicked();
        void on_pushButton_4_clicked();
        void on_pushButton_5_clicked();
        void on_pushButton_6_clicked();
        void on_pushButton_7_clicked();
        void on_pushButton_8_clicked();
        void on_pushButton_9_clicked();
        void on_pushButton_0_clicked();
        void on_pushButton_comma_clicked();
        void on_pushButton_exit_clicked();
        void on_pushButton_cancel_clicked();
        void on_pushButton_enter_clicked();
        void pinWidgetIdleTimeout();
};

#endif // PINWIDGET_H
