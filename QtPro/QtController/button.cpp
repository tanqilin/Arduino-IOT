#include "button.h"
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>

Button::Button(QWidget *parent) :
    QWidget(parent),
    m_bChecked(false),
    m_checkedColor(0, 150, 136),
    m_background(Qt::gray)
{
    // 鼠标滑过光标形状 - 手型
       setCursor(Qt::PointingHandCursor);
       // 连接信号槽
       connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
       m_nX = 0;
}

//设置开关选中颜色
void Button::setCheckedColor(QColor color)
{
    m_checkedColor = color;
}

void Button::setToggle(bool checked)
{
    m_bChecked = checked;
    m_timer.start(5);
}

void Button::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setRenderHint(QPainter::Antialiasing);
    QColor color(0x00,0x00,0x00);

    QPainterPath path;

    QColor background;
    QColor thumbColor;
    qreal dOpacity;
    if (m_bChecked) { // 打开状态
        background = m_checkedColor;
        thumbColor = m_checkedColor;
        dOpacity = 0.600;
    } else { //关闭状态
        background = m_background;
        thumbColor = Qt::white;
        dOpacity = 0.800;
    }

    // 绘制大椭圆
    painter.setBrush(background);
    painter.setOpacity(dOpacity);
    path.addRoundedRect(QRectF(5, 5, width() - 10, height()-10 ), 16, 16);
    painter.drawPath(path.simplified());

    // 开关上的圆
    painter.setBrush(thumbColor);
    painter.setOpacity(1.0);
    painter.drawEllipse(QRectF(m_nX, 0, height(), height()));
}

// 鼠标按下
void Button::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        event->accept();
    } else {
        event->ignore();
    }
}

// 鼠标放开
void Button::mouseReleaseEvent(QMouseEvent *event)
{
    event->accept();
    m_bChecked = !m_bChecked;
    emit Clicked(m_bChecked);
    m_timer.start(5);
}

void Button::resizeEvent(QResizeEvent *event)
{
    if(!m_bChecked){
        m_nX = 0;
    }else{
        m_nX = width()-height();
    }
}

void Button::onTimeout()
{
    if(m_bChecked){
        m_nX += 1;
        if(m_nX >= width()-height()) m_timer.stop();
    }else{
        m_nX -= 1;
        if(m_nX <= 0) m_timer.stop();
    }
    update();
}
