#ifndef EMQXFORM_H
#define EMQXFORM_H

#include <QWidget>
#include<QNetworkAccessManager>
#include<QNetworkReply>

namespace Ui {
class EmqxForm;
}

class EmqxForm : public QWidget
{
    Q_OBJECT

public:
    explicit EmqxForm(QWidget *parent = nullptr);
    ~EmqxForm();

private slots:
    void on_tabWidget_currentChanged(int index);
    void finishedSlot(QNetworkReply *reply);
    void showContextMenu(const QPoint&);
private:
    Ui::EmqxForm *ui;

    void initTab0Page();
    void initTab1Page(QByteArray res);
    void initTab2Page(QByteArray res);
    void initTab3Page();
};

#endif // EMQXFORM_H
