#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
using namespace cv;
/******************************************
                串口枚举
******************************************/
typedef enum
{
    COM1,
    COM2,
    COM3,
    COM4,
    COM5,
    COM6,
    COM7,
    COM8,
    COM9,
    COM10,
    COM11,
    COM12,
    COM13,
    COM14,
    COM15,
}COM_Types;
/************************************************
 ui为MainWindow的对象，且MainWindow继承于QMainWindow
 ***********************************************/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    OPEN = true;
    setWindowTitle(tr("仿人手臂机器人调试系统"));
    ui->portNameComboBox->addItem("COM1",COM1);
    ui->portNameComboBox->addItem("COM2",COM2);
    ui->portNameComboBox->addItem("COM3",COM3);
    ui->portNameComboBox->addItem("COM4",COM4);
    ui->portNameComboBox->addItem("COM5",COM5);
    ui->portNameComboBox->addItem("COM6",COM6);
    ui->portNameComboBox->addItem("COM7",COM7);
    ui->portNameComboBox->addItem("COM8",COM8);
    ui->portNameComboBox->addItem("COM9",COM9);
    ui->portNameComboBox->addItem("COM10",COM10);
    ui->portNameComboBox->addItem("COM11",COM11);
    ui->portNameComboBox->addItem("COM12",COM12);
    ui->portNameComboBox->addItem("COM13",COM13);
    ui->portNameComboBox->addItem("COM14",COM14);
    ui->portNameComboBox->addItem("COM15",COM15);
    connect(ui->OpenCameraBtn,SIGNAL(click()),this,SLOT(on_OpenCameraBtn_clicked()));//打开摄像头按钮
    connect(ui->CloseCameraBtn,SIGNAL(click()),this,SLOT(on_CloseCameraBtn_clicked()));//关闭摄像头按钮
    connect(timer,SIGNAL(timeout()),this,SLOT(getFrame()));
}
/******************************
            打开摄像头
 *****************************/
void MainWindow::on_OpenCameraBtn_clicked()
{
    cam0.set(CAP_PROP_FRAME_HEIGHT, 480);// 设置摄像头的拍摄属性为 分辨率640x480，帧率30fps
    cam0.set(CAP_PROP_FRAME_WIDTH, 640);
    cam0.set(CAP_PROP_FPS, 30.0);
    cam0.open(0);//打开摄像头 从摄像头中获取视频
    cam1.open(1);
    if(cam0.isOpened()&&!cam1.isOpened())
    {
     cam1 = cam0;
    }
    if(!cam0.isOpened()&&cam1.isOpened())
    {
     cam0 = cam1;
    }
    timer->start(50);//开启定时器，每隔0.05秒刷新一次
}
void MainWindow::getFrame()
{
    if(cam0.isOpened())//从摄像头中抓取并返回每一帧
    {
        cam0 >> frame0;//将抓取到的帧转换成QImage格式
        QImage img0 = QImage((const unsigned char*)frame0.data, frame0.cols, frame0.rows, QImage::Format_RGB888).rgbSwapped();
        ui->label_2->setPixmap(QPixmap::fromImage(img0));
        waitKey(1000 / 30);//30帧
    }
    if(cam1.isOpened())
    {
        cam1 >> frame1;//将抓取到的帧转换成QImage格式
        QImage img1 = QImage((const unsigned char*)frame1.data, frame1.cols, frame1.rows, QImage::Format_RGB888).rgbSwapped();
        ui->label->setPixmap(QPixmap::fromImage(img1));//将图片显示到QLabel上
        waitKey(1000 / 30);//30帧
    }
}

/******************************
            关闭摄像头
 *****************************/
