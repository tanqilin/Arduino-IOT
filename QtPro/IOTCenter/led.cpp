#include "led.h"
#include <QPainter>


Led::Led(QWidget *parent) :
    QWidget(parent)
{
    m_Color = Qt::gray;
    setCursor(Qt::PointingHandCursor);
    lb = new QLabel(this);
}

void Led::mousePressEvent(QMouseEvent *event){
    emit LedClicked(0);
}

void Led::paintEvent(QPaintEvent *event)
{
    resize(width(),width());

    QPainter painter(this);

    QColor color = m_Color;
    painter.setPen(QPen(color, 3,Qt::DotLine));

    //gradient(圆心x,y，以中点线为圆心的半径，渐变的起点x,y)
    QRadialGradient gradient(width()/2, width()/2, width()-10, width()/2, width()/2);
    gradient.setColorAt(0, color);
    gradient.setColorAt(1, QColor::fromRgbF(0, 0, 0, 0));

    painter.setBrush(QBrush(color,Qt::Dense1Pattern));
    painter.drawEllipse(3, 3, width()-5, width()-5);
    painter.save();
    painter.restore();
    painter.end();
}

QColor Led::getColor()
{
    return Qt::red;
}

void Led::setColor(const QColor rgb)
{
    m_Color = rgb;
    update();
}
