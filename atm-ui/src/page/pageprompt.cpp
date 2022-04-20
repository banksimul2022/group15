#include "page/pageprompt.h"
#include "ui_pageprompt.h"
#include "userstatusbarwidget.h"

#include <QSvgRenderer>

PagePrompt::PagePrompt(const char *ctx, const char *title, const char *prompt, PagePrompt::Icon icon, StateManager *stateManager, QWidget *parent, int count, ...) :
    PageWithUserBar(UserStatusBarWidget::custom, stateManager, parent),
    ui(new Ui::PagePrompt)
{
    va_list args;
    va_start(args, count);
    this->construct(ctx, title, prompt, icon, count, args);
    va_end(args);
}

PagePrompt::PagePrompt(const char *ctx, const char *title, const char *prompt, PagePrompt::Icon icon, int count, va_list args, StateManager *stateManager, QWidget *parent) :
    PageWithUserBar(UserStatusBarWidget::custom, stateManager, parent),
    ui(new Ui::PagePrompt)
{
    this->construct(ctx, title, prompt, icon, count, args);
}

PagePrompt::~PagePrompt() {
    delete ui;
}

void PagePrompt::construct(const char *ctx, const char *title, const char *prompt, Icon icon, int count, va_list args) {
    ui->setupUi(this);
    this->setupUserBar(this->ui->widgetRootLayout);

    this->setWindowTitle(QCoreApplication::translate(ctx, title, nullptr));
    this->ui->prompt->setText(QCoreApplication::translate(ctx, prompt, nullptr));

    this->userStatusBar->setButtonTitles(ctx, count, args);

    switch(icon) {
        case Icon::info:
            this->ui->icon->load(QString(":files/icons/info.svg"));
            break;
        case Icon::question:
            this->ui->icon->load(QString(":files/icons/question.svg"));
            break;
        case Icon::warning:
            this->ui->icon->load(QString(":files/icons/warning.svg"));
            break;
        case Icon::error:
            this->ui->icon->load(QString(":files/icons/error.svg"));
            break;
        default:
        case Icon::none:
            /* Do nothing */
            break;
    }

    this->ui->icon->renderer()->setAspectRatioMode(Qt::KeepAspectRatio);
}
