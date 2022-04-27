#ifndef PAGEPROMPT_H
#define PAGEPROMPT_H

#include "page/pageprompt_enum.h"
#include "page/abstract/pagewithuserbar.h"
#include <QWidget>

namespace Ui {
    class PagePrompt;
}

class PagePrompt : public PageWithUserBar {
    Q_OBJECT

    public:
        explicit PagePrompt(QString title, QString prompt, PromptEnum::Icon icon, PageManager *stateManager, QWidget *parent = nullptr, int count = 0, ...);
        explicit PagePrompt(QString title, QString prompt, PromptEnum::Icon icon, int count, va_list args, PageManager *stateManager, QWidget *parent = nullptr);
        ~PagePrompt();

    protected slots:
        void onExtraButton(int id) override;

    private:
        void construct(QString title, QString prompt, PromptEnum::Icon icon, int count, va_list args);
        Ui::PagePrompt *ui;
};

#endif // PAGEPROMPT_H
