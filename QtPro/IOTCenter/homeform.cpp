#include "homeform.h"
#include "ui_homeform.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardItemModel>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>

HomeForm::HomeForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomeForm)
{
    ui->setupUi(this);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("./home.db");
        bool ok = db.open();
        if (ok){
            QSqlQuery *query = new QSqlQuery();
            //创建传感器数据库
            query->exec("CREATE TABLE sensor(clientId varchar(64) PRIMARY KEY, username varchar(64), password varchar(64))");
            query->clear();
        }
        else {
            QMessageBox::information(this, "infor", "open failed");
            qDebug()<<"error open database because"<<db.lastError().text();
        }

    QStandardItemModel* model = new QStandardItemModel(ui->treeView);
    //构造1级节点
    QStandardItem* item1 = new QStandardItem(QIcon("://tree/icon/folder.png"),QStringLiteral("传感器"));
    QStandardItem* item2 = new QStandardItem(QIcon("://tree/icon/dataNode.png"),QStringLiteral("数据采集"));
    model->appendRow(item1);
    model->appendRow(item2);

    //构造2级节点
    QStandardItem* item11 = new QStandardItem(QIcon("://tree/icon/switch.png"),QStringLiteral("开关类"));
    QStandardItem* item12 = new QStandardItem(QIcon("://tree/icon/lightIcon.png"),QStringLiteral("照明类"));
    item1->appendRow(item11);
    item1->appendRow(item12);

    QStandardItem* item21 = new QStandardItem(QIcon("://tree/icon/temp.png"),QStringLiteral("温度"));
    QStandardItem* item22 = new QStandardItem(QIcon("://tree/icon/humidity.png"),QStringLiteral("温度"));
    item2->appendRow(item21);
    item2->appendRow(item22);

    //在QTreeView中应用上面构造的model
    ui->treeView->setHeaderHidden(true);
    ui->treeView->setModel(model);
    ui->treeView->expandAll();

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

HomeForm::~HomeForm()
{
    delete ui;
}

//接受订阅信息
void HomeForm::onMQTT_Received( QMQTT::Message msg)
{
    QString str = msg.payload();
    qDebug() <<"rcive: " << msg.topic() << ":" <<str;

}

void HomeForm::onMQTT_Connected()
{
    qDebug() <<"MQTT:Cennected successfuly...";
}

// 色卡
void HomeForm::on_color_SelectColor(const QColor &arg1)
{
    if(!ui->button->getToggle()) return;
    QMap<QString,int> map1;
    map1.insert("order",2);
    map1.insert("r", arg1.red());
    map1.insert("g", arg1.green());
    map1.insert("b", arg1.blue());
    this->sendMqttMSG(&map1);
}
// 开关
void HomeForm::on_button_Clicked(bool check)
{
    QMap<QString,int> map1;
    map1.insert("order",check?0:1);
    this->sendMqttMSG(&map1);
}

// 发送消息
void HomeForm::sendMqttMSG(QMap<QString, int> *mp)
{
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
