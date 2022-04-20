#include "page/pageprompt.h"
#include "ui_pageprompt.h"
#include "userstatusbarwidget.h"

#include <QSvgRenderer>

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

PagePrompt::~PagePrompt() {
    delete ui;
}
