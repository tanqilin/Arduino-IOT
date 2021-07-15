#include "card.h"
#include <QPainter>
#include <QPixmap>
#include <QDebug>
#include <QMouseEvent>

Card::Card(QWidget *parent) :
    QWidget(parent),
    m_iconUrl("://number.png"),
    m_bkgColor(205,205,205),
    m_fontColor(50,50,50),
    m_titleSize(12),
    m_align(displayAlign::left),
    m_title("Card Title"),
    m_content("Card Content!"),
    m_font("Bitstream Charter"),
    m_contentFont("Bitstream Charter"),
    ctr_width(width()),
    ctr_height(height())
{
    setStyleSheet("widget:hover{background-color:#d9d9d9;color:#fefffe; top: 5px; left: 10px;}");
    setCursor(Qt::PointingHandCursor);
}

void Card::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setRenderHint(QPainter::Antialiasing);

    QPainterPath path;

    // 绘制大椭圆
    painter.setBrush(m_bkgColor);
    painter.setOpacity(0.8);
    if (m_model == 0) {
        resize(ctr_width,ctr_width/2.8);
        path.addRoundedRect(QRectF(0, 0, ctr_width, height()), 8, 8);
        painter.drawPath(path.simplified());

        // 获取要现实的title的宽度，用于左右中对齐
        QFontMetrics fm(m_font);
        QRect rec = fm.boundingRect(m_title);
        QRect conRec = fm.boundingRect(m_content);
        y_title = rec.height() + height()/8;
        y_content = y_title +conRec.height();
        if(!m_iconUrl.isEmpty()){
            QPixmap pix;
            pix.load(m_iconUrl);
            painter.drawPixmap(5,5,height()-10,height()-10,pix);

            switch (m_align) {
            case 1:
                x_title = height()+ (ctr_width-height()-rec.width())/2;
                x_content = height()+ (ctr_width-height()-conRec.width())/2;
                break;
            case 2:
                x_title = ctr_width-rec.width();
                x_content = ctr_width-conRec.width();
                break;
            default:
                x_title = height();
                x_content = height();break;
            }
        }else{
            switch (m_align) {
            case 1:
                x_title = (ctr_width-rec.width())/2;
                x_content =(ctr_width-conRec.width())/2;
                break;
            case 2:
                x_title = ctr_width-rec.width();
                x_content = ctr_width-conRec.width();
                break;
            default:
                x_title = 5;
                x_content = 5;break;
            }
        }
    }else{
        resize(ctr_width,ctr_width*1.2);
        path.addRoundedRect(QRectF(0, 0, width(), height()), 8, 8);
        painter.drawPath(path.simplified());

        // 获取要现实的title的宽度，用于左右中对齐
        QFontMetrics fm(m_font);
        QRect rec = fm.boundingRect(m_title);
        QRect conRec = fm.boundingRect(m_content);
        if(!m_iconUrl.isEmpty()){
            QPixmap pix;
            pix.load(m_iconUrl);
            painter.drawPixmap(5,5,ctr_width-10,ctr_width-10,pix);
            painter.drawText(5,ctr_width+5,m_title);
            y_title = ctr_width+10;
            y_content = ctr_width + conRec.height() +20;

        }else{
            y_title = rec.height() + height()/6;
            y_content = rec.height() + height()/6 + conRec.height() + 10;
        }

        switch (m_align) {
        case 1:
            x_title = (ctr_width-rec.width())/2;
            x_content = (ctr_width-conRec.width())/2;
            break;
        case 2:
            x_title = ctr_width-rec.width()-5;
            x_content = ctr_width-conRec.width()-5;
            break;
        default:
            x_title = 5;
            x_content = 5;break;
        }
    }

    // 现示文字
    painter.setPen(m_fontColor); //字体颜色
    m_font.setPointSize(m_titleSize);//字体大小
    painter.setFont(m_font);
    painter.drawText(x_title,y_title,m_title);
    m_contentFont.setPointSize(m_titleSize - 2);
    painter.setFont(m_contentFont);
    painter.drawText(x_content,y_content,m_content);

    painter.save();
    painter.restore();
    painter.end();
}

void Card::resizeEvent(QResizeEvent *event)
{
    ctr_width = width();
    if(m_model == 0){
        if(width() < 150){
            resize(150,60);
            ctr_width = 150;
        }
    }
    else{
        if(height()<100){
            resize(90,100);
            ctr_width = 90;
        }
    }
}

void Card::enterEvent(QEvent *event)
{
    ctr_width= ctr_width+10;
    update();
}

void Card::leaveEvent(QEvent *e)
{
    ctr_width= ctr_width-10;
    update();
}

// 鼠标按下
void Card::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        event->accept();
    } else {
        event->ignore();
    }
}

// 鼠标放开
void Card::mouseReleaseEvent(QMouseEvent *event)
{
    event->accept();
    emit Clicked();
}

// 自定义显示模式
Card::displayModel Card::getModel()
{
    return m_model;
}

void Card::setModel(displayModel model)
{

    m_model = model;
    update();
}

// 对齐方式
Card::displayAlign Card::getAlign()
{
    return m_align;
}

void Card::setAlign(displayAlign align)
{
    m_align = align;
    update();
}

// 自定义title
QString Card::getTitle()
{
    return m_title;
}

void Card::setTitle(QString title)
{
    m_title = title;
    update();
}

// icon
QString Card::getCardIcon()
{
    return m_iconUrl;
}

void Card::setCardIcon(QString icon)
{
    m_iconUrl = icon;
    update();
}

// 自定义content
QString Card::getContent()
{
    return m_content;
}

void Card::setContent(QString content)
{
    m_content = content;
    update();
}

// title颜色
QColor Card::getTitleColor()
{
    return m_fontColor;
}

void Card::setTitleColor(QColor color)
{
    m_fontColor = color;
}

// 背景颜色
QColor Card::getBkgColor()
{
    return m_bkgColor;
}

void Card::setBkgColor(QColor color)
{
    m_bkgColor = color;
    update();
}

// title大小
int Card::getTitleSize()
{
    return m_titleSize;
}

void Card::setTitleSize(int size)
{
   m_titleSize = size;
   update();
}

// 字体
QFont Card::getTextFont()
{
    return m_font;
}

void Card::setTextFont(QFont font)
{
    m_font = font;
    update();
}

