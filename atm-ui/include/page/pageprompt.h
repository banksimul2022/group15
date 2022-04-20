#ifndef PAGEPROMPT_H
#define PAGEPROMPT_H

#include "page/abstract/pagewithuserbar.h"
#include <QWidget>

namespace Ui {
    class PagePrompt;
}

class PagePrompt : public PageWithUserBar {
    Q_OBJECT

    public:
        enum Icon {
            info,
            question,
            warning,
            error,
            none
        };

        explicit PagePrompt(const char *ctx, const char *title, const char *prompt, PagePrompt::Icon icon, StateManager *stateManager, QWidget *parent = nullptr, int count = 0, ...);
        explicit PagePrompt(const char *ctx, const char *title, const char *prompt, PagePrompt::Icon icon, int count, va_list args, StateManager *stateManager, QWidget *parent = nullptr);
        ~PagePrompt();

    private:
        void construct(const char *ctx, const char *title, const char *prompt, PagePrompt::Icon icon, int count, va_list args);
        Ui::PagePrompt *ui;
};

#endif // PAGEPROMPT_H
