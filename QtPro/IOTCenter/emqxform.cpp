#include "emqxform.h"
#include "ui_emqxform.h"
#include <QtWebKit>
#include <QWebView>
#include <QStandardItemModel>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QPushButton>
#include <QMenu>

EmqxForm::EmqxForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EmqxForm)
{
    ui->setupUi(this);

    ui->tableWidget->setShowGrid(false);
    ui->tableWidget->setColumnWidth(0,300);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//整行选中的方式
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//禁止修改
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);//可以选中单个

    ui->tableWidget2->setShowGrid(false);
    ui->tableWidget2->setEditTriggers(QAbstractItemView::NoEditTriggers);//禁止修改
    ui->tableWidget2->setSelectionBehavior(QAbstractItemView::SelectRows);//整行选中的方式

    ui->tableWidget->setContextMenuPolicy (Qt::CustomContextMenu);
    connect(ui->tableWidget,SIGNAL(customContextMenuRequested(const QPoint&)),this, SLOT(showContextMenu(const QPoint&)));
}

EmqxForm::~EmqxForm()
{
    delete ui;
}

// tab切换
void EmqxForm::on_tabWidget_currentChanged(int index)
{
    QNetworkAccessManager *accessManager = new QNetworkAccessManager(this);
    connect(accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finishedSlot(QNetworkReply*)));
    QNetworkRequest request;

    QString concatenated = "admin:public";

    QByteArray data = concatenated.toLocal8Bit().toBase64();
    QString headerData = "Basic " + data;
    request.setRawHeader("Authorization", headerData.toLocal8Bit());
    switch(index){
    case 0:
        this->initTab0Page();
        break;
    case 1:
        request.setUrl(QUrl("http://127.0.0.1:8081/api/v4/clients"));
        break;
    case 2:
        request.setUrl(QUrl("http://127.0.0.1:8081/api/v4/subscriptions"));
        break;
    }
    //get
    if(!request.url().isEmpty())
        accessManager->get(request);
}

//请求结果
void EmqxForm::finishedSlot(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();
        switch(ui->tabWidget->currentIndex()){
        case 1:
            initTab1Page(bytes);
             break;
        case 2:
            initTab2Page(bytes);
             break;
        }
    }
    else
     {
         qDebug() << "finishedSlot errors here";
         qDebug( "found error .... code: %d\n", (int)reply->error());
         qDebug(qPrintable(reply->errorString()));
     }
    reply->deleteLater();
}

//菜单
void EmqxForm::showContextMenu(const QPoint &)
{

    QMenu *cmenu = new QMenu(ui->tableWidget->horizontalHeader());

    QAction *ascendSortAction = cmenu->addAction("查看");
    QAction *descendSortAction = cmenu->addAction("踢除");
    QAction *filterAction = cmenu->addAction("控制页面");
    QAction *reshowAction = cmenu->addAction("刷新");

//    connect(ascendSortAction, SIGNAL(triggered(bool)), this, SLOT(sort_ascend()));
//    connect(descendSortAction, SIGNAL(triggered(bool)), this, SLOT(sort_descend()));
//    connect(filterAction, SIGNAL(triggered(bool)), this, SLOT(show_filter_dlg()));
//    connect(reshowAction, SIGNAL(triggered(bool)), this, SLOT(reshow_data()));

    cmenu->exec(QCursor::pos());//在当前鼠标位置显示
}

void EmqxForm::initTab0Page()
{
    ui->webView->load(QUrl("http://127.0.0.1:18083"));
}

void EmqxForm::initTab1Page(QByteArray res)
{
    QString resultData = res;
    QJsonDocument resJson = QJsonDocument::fromJson(res);
    QJsonObject resJsonObj = resJson.object();

    // 解析code节点
    if(resJsonObj["code"].toInt() == 0){
        //解析mate节点
        if(resJsonObj.contains(QStringLiteral("meta"))){
            QJsonObject meta =  resJsonObj.value("meta").toObject();
            int count = meta["count"].toInt();
            int page = meta["page"].toInt();
            int limit = meta["limit"].toInt();
        }

        // 解析data节点
        if(resJsonObj.contains(QStringLiteral("data"))){
             QJsonValue arrayValue = resJsonObj.value(QStringLiteral("data"));
             if(arrayValue.isArray()){

                ui->tableWidget->setRowCount(0);
                ui->tableWidget->clearContents();

                QJsonArray array = arrayValue.toArray();
                for(int i=0;i<array.size();i++)
                {
                     QJsonObject node = array.at(i).toObject();

                     QString id = node["clientid"].toString();
                     QString username = node["username"].toString();
                     QString ip = node["ip_address"].toString();
                     QString nodeName = node["node"].toString();
                     int port = node["port"].toInt();
                     int subCount = node["subscriptions_cnt"].toInt();
                     bool connected = node["connected"].toBool();
                     QString connectTime = node["connected_at"].toString();
                     QString createTime = node["created_at"].toString();

                     ui->tableWidget->insertRow(i);
                     ui->tableWidget->setItem(i,0,new QTableWidgetItem(id));
                     ui->tableWidget->setItem(i,1,new QTableWidgetItem(username));
                     ui->tableWidget->setItem(i,2,new QTableWidgetItem(ip+":"+QString::number(port)));
                     ui->tableWidget->setItem(i,3,new QTableWidgetItem(QString::number(subCount)));

                     QTableWidgetItem * statusItem = new QTableWidgetItem(connected?"已连接":"断开");
                     if(connected)
                         statusItem->setTextColor(Qt::blue);
                     else {
                         statusItem->setTextColor(Qt::red);
                     }
                     ui->tableWidget->setItem(i,4,statusItem);
                     ui->tableWidget->setItem(i,5,new QTableWidgetItem(nodeName));
                     ui->tableWidget->setItem(i,6,new QTableWidgetItem(connectTime));
                     ui->tableWidget->setItem(i,7,new QTableWidgetItem(createTime));
                 }
            }
        }
    }
}

// 主题
void EmqxForm::initTab2Page(QByteArray res)
{
    QJsonDocument resJson = QJsonDocument::fromJson(res);
    QJsonObject resJsonObj = resJson.object();

    // 解析code节点
    if(resJsonObj["code"].toInt() == 0){
        //解析mate节点
        if(resJsonObj.contains(QStringLiteral("meta"))){
            QJsonObject meta =  resJsonObj.value("meta").toObject();
            int count = meta["count"].toInt();
            int page = meta["page"].toInt();
            int limit = meta["limit"].toInt();
        }


        // 解析data节点
        if(resJsonObj.contains(QStringLiteral("data"))){
             QJsonValue arrayValue = resJsonObj.value(QStringLiteral("data"));
             if(arrayValue.isArray()){

                ui->tableWidget2->setRowCount(0);
                ui->tableWidget2->clearContents();

                QJsonArray array = arrayValue.toArray();
                for(int i=0;i<array.size();i++)
                {
                     QJsonObject node = array.at(i).toObject();

                     QString topic = node["topic"].toString();
                     int qos = node["qos"].toInt();
                     QString nodeName = node["node"].toString();
                     QString clientid = node["clientid"].toString();

                     ui->tableWidget2->insertRow(i);
                     ui->tableWidget2->setItem(i,0,new QTableWidgetItem(topic));
                     ui->tableWidget2->setItem(i,1,new QTableWidgetItem(QString::number(qos)));
                     ui->tableWidget2->setItem(i,2,new QTableWidgetItem(nodeName));
                     ui->tableWidget2->setItem(i,3,new QTableWidgetItem(clientid));
                }
             }
        }
    }
    qDebug()<<res;
}
