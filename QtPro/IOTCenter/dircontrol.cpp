#include "dircontrol.h"
#include <QPushButton>
#include <QDebug>

DIRControl::DIRControl(QWidget *parent) :
    QWidget(parent)
{
    grid = new QGridLayout();
    setMinimumSize(200,120);

    setStyleSheet("QPushButton{height:100%;border:false;} QPushButton:hover{background-color:#d9d9d9;color:#fefffe; top: 5px; left: 10px;}");

    bt1 = new QPushButton();
    bt2 = new QPushButton();
    bt3 = new QPushButton();
    bt4 = new QPushButton();
    bt5 = new QPushButton();

    connect(bt1,SIGNAL(clicked()),this,SLOT(btn1_Clicked()));
    connect(bt2,SIGNAL(clicked()),this,SLOT(btn2_Clicked()));
    connect(bt3,SIGNAL(clicked()),this,SLOT(btn3_Clicked()));
    connect(bt4,SIGNAL(clicked()),this,SLOT(btn4_Clicked()));
    connect(bt5,SIGNAL(clicked()),this,SLOT(btn5_Clicked()));

    //bt1->setFlat(true);
    bt1->setIcon(QIcon(":/begin.png"));
    bt1->setContentsMargins(0,0,0,0);
    bt1->setCursor(Qt::PointingHandCursor);

    bt2->setIcon(QIcon(":/top.png"));
    bt3->setIcon(QIcon(":/bottom.png"));
    bt4->setIcon(QIcon(":/left.png"));
    bt5->setIcon(QIcon(":/right.png"));

    grid->addWidget(bt1,1,1,1,1);
    grid->addWidget(bt2,0,1);
    grid->addWidget(bt3,2,1);
    grid->addWidget(bt4,1,0);
    grid->addWidget(bt5,1,2);

    setLayout(grid);
}

void DIRControl::paintEvent(QPaintEvent *event)
{
    bt1->setIconSize(QSize (bt1->height(),bt1->height()));
    bt2->setIconSize(QSize (bt2->height(),bt2->height()));
    bt3->setIconSize(QSize (bt3->height(),bt3->height()));
    bt4->setIconSize(QSize (bt4->height(),bt4->height()));
    bt5->setIconSize(QSize (bt5->height(),bt5->height()));
}

void DIRControl::mousePressEvent(QMouseEvent *event)
{

}

void DIRControl::btn1_Clicked()
{
    if(!bt1->isCheckable()){
        bt1->setIcon(QIcon(":/stop.png"));
        emit btn_moveTo(DIRControl::On);
    }else{
        bt1->setIcon(QIcon(":/begin.png"));
        emit btn_moveTo(DIRControl::Off);
    }
    bt1->setCheckable(!bt1->isCheckable());
}

void DIRControl::btn2_Clicked()
{
    emit btn_moveTo(DIRControl::Top);
}

void DIRControl::btn3_Clicked()
{
    emit btn_moveTo(DIRControl::Bottom);
}

void DIRControl::btn4_Clicked()
{
    emit btn_moveTo(DIRControl::Left);
}

void DIRControl::btn5_Clicked()
{
    emit btn_moveTo(DIRControl::Right);
}
