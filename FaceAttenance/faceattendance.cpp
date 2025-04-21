#include "faceattendance.h"
#include "ui_faceattendance.h"
#include <QImage>
#include <QPainter>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
FaceAttendance::FaceAttendance(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FaceAttendance)
{
    ui->setupUi(this);
    //打开摄像头
    cap.open(0);
    //启动定时器事件
    startTimer(100);

    //导入级联分类器文件
    cascade.load("D:/opencv4.5.4/opencv-4.5.4/build/install/etc/haarcascades/haarcascade_frontalface_alt2.xml");

    //QTcpSocket当断开连接的时候disconnected信号，连接成功发送connected
    connect(&msocket, &QTcpSocket::disconnected, this, &FaceAttendance::start_connect);
    connect(&msocket, &QTcpSocket::connected, this, &FaceAttendance::stop_connect);
    //关联接收数据的槽函数
    connect(&msocket, &QTcpSocket::readyRead, this, &FaceAttendance::recv_data);

    //定时器连接
    connect(&mtimer, &QTimer::timeout, this, &FaceAttendance::timer_connect);
    //启动定时器
    mtimer.start(5000);//每5s连接一次，直到成功之后不再连接

    flag = 0;

    ui->widgetLb->hide();
}

FaceAttendance::~FaceAttendance()
{
    delete ui;
}

void FaceAttendance::timerEvent(QTimerEvent *e)
{
    //采集数据
    Mat srcImage;
    if(cap.grab())
    {
        cap.read(srcImage);//读取一帧数据
    }

    //把图片大小设置与显示窗口一样大
    cv::resize(srcImage, srcImage, Size(480, 480));

    Mat grayImage;
    //转灰度图
    cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);
    //监测人脸数据
    std::vector<Rect> faceRect;
    cascade.detectMultiScale(grayImage, faceRect);//
    if(faceRect.size()>0 && flag >= 0)
    {

        Rect rect = faceRect.at(0);
        //rectangle(srcImage,rect,Scalar(0,0,255));

        //移动人脸框（图片--QLabel）
        ui->headpicLb->move(rect.x, rect.y);

        if(flag > 2)
        {
            //把Mat数据转化为QbyteArray ————》编码成jpg格式发送
            std::vector<uchar> buf;
            cv::imencode(".jpg", srcImage, buf);
            QByteArray byte((const char*)buf.data(), buf.size());
            //准备发送
            quint64 backsize = byte.size();
            QByteArray sendData;
            QDataStream stream(&sendData, QIODevice::WriteOnly);
            stream.setVersion(QDataStream::Qt_5_14);
            stream<<backsize<<byte;
            //发送
            msocket.write(sendData);
            flag = -2;

            faceMat = srcImage(rect);
            //保存
            imwrite("./face.jpg", faceMat);
        }
        flag++;
    }
    if(faceRect.size() == 0)
    {
        //把人脸移到中心位置
        ui->headpicLb->move(100, 60);
        ui->widgetLb->hide();
        flag = 0;
    }

    if(srcImage.data == nullptr) return;
    //把opencv里面的Mat格式数据（BGR）转Qt里面的QImage（RGB）
    cvtColor(srcImage,srcImage, COLOR_BGR2RGB);
    QImage image(srcImage.data, srcImage.cols, srcImage.rows, srcImage.step1(), QImage::Format_RGB888);
    QPixmap mmp = QPixmap::fromImage(image);

    ui->videoLb->setPixmap(mmp);
}

void FaceAttendance::recv_data()
{
    //{employeeID:%1,name:%2,department:软件,time:%3}
    QByteArray array = msocket.readAll();
    qDebug()<<array;
    //Json解析
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(array, &err);
    if(err.error != QJsonParseError::NoError)
    {
        qDebug()<<"json数据错误";
        return;
    }

    QJsonObject obj = doc.object();
    QString employeeID = obj.value("employeeID").toString();
    QString name = obj.value("name").toString();
    QString department = obj.value("department").toString();
    QString timestr = obj.value("time").toString();

    ui->numberEdit->setText(employeeID);
    ui->nameEdit->setText(name);
    ui->departmentEdit->setText(department);
    ui->timeEdit->setText(timestr);

    //通过样式来显示图片
    ui->headLb->setStyleSheet("border-radius:75px; border-image: url(./face.jpg);");
    ui->widgetLb->show();
}


void FaceAttendance::timer_connect()
{
    //连接服务器
    msocket.connectToHost("192.168.127.227",999);
    qDebug()<<"正在连接服务器";
}

void FaceAttendance::stop_connect()
{
    mtimer.stop();
    qDebug()<<"成功连接服务器";
}

void FaceAttendance::start_connect()
{
    mtimer.start(5000);//启动定时器
    qDebug()<<"断开连接";
}
