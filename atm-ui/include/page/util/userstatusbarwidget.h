#ifndef USERSTATUSBARWIDGET_H
#define USERSTATUSBARWIDGET_H

#include <QSignalMapper>
#include <QWidget>
#include <QTimer>
#include <restinfodata.h>

namespace Ui {
    class UserStatusBarWidget;
}

class UserStatusBarWidget : public QWidget {
    Q_OBJECT

    public:
        enum Mode {
            logout,
            leaveOnly,
            leaveAndOk,
            custom
        };

        explicit UserStatusBarWidget(Mode logout, RestInfoData *userInfo = nullptr, QWidget *parent = nullptr);
        ~UserStatusBarWidget();

        void setButtonTitles(int count, ...);
        void setButtonTitles(int count, va_list args) ;

        void stopLeaveTimeout();
        void startLeaveTimeout();
        void resetLeaveTimeout(bool start = true);
        Mode mode();

    signals:
        void extraButton(int id);
        void leave();
        void ok();

    private slots:
        void leaveTimerTick();

private:
        void addButton(const char *text);
        uint getDefaultTimeout();
        void updateTimeoutLabel();

        static const uint defaultLogoutTimeout = 30;
        static const uint defaultLeaveTimeout = 10;

        Ui::UserStatusBarWidget *ui;
        QSignalMapper *extraBtnMapper;
        const Mode barMode;
        QTimer leaveTimer;
        uint leaveTimout;
};

#endif // USERSTATUSBARWIDGET_H
