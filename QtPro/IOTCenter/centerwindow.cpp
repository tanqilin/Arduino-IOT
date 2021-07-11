#include "centerwindow.h"
#include "ui_centerwindow.h"
#include <QColorDialog>
#include <QDebug>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>

CenterWindow::CenterWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CenterWindow)
{
    ui->setupUi(this);

    ui->button->setCheckedColor(QColor(245,12,0));
    client = new QMQTT::Client(QHostAddress("192.168.1.4"), 1883);
    connect(client,SIGNAL(connected()),this,SLOT(onMQTT_Connected()));
    connect(client, SIGNAL(received(QMQTT::Message)),this, SLOT(onMQTT_Received(QMQTT::Message)));

    client->setKeepAlive(120); // 心跳
    client->setClientId("iot-center-admin");
    client->setUsername("iotcenter");
    //client->setPassword("password");
    client->connectToHost();
    QTimer::singleShot(1000, this, [=](){
        client->subscribe("tanqilin/in@center",0); // 订阅tanqilin/home
    });
}

CenterWindow::~CenterWindow()
{
    delete client;
    delete ui;
}

// 链接成功
void CenterWindow::onMQTT_Connected(){
    qDebug() <<"MQTT:Cennected successfuly...";
}

//接受订阅信息
void CenterWindow::onMQTT_Received( QMQTT::Message msg)
{
    QString str = msg.payload();
    qDebug() <<"rcive: " << msg.topic() << ":" <<str;

}

void CenterWindow::on_pushButton_clicked()
{
    QColorDialog color;
    QColor c = color.getRgba();
    ui->led->setColor(c);

    QMap<QString,int> map1;
    map1.insert("order",2);
    map1.insert("r", c.red());
    map1.insert("g", c.green());
    map1.insert("b", c.blue());
    this->sendMqttMSG(&map1);
}

// 关灯
void CenterWindow::on_pushButton_2_clicked()
{
   QMap<QString,int> map1;
   map1.insert("order",1);
   this->sendMqttMSG(&map1);
}

void CenterWindow::on_led_LedClicked(const int8_t &stat)
{

}

void CenterWindow::on_dIRControl_btn_moveTo(const DIRControl::MoveTo &arg1)
{
    QMap<QString,int> map1;
    switch (arg1) {
    case 0:
        map1.insert("order",0);
        this->sendMqttMSG(&map1);
        ui->button->setToggle(true);
        break;
    case 1:
        map1.insert("order",1);
        this->sendMqttMSG(&map1);
        ui->button->setToggle(false);
        break;
    }
}

void CenterWindow::sendMqttMSG(QMap<QString,int> *mp){
    QJsonObject ob;
    QMap<QString,int>::Iterator it=mp->begin();
    while(it!=mp->end())
    {
        ob.insert(it.key(), it.value());
        it++;
    }
    QJsonDocument doc;
    doc.setObject(ob);
    QString data = doc.toJson(QJsonDocument::Compact);
    QMQTT::Message msg(136,QString("tanqilin/out@center"),data.toUtf8());
    client->publish(msg);
}

// 色卡颜色
void CenterWindow::on_color_SelectColor(const QColor &arg1)
{
    QMap<QString,int> map1;
    map1.insert("order",2);
    map1.insert("r", arg1.red());
    map1.insert("g", arg1.green());
    map1.insert("b", arg1.blue());
    this->sendMqttMSG(&map1);
    ui->led->setColor(arg1);
}

void CenterWindow::on_button_Clicked(bool check)
{
    QMap<QString,int> map1;
    if(check){
        map1.insert("order",0);
        this->sendMqttMSG(&map1);
    }else{
        map1.insert("order",1);
        this->sendMqttMSG(&map1);
    }
}
