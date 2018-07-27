#ifndef TU_H
#define TU_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>
#include <QString>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include <QProcess>

class tu : public QObject
{
    Q_OBJECT
public:
    explicit tu(QObject *parent = 0);
    void writeOpt(QByteArray, QHostAddress, qint16);
    void writeNet(QByteArray, QHostAddress, qint16);
    void writeRS485(QByteArray);
    void writeRS232(QByteArray);
    bool compareQba(QByteArray, QByteArray);


signals:

public slots:
    void readyReadOpt();
    void readyReadNet();
    void readyReadRs485();
    void readyReadRs232();
    void sendMessages();

private:
    QString pingCommand;
    QString masterIP;
    QString resPing;

    QSerialPort* linkRS485, *linkRS232; // последовательные порты

    QHostAddress hostAddr;
    qint16 portNet, portOpt;
    QUdpSocket *socketOpt, *socketNet;  // сокеты

    QByteArray readBuffOpt, readBuffNet, readBuffRs485, readBuffRs232;      // массивы вычитанных данных со всех каналов
    QByteArray writeBuffOpt, writeBuffNet, writeBuffRs485, writeBuffRs232;  // массивы записываемых данных во всех каналы
    QByteArray ethBuffOpt, ethBuffNet, ethBuffRs485, ethBuffRs232;          // ethernet

    QTimer* timerSend;
    int interval;
};

#endif // TU_H
