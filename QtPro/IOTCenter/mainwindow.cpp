#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    emqx = new EmqxForm(this);
    home = new HomeForm(this);
    home->hide();
    ui->verticalLayout_3->addWidget(emqx);
    ui->verticalLayout_3->addWidget(home);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete emqx;
    delete home;
}

// EMQX管理
void MainWindow::on_card_Clicked()
{
    home->hide();
    emqx->show();
}

// My Home
void MainWindow::on_card_2_Clicked()
{
    emqx->hide();
    home->show();
}
