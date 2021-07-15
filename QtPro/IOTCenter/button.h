#ifndef BUTTON_H
#define BUTTON_H

#include <QWidget>
#include <QTimer>

class Button : public QWidget
{
    Q_OBJECT

public:
    Button(QWidget *parent = 0);

    void setCheckedColor(QColor color);     // 设置开关选中颜色
    void setToggle(bool checked);           // 设置开关状态
    bool getToggle();

protected:
    void paintEvent(QPaintEvent *event);
    // 鼠标按下事件
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    // 鼠标释放事件 - 切换开关状态、发射toggled()信号
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event);

private slots:
    void onTimeout();

private:
    bool m_bChecked;         // 是否选中
    QColor m_checkedColor;   // 选中时的颜色
    QColor m_background;     // 按钮背景色
    QTimer m_timer;          // 定时器
    qreal m_nX;              // x点坐标
    qreal m_nY;              // y点坐标

signals:
    void Clicked(bool check);
};

#endif
