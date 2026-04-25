#include "mainpage.h"
#include "ui_mainpage.h"

mainPage::mainPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainPage)
{
    ui->setupUi(this);
}

mainPage::~mainPage()
{
    delete ui;
}
