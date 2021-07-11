#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>

#include <VLCQtCore/Media.h>
#include <VLCQtCore/Common.h>
#include <VLCQtCore/Instance.h>
#include <VLCQtCore/MediaPlayer.h>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgproc/types_c.h"

using namespace std;
using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _instance = new VlcInstance(VlcCommon::args(), this);
    qDebug() << _instance->version();

    _player = new VlcMediaPlayer(_instance);
    ui->gridLayout->addWidget(ui->VideoWidget,2,2);
    ui->gridLayout->addWidget(ui->showImg,1,1);
    ui->gridLayout->addWidget(ui->pushButton,0,0);
    ui->gridLayout->addWidget(ui->pushButton_2,0,1);

    ui->gridLayout -> setColumnStretch(0, 1);
    ui->gridLayout -> setColumnStretch(1, 1);
    ui->gridLayout -> setColumnStretch(2, 1);
    // 行比   第0行与第1行之比为1:1
    ui->gridLayout -> setRowStretch(0, 1);
    ui->gridLayout -> setRowStretch(1, 1);
    ui->gridLayout -> setRowStretch(2, 1);

    _player->setVideoWidget(ui->VideoWidget);
}

MainWindow::~MainWindow()
{
    delete _player;
    delete _instance;
    delete _media;
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Open file"),
                                         QDir::homePath(),
                                         tr("Multimedia files(*)"));

    if (file.isEmpty())
        return;

    _media = new VlcMedia(file, true, _instance);
    _player->open(_media);
}

//显示图片
void MainWindow::on_pushButton_2_clicked()
{
//    Mat src = imread("/home/pi/Pictures/凌.jpeg");
//    cvtColor( src, src, CV_BGR2RGB );
//    QImage     img = QImage( (src.data), src.cols, src.rows,static_cast<int>(src.step), QImage::Format_RGB888 );
//    ui->showImg->setPixmap( QPixmap::fromImage(img));
}
