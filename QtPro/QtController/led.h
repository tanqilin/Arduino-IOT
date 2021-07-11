#ifndef LED_H
#define LED_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

class Led : public QWidget
{
    Q_OBJECT
    // 添加自定义属性
    //Q_ENUMS(DisplayMode)
    Q_PROPERTY(QColor displayColor READ getColor WRITE setColor)

public:
    Led(QWidget *parent = 0);
    void setColor(QColor rgb);

signals:
    void LedClicked(int8_t stat);

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

    QColor getColor();
private:
    //Ui::Widget * ui;
    QLabel *lb;
    QColor m_Color;
};

#endif
