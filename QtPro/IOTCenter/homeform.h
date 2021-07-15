#ifndef HOMEFORM_H
#define HOMEFORM_H

#include <QWidget>
#include <QHostAddress>
#include "mqtt/qmqtt.h"

namespace Ui {
class HomeForm;
}

class HomeForm : public QWidget
{
    Q_OBJECT

public:
    explicit HomeForm(QWidget *parent = nullptr);
    ~HomeForm();

private slots:
    void onMQTT_Received( QMQTT::Message msg);
    void onMQTT_Connected();

    void on_color_SelectColor(const QColor &arg1);

    void on_button_Clicked(bool check);

private:
    Ui::HomeForm *ui;
    QMQTT::Client *client;

    void sendMqttMSG(QMap<QString,int> *mp);
};

#endif // HOMEFORM_H
