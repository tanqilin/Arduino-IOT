#ifndef CENTERWINDOW_H
#define CENTERWINDOW_H

#include <QMainWindow>
#include <QHostAddress>
#include "mqtt/qmqtt.h"
#include "dircontrol.h"

namespace Ui {
class CenterWindow;
}

class CenterWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CenterWindow(QWidget *parent = nullptr);
    ~CenterWindow();

private slots:
    void onMQTT_Received( QMQTT::Message msg);
    void onMQTT_Connected();
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_led_LedClicked(const int8_t &stat);

    void on_dIRControl_btn_moveTo(const DIRControl::MoveTo &arg1);

    void on_color_SelectColor(const QColor &arg1);

    void on_button_Clicked(bool check);

private:
    QMQTT::Client *client;
    Ui::CenterWindow *ui;

    void sendMqttMSG(QMap<QString,int> *mp);
};

#endif // CENTERWINDOW_H
