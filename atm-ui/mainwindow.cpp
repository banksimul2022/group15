#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pageinsertcard.h"

#include <QShortcut>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    currentPage(nullptr),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    new QShortcut(QKeySequence(Qt::Key_F11), this, SLOT(fullscreenShortcut()));
    this->baseTitle = this->windowTitle();
    this->setPage(new PageInsertCard);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::fullscreenShortcut() {
    if(this->isFullScreen()) {
        this->showNormal();
    } else {
        this->showFullScreen();
    }
}

void MainWindow::setPage(QWidget *page) {
    if(this->currentPage) {
        this->ui->rootLayout->removeWidget(this->currentPage);
        this->currentPage->setParent(nullptr);
        delete this->currentPage;
    }

    if(!page) {
        this->currentPage = nullptr;
        this->setWindowTitle(this->baseTitle);
        return;
    }

    this->setWindowTitle(QString("%1 - %2").arg(this->baseTitle, page->windowTitle()));
    this->currentPage = page;

    QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(this->currentPage->sizePolicy().hasHeightForWidth());
    this->currentPage->setSizePolicy(sizePolicy);

    this->currentPage->setParent(this->ui->rootWidget);
    this->ui->rootLayout->addWidget(this->currentPage);
}
