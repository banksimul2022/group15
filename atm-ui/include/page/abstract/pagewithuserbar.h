#ifndef PAGEWITHUSERBAR_H
#define PAGEWITHUSERBAR_H

#include "pagebase.h"
#include "page/util/userstatusbarwidget.h"

#include <QHBoxLayout>

class PageWithUserBar : public PageBase {
    Q_OBJECT

    public:
        explicit PageWithUserBar(UserStatusBarWidget::Mode mode, PageManager *pageManager, RestInfoData *infoData = nullptr, QWidget *parent = nullptr);
        virtual ~PageWithUserBar() = 0;

        void stopTimer();
        void startTimer();

    protected:
        void setupUserBar(QLayout *layout);
        UserStatusBarWidget * userStatusBar;

    protected slots:
        PageBase::RestDataAction onRestData(RestReturnData *data) override;

        virtual void onExtraButton(int id);
        virtual void onLeave();
        virtual void onOk();
};

#endif // PAGEWITHUSERBAR_H
