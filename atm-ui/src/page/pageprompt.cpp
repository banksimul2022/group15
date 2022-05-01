#include "page/pageprompt.h"
#include "ui_pageprompt.h"
#include "userstatusbarwidget.h"

#include <QSvgRenderer>

PagePrompt::PagePrompt(QString title, QString prompt, PromptEnum::Icon icon, PageManager *pageManager, QWidget *parent, int count, ...) :
    PageWithUserBar(UserStatusBarWidget::custom, pageManager, nullptr, parent),
    ui(new Ui::PagePrompt)
{
    va_list args;
    va_start(args, count);
    this->construct(title, prompt, icon, count, args);
    va_end(args);
}

PagePrompt::PagePrompt(QString title, QString prompt, PromptEnum::Icon icon, int count, va_list args, PageManager *pageManager, QWidget *parent) :
    PageWithUserBar(UserStatusBarWidget::custom, pageManager, nullptr, parent),
    ui(new Ui::PagePrompt)
{
    this->construct(title, prompt, icon, count, args);
}

void PagePrompt::onExtraButton(int id) {
    this->pageManager->leaveCurrentPage(QVariant(id));
}

PagePrompt::~PagePrompt() {
    delete ui;
}

void PagePrompt::construct(QString title, QString prompt, PromptEnum::Icon icon, int count, va_list args) {
    ui->setupUi(this);
    this->setupUserBar(this->ui->widgetRootLayout);

    this->setWindowTitle(title);
    this->ui->prompt->setText(prompt);

    this->userStatusBar->setButtonTitles(count, args);

    switch(icon) {
        case PromptEnum::info:
            this->ui->icon->load(QString(":files/icons/info.svg"));
            break;
        case PromptEnum::question:
            this->ui->icon->load(QString(":files/icons/question.svg"));
            break;
        case PromptEnum::warning:
            this->ui->icon->load(QString(":files/icons/warning.svg"));
            break;
        case PromptEnum::error:
            this->ui->icon->load(QString(":files/icons/error.svg"));
            break;
        default:
        case PromptEnum::none:
            /* Do nothing */
            break;
    }

    this->ui->icon->renderer()->setAspectRatioMode(Qt::KeepAspectRatio);
}
