#ifndef USERSTATUSBARWIDGET_H
#define USERSTATUSBARWIDGET_H

#include <QWidget>

namespace Ui {
    class UserStatusBarWidget;
}

class UserStatusBarWidget : public QWidget {
    Q_OBJECT

    public:
        explicit UserStatusBarWidget(QWidget *parent = nullptr);
        ~UserStatusBarWidget();

        void resetTimeout();

    signals:
        void logout();

    private slots:
        void logoutTimerTick();

    private:
        void updateTimeoutLabel();

        static const uint defaultLogoutTimeout = 30;

        Ui::UserStatusBarWidget *ui;
        QMetaObject::Connection btnConnection;
        QTimer *logoutTimer;
        uint logoutTimout;
};

#endif // USERSTATUSBARWIDGET_H
