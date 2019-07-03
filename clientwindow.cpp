#include "clientwindow.h"
#include "ui_clientwindow.h"

#include <QtWidgets/qmainwindow.h>


ClientWindow::ClientWindow(QWidget *parent) : QMainWindow (parent), ui(new Ui::ClientWindow)
{
    ui->setupUi(this);
}

ClientWindow::~ClientWindow()
{
    delete ui;
}
