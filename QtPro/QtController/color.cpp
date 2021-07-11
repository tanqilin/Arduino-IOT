#include "color.h"
#include <QPainter>
#include <QDebug>
#include <QLabel>

Color::Color(QWidget *parent) :
    QWidget(parent)
{
    setMinimumSize(100,100);
    mouseXy = new QPoint();
}

void Color::paintEvent(QPaintEvent *event)
{
    resize(width(),width());

    QPainter *painter = new QPainter(this);
    painter->setRenderHint(QPainter::Antialiasing);
    QColor color(0x00,0x00,0x00);

    QRectF outRect(0,0, width(),width());
    painter->setPen(Qt::NoPen);
    //弧度渐变
    QConicalGradient conicalGradient(width()/2,width()/2,0);
    //创建了一个QConicalGradient对象实例，参数分别为中心坐标和初始角度
    conicalGradient.setColorAt(0,Qt::green);
    conicalGradient.setColorAt(0.1,Qt::white);
    conicalGradient.setColorAt(0.2,Qt::blue);
    conicalGradient.setColorAt(0.4,Qt::gray);
    conicalGradient.setColorAt(0.6,Qt::red);
    conicalGradient.setColorAt(0.8,Qt::yellow);
    conicalGradient.setColorAt(1,Qt::green);
    //设置渐变的颜色和路径比例
    painter->setBrush(QBrush(conicalGradient));
    painter->drawPie(outRect,0,360*16);


    //画内圆
    QRectF inRect(width()/2-20, width()/2-20, 40, 40);
    painter->setBrush(palette().window().color());
    painter->drawPie(inRect, 0, 360*16);
    painter->save();

    painter->setPen(Qt::black);//字体颜色
    if(!mouseXy->isNull() && selectColor.isValid()){
        painter->drawPie(QRect(mouseXy->x()-5,mouseXy->y()-5,5,5), 0, 360*16);

        QFont font;
        font.setPointSize(8);//字体大小
        painter->setFont(font);

        if(this->mouseXy->x()+60 > width()) this->mouseXy->setX(this->mouseXy->x()-60);
        painter->drawText(mouseXy->x(),mouseXy->y()+10,QString("R:%1 G:%2 B:%3")
                          .arg(selectColor.red()).arg(selectColor.green()).arg(selectColor.blue()));

    }
}

void Color::mousePressEvent(QMouseEvent  *event)
{
    QPixmap map = grab(QRect(0,0,width(),width()));
    if (!map.isNull()) {
        QImage image = map.toImage();

        if (!image.isNull()) {
            if (image.valid(0, 0)) {
                selectColor= image.pixel(event->x(), event->y());
                emit SelectColor(selectColor);
                // 十六进制
//                QString strRed = tr("%1").arg(selectColor.red() & 0xFF, 2, 16, QChar('0'));
//                QString strGreen = tr("%1").arg(color.red() & 0xFF, 2, 16, QChar('0'));
//                QString strBlue = tr("%1").arg(color.red() & 0xFF, 2, 16, QChar('0'));
            }
        }
    }

    this->mouseXy = new QPoint (event->x(),event->y());
    if(event->x()+60 > width()) this->mouseXy->setX(event->x()-60);
    update();
}
