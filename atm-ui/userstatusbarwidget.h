#ifndef USERSTATUSBARWIDGET_H
#define USERSTATUSBARWIDGET_H

#include <QWidget>

namespace Ui {
    class UserStatusBarWidget;
}

class UserStatusBarWidget : public QWidget {
    Q_OBJECT

    public:
        enum Mode {
            logout,
            leaveOnly,
            leaveAndOk
        };

        explicit UserStatusBarWidget(Mode logout, QWidget *parent = nullptr);
        ~UserStatusBarWidget();

        void resetLeaveTimeout();
        Mode mode();

    signals:
        void leave();
        void ok();

    private slots:
        void leaveTimerTick();

private:
        uint getDefaultTimeout();
        void updateTimeoutLabel();

        static const uint defaultLogoutTimeout = 30;
        static const uint defaultLeaveTimeout = 10;

        Ui::UserStatusBarWidget *ui;
        const Mode barMode;
        QTimer *leaveTimer;
        uint leaveTimout;
};

#endif // USERSTATUSBARWIDGET_H
