#ifndef COLOR_H
#define COLOR_H

#include <QWidget>
#include <QMouseEvent>

class Color : public QWidget
{
    Q_OBJECT

public:
    Color(QWidget *parent = 0);

private:
    QWidget *tip;
    QPoint *mouseXy;
    QImage *image;    //色卡图片
    QColor selectColor;

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent  *event);

signals:
    void SelectColor(QColor);
};

#endif
