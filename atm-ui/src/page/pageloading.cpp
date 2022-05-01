#include "page/pageloading.h"
#include "ui_pageloading.h"
#include "userstatusbarwidget.h"

#include <QSvgRenderer>

PageLoading::PageLoading(PageManager *pageManager, QWidget *parent) :
    PageBase(pageManager, parent),
    ui(new Ui::PageLoading)
{
    ui->setupUi(this);
    this->ui->icon->load(QString(":files/icons/spinner.svg"));
    this->ui->icon->renderer()->setAspectRatioMode(Qt::KeepAspectRatio);
}

PageLoading::~PageLoading() {
    delete ui;
}
