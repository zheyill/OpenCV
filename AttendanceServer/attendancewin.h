#ifndef ATTENDANCEWIN_H
#define ATTENDANCEWIN_H

#include "qfaceobject.h"

#include <QMainWindow>
#include <QTcpSocket>
#include <QTcpServer>
#include <QSqlTableModel>
#include <QSqlRecord>
QT_BEGIN_NAMESPACE
namespace Ui { class AttendanceWin; }
QT_END_NAMESPACE

class AttendanceWin : public QMainWindow
{
    Q_OBJECT

public:
    AttendanceWin(QWidget *parent = nullptr);
    ~AttendanceWin();
signals:
    void query(cv::Mat& image);
protected slots:
    void accept_client();
    void read_data();
    void recv_faceid(int64_t faceid);
private:
    Ui::AttendanceWin *ui;
    QTcpServer mserver;
    QTcpSocket *msocket;
    quint64 bsize;

    QFaceObject fobj;
    QSqlTableModel model;
};
#endif // ATTENDANCEWIN_H