void MainWindow::on_CloseCameraBtn_clicked()
{
    if(cam0.isOpened()|cam1.isOpened())
      {
         int ret = QMessageBox::question(this," ", "确定要关闭吗?", QMessageBox::Ok |QMessageBox::Cancel);
         switch(ret)
         {
             case QMessageBox::Ok:
                 timer->stop();
                 cam0.release();
                 cam1.release();
                 OPEN = false;
                 write0.release();
                 write1.release();
                 ui->label->clear();
                 ui->label_2->clear();
                 break;
             case QMessageBox::Cancel:

                 break;
             default:
                 break;
         }
      }
}
/******************************************
           字符串转换成16进制数据
******************************************/
void MainWindow::StringToHex(QString str, QByteArray & senddata)
{
    int hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    senddata.resize(len/2);
    char lstr,hstr;
    for(int i=0; i<len; )
    {
        hstr=str[i].toLatin1();
        if(hstr == ' ')
        {
            i++;
            continue;
        }
        i++;
        if(i >= len)
            break;
        lstr = str[i].toLatin1();
        hexdata = ConvertHexChar(hstr);
        lowhexdata = ConvertHexChar(lstr);
        if((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata*16+lowhexdata;
        i++;
        senddata[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }
    senddata.resize(hexdatalen);
}
/******************************************
                    转换函数
******************************************/
char MainWindow::ConvertHexChar(char ch)
{
    if((ch >= '0') && (ch <= '9'))
            return ch-0x30;
        else if((ch >= 'A') && (ch <= 'F'))
            return ch-'A'+10;
        else if((ch >= 'a') && (ch <= 'f'))
            return ch-'a'+10;
        else return ch-ch;//不在0-f范围内的会发送成0
}

/**************************************************************
                            打开串口处理函数
***************************************************************/
void MainWindow::on_openPortBtn_clicked()
{
    if(ui->portNameComboBox->isEnabled())
        {
            ui->openPortBtn->setText("关闭串口");		//按下“打开串口”后，按键显示为“关闭串口”
            ui->portNameComboBox->setEnabled(false);//按下“打开串口”后，COM口组合框不可用
            ui->SendBtn->setEnabled(true);          //按下“打开串口”后，“发送数据”按钮可用
            ui->baudRateComboBox->setEnabled(false);//按下“打开串口”后，波特率组合框不可用
            ui->dataBitsComboBox->setEnabled(false);//按下“打开串口”后，数据位组合框不可用
            ui->parityComboBox->setEnabled(false);  //按下“打开串口”后，奇偶校验位组合框不可用
            ui->stopBitsComboBox->setEnabled(false);//按下“打开串口”后，停止位组合框不可用
            ui->sendTextEdit->setEnabled(true);
            serial.setPortName(ui->portNameComboBox->currentText());//设置COM口
            if(ui->baudRateComboBox->currentText()==tr("9600"))     //根据组合框内容对串口进行设置
               serial.setBaudRate(QSerialPort::Baud9600,QSerialPort::AllDirections);//设置波特率和读写方向
            else if(ui->baudRateComboBox->currentText()==tr("115200"))
               serial.setBaudRate(QSerialPort::Baud115200,QSerialPort::AllDirections);

            if(ui->dataBitsComboBox->currentText()==tr("8"))
               serial.setDataBits(QSerialPort::Data8);		//数据位为8位
            else if(ui->dataBitsComboBox->currentText()==tr("7"))
               serial.setDataBits(QSerialPort::Data7);		//数据位为7位
               serial.setFlowControl(QSerialPort::NoFlowControl);  //无流控制
            if(ui->parityComboBox->currentText()==tr("无"))
               serial.setParity(QSerialPort::NoParity);	    //无校验位
            else if(ui->parityComboBox->currentText()==tr("奇"))
               serial.setParity(QSerialPort::OddParity);	//奇校验位
            else if(ui->parityComboBox->currentText()==tr("偶"))
               serial.setParity(QSerialPort::EvenParity);	//偶校验位

            if(ui->stopBitsComboBox->currentText()==tr("1"))
               serial.setStopBits(QSerialPort::OneStop);	//一位停止位
            else if(ui->stopBitsComboBox->currentText()==tr("2"))
               serial.setStopBits(QSerialPort::TwoStop);	//二位停止位
               serial.close();					            //先关串口，再打开，可以保证串口不被其它函数占用。
            if(serial.open(QIODevice::ReadWrite))		    //以可读写的方式打开串口
            {
                connect(&serial,SIGNAL(readyRead()),this,SLOT(read_Com()));	//把串口的readyRead()信号绑定到read_Com()这个槽函数上
            }
        }
    else {
            ui->openPortBtn->setText("打开串口");	   //按下“关闭串口”后，按键显示为“打开串口”
            ui->portNameComboBox->setEnabled(true);//按下“关闭串口”后，COM口组合框可用
            ui->SendBtn->setEnabled(false);        //按下“关闭串口”后，“发送数据”按钮不可用
            ui->baudRateComboBox->setEnabled(true);//按下“关闭串口”后，波特率组合框可用
            ui->dataBitsComboBox->setEnabled(true);//按下“关闭串口”后，数据位组合框可用
            ui->parityComboBox->setEnabled(true);  //按下“关闭串口”后，奇偶校验位组合框可用
            ui->stopBitsComboBox->setEnabled(true);//按下“关闭串口”后，停止位组合框可用
            ui->sendTextEdit->setEnabled(false);
            serial.close();					       //关串口
        }
}
/******************************************
           接收数据处理函数
******************************************/
void MainWindow::read_Com()
{
    if(ui->setMode->currentText()==tr("字符串"))
    {
        QByteArray buf;
        buf = serial.readAll();
        if(buf!=NULL)
        {
            QString str = ui->recvTextBrowser->toPlainText();
            str+=tr(buf);//累加接收的数据
            ui->recvTextBrowser->clear();
            ui->recvTextBrowser->insertPlainText(str);//附加str字符串值
        }
    }

    else  if(ui->setMode->currentText()==tr("十六进制"))
    {
        QByteArray temp = serial.readAll();
        QDataStream out(&temp,QIODevice::ReadWrite);    //将字节数组读入
        while(!out.atEnd())
        {
            qint8 outChar = 0;
            out>>outChar;   //每字节填充一次，直到结束         
            QString str = QString("%1").arg(outChar & 0xFF,2,16,QLatin1Char('0'));//十六进制的转换
            ui->recvTextBrowser->insertPlainText(str.toUpper());//大写
            ui->recvTextBrowser->insertPlainText(" ");//每发送两个字符后添加一个空格
            ui->recvTextBrowser->moveCursor(QTextCursor::End);
        }
    }
}
/**************************************************************
                            发送数据处理函数
***************************************************************/
void MainWindow::on_SendBtn_clicked() //发送lineEdit窗口中的数据
{
    QString str = ui->sendTextEdit->toPlainText().toLatin1();//从LineEdit得到字符串
   if(!str.isEmpty())
   {
        int len =str.length();
        if(len%2 == 1)   //如果发送的数据个数为奇数的，则在前面最后落单的字符前添加一个字符0
            {
                str = str.insert(len-1,'0'); //insert(int position, const QString & str)
            }

        QByteArray senddata;

        if(ui->setMode_2->currentText()==tr("十六进制"))
        {
            StringToHex(str,senddata);//将str字符串转换为16进制的形式
            serial.write(senddata);//发送到串口
        }
        else if(ui->setMode_2->currentText()==tr("字符串"))
        {
            serial.write(ui->sendTextEdit->toPlainText().toLatin1());//发送到串口
        }
   }
}
MainWindow::~MainWindow()//析构函数
{
    delete ui;
}
void MainWindow::sendVal(QString val)
{
    QString str = val;
    int len =str.length();
    if(len%2 == 1)   //如果发送的数据个数为奇数的，则在前面最后落单的字符前添加一个字符0
        {
            str = str.insert(len-1,'0'); //insert(int position, const QString & str)
        }
    QByteArray senddata;
        StringToHex(str,senddata);//将str字符串转换为16进制的形式
        serial.write(senddata);//发送到串口
}
void MainWindow::sendVal_buf(QString val_buf)
{
    QString str = val_buf;
    int len =str.length();
    if(len%2 == 1)   //如果发送的数据个数为奇数的，则在前面最后落单的字符前添加一个字符0
        {
            str = str.insert(len-1,'0'); //insert(int position, const QString & str)
        }

        serial.write(str.toLatin1());//发送到串口
}
QString  MainWindow::motor_id(QString id)
{
     int motor_ids = id.toInt();
     QString ids;
     ids=QString::number(motor_ids,16);
     ids=ids.setNum(motor_ids,16);
     ui->jieguo6->setText(ids);
     int n=ids.size();
     if(n==1)
     {
       ids=ids.prepend("0");
      }
     ui->jieguo7->setText(ids);
     return ids;

}
QString  MainWindow::range_test_L(QString str1,QString str2,QString str3,QString str4,int str5)
{

    float sl = str1.toFloat();
    float sh = str2.toFloat();
    float al = str3.toFloat();
    float ah = str4.toFloat();
    float ip = str5;
    int op=((ip-al)*(sh-sl))/(ah-al)+sl;
    QString str6;
    str6=QString::number(op,16);
    str6=str6.setNum(op,16);
    ui->jieguo->setText(str6);


    int N=str6.size();

    QString str8;
    str8=QString::number(N,10);
    str8=str8.setNum(N,10);
    ui->jieguo2->setText(str8);
    if(N==3)
    {
      str6=str6.prepend("0");

    }
    if(N==2)
    {
      str6=str6.prepend("00");

    }
    if(N==1)
    {
      str6=str6.prepend("000");

    }
    QString str7;
    str7=str6.right(2);
    ui->jieguo1->setText(str7);
    ui->jieguo->setText(str6);
    QString str9;
    str9=str6.left(2);
    ui->jieguo3->setText(str9);
    return str7;

}
QString  MainWindow::range_test_H(QString str1,QString str2,QString str3,QString str4,int str5)
{

    float sl = str1.toFloat();
    float sh = str2.toFloat();
    float al = str3.toFloat();
    float ah = str4.toFloat();
    float ip = str5;
    int op=((ip-al)*(sh-sl))/(ah-al)+sl;
    QString str6;
    str6=QString::number(op,16);
    str6=str6.setNum(op,16);
    ui->jieguo->setText(str6);


    int N=str6.size();

    QString str8;
    str8=QString::number(N,10);
    str8=str8.setNum(N,10);
    ui->jieguo2->setText(str8);
    if(N==3)
    {
      str6=str6.prepend("0");

    }
    if(N==2)
    {
      str6=str6.prepend("00");

    }
    if(N==1)
    {
      str6=str6.prepend("000");

    }
    QString str7;
    str7=str6.right(2);
    ui->jieguo1->setText(str7);
    ui->jieguo->setText(str6);
    QString str9;
    str9=str6.left(2);
    ui->jieguo3->setText(str9);
    return str9;

}
void MainWindow::on_motor1_z_pressed()
{
    sendVal("55 55 08 03 01 E8 03 00 E8 03 ");
}

void MainWindow::on_motor1_f_pressed()
{
    sendVal("55 55 08 03 01 E8 03 00 6C 07 ");
}

void MainWindow::on_motor2_z_pressed()
{
    sendVal("55 55 08 03 01 E8 03 01 14 05 ");
}

void MainWindow::on_motor2_f_pressed()
{
    sendVal("55 55 08 03 01 E8 03 01 66 08 ");
}

void MainWindow::on_motor3_z_pressed()
{
    sendVal("55 55 08 03 01 E8 03 02 E8 03 ");
}

void MainWindow::on_motor3_f_pressed()
{
    sendVal("55 55 08 03 01 E8 03 02 9E 07 ");
}

void MainWindow::on_motor4_z_pressed()
{
    sendVal("55 55 08 03 01 E8 03 03 E8 03 ");
}

void MainWindow::on_motor4_f_pressed()
{
    sendVal("55 55 08 03 01 E8 03 03 98 08 ");
}

void MainWindow::on_motor5_z_pressed()
{
    sendVal("55 55 08 03 01 E8 03 04 F4 01 ");
}

void MainWindow::on_motor5_f_pressed()
{
    sendVal("55 55 08 03 01 E8 03 04 DC 05 ");
}

void MainWindow::on_motor6_z_pressed()
{
    sendVal("55 55 08 03 01 E8 03 05 98 08 ");
}

void MainWindow::on_motor6_f_pressed()
{
    sendVal("55 55 08 03 01 E8 03 05 72 06 ");
}

void MainWindow::on_actionO_triggered()
{
   on_OpenCameraBtn_clicked();
}

void MainWindow::on_actionC_triggered()
{
   on_CloseCameraBtn_clicked();
}



void MainWindow::on_set_motor6_editingFinished()
{
  serial.write(ui->set_motor6->text().toLatin1());//发送到串口
}

void MainWindow::on_r3_editingFinished()
{
serial.write(ui->d1->text().toLatin1());//发送到串口
serial.write(ui->d2->text().toLatin1());//发送到串口
serial.write(ui->d3->text().toLatin1());//发送到串口
serial.write(ui->r1->text().toLatin1());//发送到串口
serial.write(ui->r2->text().toLatin1());//发送到串口
serial.write(ui->r3->text().toLatin1());//发送到串口
}
void MainWindow::on_actiona_triggered()
{
QMessageBox::about(this, "关于复位机器人", "      视觉伺服控制是采用视觉传感器来直接或间接检测机器人当前位姿，或关于目标体的相对位姿"
                                               "然后对机器人进行定位控制或轨迹跟踪，即将机器视觉应用于反馈环对机器人进行控制，采用"
                                               "视觉伺服控制可以充分发挥机器视觉的优势使控制更为灵活和智能，帮助医生精确定位骨折情况，"
                                               "精确完成骨折复位。");
}
void MainWindow::on_motor1_z_2_clicked()
{
    sendVal("55 55 08 03 01 E8 03 06 20 03 ");
}

void MainWindow::on_motor1_f_2_clicked()
{
    sendVal("55 55 08 03 01 E8 03 06 40 06 ");
}


void MainWindow::on_motor2_z_2_pressed()
{
    sendVal("55 55 08 03 01 E8 03 07 20 03 ");
}

void MainWindow::on_motor2_f_2_pressed()
{
    sendVal("55 55 08 03 01 E8 03 07 08 07 ");
}

void MainWindow::on_motor3_z_2_pressed()
{
    sendVal("55 55 08 03 01 E8 03 08 D0 07 ");
}

void MainWindow::on_motor3_f_2_pressed()
{
    sendVal("55 55 08 03 01 E8 03 08 E8 03 ");
}

void MainWindow::on_motor4_z_2_pressed()
{
    sendVal("55 55 08 03 01 E8 03 09 20 03 ");
}

void MainWindow::on_motor4_f_2_pressed()
{
    sendVal("55 55 08 03 01 E8 03 09 40 06 ");
}

void MainWindow::on_motor5_z_2_pressed()
{
    sendVal("55 55 08 03 01 E8 03 0A E8 03 ");
}

void MainWindow::on_motor5_f_2_pressed()
{
    sendVal("55 55 08 03 01 E8 03 0A 08 07 ");
}

void MainWindow::on_motor6_z_2_pressed()
{
   sendVal("55 55 08 03 01 D0 07");
   sendVal(motor_id(ui->set_motor6->text()));
   sendVal(range_test_L(ui->set_motor1->text(),ui->set_motor2->text(),ui->set_motor3->text(),ui->set_motor4->text(),ui->motor0Value->value()));
   sendVal(range_test_H(ui->set_motor1->text(),ui->set_motor2->text(),ui->set_motor3->text(),ui->set_motor4->text(),ui->motor0Value->value()));

}

void MainWindow::on_motor6_f_2_pressed()
{

}



void MainWindow::on_motor1_z_3_clicked()
{
    sendVal("55 55 08 03 01 E8 03 1F 34 08 ");
}

void MainWindow::on_motor1_f_3_clicked()
{
    sendVal("55 55 08 03 01 E8 03 1F 4C 04 ");
}

void MainWindow::on_motor2_z_3_clicked()
{
    sendVal("55 55 08 03 01 E8 03 1E 14 05 ");
}

void MainWindow::on_motor2_f_3_clicked()
{
    sendVal("55 55 08 03 01 E8 03 1E B6 03 ");
}

void MainWindow::on_motor3_z_3_clicked()
{
    sendVal("55 55 08 03 01 E8 03 1D 98 08 ");
}

void MainWindow::on_motor3_f_3_clicked()
{
    sendVal("55 55 08 03 01 E8 03 1D B0 04 ");
}

void MainWindow::on_motor4_z_3_clicked()
{
    sendVal("55 55 08 03 01 E8 03 1C F4 01 ");
}

void MainWindow::on_motor4_f_3_clicked()
{
    sendVal("55 55 08 03 01 E8 03 1C 78 05 ");
}

void MainWindow::on_motor5_z_3_clicked()
{
    sendVal("55 55 08 03 01 E8 03 1B C4 09 ");
}

void MainWindow::on_motor5_f_3_clicked()
{
    sendVal("55 55 08 03 01 E8 03 1B DC 05 ");
}

void MainWindow::on_motor6_z_3_clicked()
{
    sendVal("55 55 08 03 01 E8 03 1A EB 03 ");
}

void MainWindow::on_motor6_f_3_clicked()
{
    sendVal("55 55 08 03 01 E8 03 1A DC 05 ");
}

void MainWindow::on_motor1_z_4_clicked()
{
    sendVal("55 55 08 03 01 E8 03 19 D0 07 ");
}

void MainWindow::on_motor1_f_4_clicked()
{
    sendVal("55 55 08 03 01 E8 03 19 B0 04 ");
}

void MainWindow::on_motor2_z_4_clicked()
{
    sendVal("55 55 08 03 01 E8 03 18 F4 01 ");
}

void MainWindow::on_motor2_f_4_clicked()
{
    sendVal("55 55 08 03 01 E8 03 18 08 07 ");
}

void MainWindow::on_motor3_z_4_clicked()
{
    sendVal("55 55 08 03 01 E8 03 17 34 08 ");
}

void MainWindow::on_motor3_f_4_clicked()
{
    sendVal("55 55 08 03 01 E8 03 17 E8 03 ");
}

void MainWindow::on_motor4_z_4_clicked()
{
    sendVal("55 55 08 03 01 E8 03 16 F4 01 ");
}

void MainWindow::on_motor4_f_4_clicked()
{
    sendVal("55 55 08 03 01 E8 03 16 D0 07 ");
}

void MainWindow::on_motor5_z_4_clicked()
{
    sendVal("55 55 08 03 01 E8 03 15 D0 07 ");
}

void MainWindow::on_motor5_f_4_clicked()
{
    //sendVal("55 55 08 03 01 E8 03 15 E8 03 ");
    sendVal("55 55 47 03 16 D0 07 00 6C 07 01 66 08 02 9E 07 03 98 08 04 DC 05 05 72 06 06 40 06 07 08 07 08 E8 03 09 40 06 0A 40 06 1F 4C 04 1E B6 03 1D B0 04 1C 78 05 1B DC 05 1A DC 05 15 E8 03 16 D0 07 17 E8 03 18 08 07 19 B0 04 ");
}

void MainWindow::on_motor0Value_sliderPressed()
{

}

void MainWindow::on_motor0Value_sliderMoved(int position)
{

}

