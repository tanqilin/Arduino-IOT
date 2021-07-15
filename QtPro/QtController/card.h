#ifndef CARD_H
#define CARD_H

#include <QWidget>

class Card : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(displayModel displayModel READ getModel WRITE setModel)
    Q_PROPERTY(displayAlign displayAlign READ getAlign WRITE setAlign)
    Q_PROPERTY(QString cardIcon READ getCardIcon WRITE setCardIcon)
    Q_PROPERTY(QString cardTitle READ getTitle WRITE setTitle)
    Q_PROPERTY(QString cardContent READ getContent WRITE setContent)
    Q_PROPERTY(QColor bkgColor READ getBkgColor WRITE setBkgColor)
    Q_PROPERTY(QColor titleColor READ getTitleColor WRITE setTitleColor)
    Q_PROPERTY(int fontSize READ getTitleSize WRITE setTitleSize)
    Q_PROPERTY(QFont textFont READ getTextFont WRITE setTextFont)

public:
    enum displayModel{
        horizontal = 0,
        vertical,
    };

    enum displayAlign{
        left = 0,
        center,
        right
    };

    Q_ENUMS(displayModel)
    Q_ENUMS(displayAlign)
public:
    Card(QWidget *parent = 0);
    displayModel getModel();
    void setModel(displayModel model);

    displayAlign getAlign();
    void setAlign(displayAlign align);

    QString  getTitle();
    void setTitle(QString title);

    QString  getCardIcon();
    void setCardIcon(QString icon);

    QString  getContent();
    void setContent(QString content);

    QColor  getTitleColor();
    void setTitleColor(QColor color);

    QColor  getBkgColor();
    void setBkgColor(QColor color);

    int  getTitleSize();
    void setTitleSize(int size);

    QFont  getTextFont();
    void setTextFont(QFont font);

protected:
    // 鼠标按下事件
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *e);

private:
    QString m_iconUrl;
    QColor m_bkgColor;
    QColor m_fontColor;     // 自体颜色
    int m_titleSize;        // title大小
    displayModel m_model;
    displayAlign m_align;   // text位置

    QString m_title;
    QString m_content;      // content text

    QFont m_font;
    QFont m_contentFont;

    int ctr_width;          //控件大小
    int ctr_height;

    int x_title;            // title位置
    int y_title;

    int x_content;          // content 位置
    int y_content;

signals:
    void Clicked();
};

Q_DECLARE_METATYPE(Card::displayModel)
Q_DECLARE_METATYPE(Card::displayAlign)
#endif
