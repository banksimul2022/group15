#include "pageprompt.h"
#include "ui_pageprompt.h"
#include "userstatusbarwidget.h"

PagePrompt::PagePrompt(const char *ctx, const char *title, const char *prompt, PagePrompt::Icon icon, StateManager *stateManager, QWidget *parent, int count, ...) :
    PageWithUserBar(UserStatusBarWidget::custom, stateManager, parent),
    ui(new Ui::PagePrompt)
{
    ui->setupUi(this);
    this->setupUserBar(this->ui->widgetRootLayout);

    this->setWindowTitle(QCoreApplication::translate(ctx, title, nullptr));
    this->ui->prompt->setText(QCoreApplication::translate(ctx, prompt, nullptr));

    va_list args;
    va_start(args, count);
    this->userStatusBar->setButtonTitles(ctx, count, args);
    va_end(args);

    switch(icon) {
        default:
        case Icon::none:
            /* Do nothing */
            break;
    }
}

PagePrompt::~PagePrompt() {
    delete ui;
}
