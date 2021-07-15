#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "emqxform.h"
#include "homeform.h"
#include "centerwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_card_Clicked();

    void on_card_2_Clicked();

private:
    Ui::MainWindow *ui;
    EmqxForm *emqx;
    HomeForm *home;
};

#endif // MAINWINDOW_H
