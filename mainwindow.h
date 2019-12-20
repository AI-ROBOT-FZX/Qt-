#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <opencv2/opencv.hpp>
#include <QMainWindow>
#include <opencv2/video.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <QTimer>
#include <QDialog>
#include <QMenuBar>
#include <QMenu>
#include <QLabel>
#include <QDebug>
#include <QAction>
#include <QToolBar>
#include <QPushButton>
#include <QStatusBar>
#include <QMessageBox>
#include <QFileDialog>
using namespace cv;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_OpenCameraBtn_clicked();
    void getFrame();
    void sendVal(QString str);
    void sendVal_buf(QString val_buf);
    QString motor_id(QString id);
    QString range_test_L(QString str1,QString str2,QString str3,QString str4,int str5);
    QString range_test_H(QString str1,QString str2,QString str3,QString str4,int str5);
    void on_CloseCameraBtn_clicked();
    void on_openPortBtn_clicked();//利用“转到槽”添加的槽函数声明
    void on_SendBtn_clicked();
    void read_Com();			//手动添加的槽函数声明，用于读出串口缓冲区的内容
    void StringToHex(QString str, QByteArray &senddata);
    char ConvertHexChar(char ch);  
    void on_motor1_z_pressed();
    void on_motor1_f_pressed();
    void on_motor2_z_pressed();
    void on_motor2_f_pressed();
    void on_motor3_z_pressed();
    void on_motor3_f_pressed();
    void on_motor4_z_pressed();
    void on_motor4_f_pressed();
    void on_motor5_z_pressed();
    void on_motor5_f_pressed();
    void on_motor6_z_pressed();
    void on_motor6_f_pressed();
    void on_actionO_triggered();
    void on_actionC_triggered();
    void on_set_motor6_editingFinished();
    void on_r3_editingFinished();
    void on_actiona_triggered();
    void on_motor2_z_2_pressed();

    void on_motor2_f_2_pressed();

    void on_motor3_z_2_pressed();

    void on_motor3_f_2_pressed();

    void on_motor4_z_2_pressed();

    void on_motor4_f_2_pressed();

    void on_motor5_z_2_pressed();

    void on_motor5_f_2_pressed();

    void on_motor6_z_2_pressed();

    void on_motor6_f_2_pressed();

    void on_motor1_z_2_clicked();

    void on_motor1_f_2_clicked();

    void on_motor1_z_3_clicked();

    void on_motor1_f_3_clicked();

    void on_motor2_z_3_clicked();

    void on_motor2_f_3_clicked();

    void on_motor3_z_3_clicked();

    void on_motor3_f_3_clicked();

    void on_motor4_z_3_clicked();

    void on_motor4_f_3_clicked();

    void on_motor5_z_3_clicked();

    void on_motor5_f_3_clicked();

    void on_motor6_z_3_clicked();

    void on_motor6_f_3_clicked();

    void on_motor1_z_4_clicked();

    void on_motor1_f_4_clicked();

    void on_motor2_z_4_clicked();

    void on_motor2_f_4_clicked();

    void on_motor3_z_4_clicked();

    void on_motor3_f_4_clicked();

    void on_motor4_z_4_clicked();

    void on_motor4_f_4_clicked();

    void on_motor5_z_4_clicked();

    void on_motor5_f_4_clicked();

    void on_motor0Value_sliderPressed();

    void on_motor0Value_sliderMoved(int position);

private:
    Ui::MainWindow *ui;
    Mat frame0,frame1;//摄像头每次抓取的图像为一帧，使用该指针指向一帧图像的内存空间
    QTimer *timer;//定时器用于定时取帧，上面说的隔一段时间就去取就是用这个实现
    VideoCapture cam0,cam1;
    VideoWriter write0,write1;
    QSerialPort serial;			//声明串口类
    bool OPEN;
};

#endif // MAINWINDOW_H
