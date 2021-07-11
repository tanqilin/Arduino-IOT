#ifndef DIRCONTROL_H
#define DIRCONTROL_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>

class DIRControl : public QWidget
{
    Q_OBJECT

public:
    DIRControl(QWidget *parent = 0);
    enum MoveTo{
        On = 0,
        Off,
        Top,
        Bottom,
        Left,
        Right,
    };

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent  *event);

private:
    QGridLayout *grid;
    QPushButton *bt1;
    QPushButton *bt2;
    QPushButton *bt3;
    QPushButton *bt4;
    QPushButton *bt5;

private slots:
    void btn1_Clicked();
    void btn2_Clicked();
    void btn3_Clicked();
    void btn4_Clicked();
    void btn5_Clicked();

signals:
    void btn_moveTo(DIRControl::MoveTo);
};

#endif
