#include "tu.h"

tu::tu(QObject *parent) : QObject(parent)
{
    socketNet = new QUdpSocket(this);
    socketOpt = new QUdpSocket(this);

    portNet = 8050;
    portOpt = 8051;

    hostAddr = QHostAddress::LocalHost;

    socketNet->bind(hostAddr, portNet);
    socketOpt->bind(hostAddr, portOpt);

    timerSend = new QTimer(this);
    interval = 500;

    linkRS485 = new QSerialPort("COM1");
    linkRS232 = new QSerialPort("COM2");

    if(linkRS485->open(QSerialPort::ReadWrite)) {
        linkRS485->setBaudRate(QSerialPort::Baud4800);
        linkRS485->setDataBits(QSerialPort::Data8);
        linkRS485->setParity(QSerialPort::NoParity);
        linkRS485->setStopBits(QSerialPort::OneStop);
        linkRS485->setFlowControl(QSerialPort::NoFlowControl);

        linkRS485->flush();
        linkRS485->clear();
    }

    if (linkRS232->open(QSerialPort::ReadWrite)) {
        linkRS232->setBaudRate(QSerialPort::Baud19200);
        linkRS232->setDataBits(QSerialPort::Data8);
        linkRS232->setParity(QSerialPort::NoParity);
        linkRS232->setStopBits(QSerialPort::OneStop);
        linkRS232->setFlowControl(QSerialPort::NoFlowControl);

        linkRS232->flush();
        linkRS232->clear();
    }

    ethBuffNet.append(1).append(1).append(1).append(0.).append(0.);
    ethBuffOpt.append(1).append(1).append(0.).append(1).append(1);
    ethBuffRs485.append(1).append(1).append(1).append(1).append(1).append(1);

    short* frameAPD = new short [9];
    for (int i = 0; i < 9; i++)
        frameAPD[i] = 1;
    for (int i = 0; i < 9; i++)
        ethBuffRs232.append(frameAPD[i]);

    connect(socketNet, SIGNAL(readyRead()), this, SLOT(readyReadNet()));
    connect(socketOpt, SIGNAL(readyRead()), this, SLOT(readyReadOpt()));
    connect(linkRS485, SIGNAL(readyRead()), SLOT(readyReadRs485()));
    connect(linkRS232, SIGNAL(readyRead()), SLOT(readyReadRs232()));
    connect(timerSend, SIGNAL(timeout()), SLOT(sendMessages()));

    timerSend->start(interval);

}

void tu::writeNet(QByteArray buff, QHostAddress addr, qint16 port)
{
    socketNet->writeDatagram(buff, addr, port);
}

void tu::writeOpt(QByteArray buff, QHostAddress addr, qint16 port)
{
    socketOpt->writeDatagram(buff, addr, port);
}

void tu::writeRS485(QByteArray buff)
{
    linkRS485->write(buff, buff.size());
}

void tu::writeRS232(QByteArray buff)
{
    linkRS232->write(buff, buff.size());
}

void tu::readyReadNet()
{
    readBuffNet.resize(socketNet->pendingDatagramSize());
    QHostAddress sender;
    quint16 senderPort;
    socketNet->readDatagram(readBuffNet.data(), readBuffNet.size(), &sender, &senderPort);

    if (compareQba(readBuffNet, ethBuffNet))
        writeBuffNet.append(5);
    else
        writeBuffNet.append(4);
}

void tu::readyReadOpt()
{
    readBuffOpt.resize(socketOpt->pendingDatagramSize());
    QHostAddress sender;
    quint16 senderPort;
    socketOpt->readDatagram(readBuffOpt.data(), readBuffOpt.size(), &sender, &senderPort);

    if (compareQba(readBuffOpt, ethBuffOpt))
        writeBuffOpt.append(5);
    else
        writeBuffOpt.append(4);
}

void tu::readyReadRs485()
{
    readBuffRs485 = linkRS485->readAll();
    if (compareQba(readBuffRs485, ethBuffRs485))
        writeBuffRs485.append(5);
    else
        writeBuffRs485.append(4);
}

void tu::readyReadRs232()
{
    readBuffRs232 = linkRS232->readAll();
    if (compareQba(readBuffRs232, ethBuffRs232))
         writeBuffRs232.append(5);
    else
        writeBuffRs232.append(4);
}

void tu::sendMessages()
{
    writeNet(writeBuffNet, hostAddr, portNet);
    writeOpt(writeBuffOpt, hostAddr, portOpt);
    writeRS485(writeBuffRs485);
}

bool tu::compareQba(QByteArray q1, QByteArray q2)
{
    bool res;
    if (q1 == q2)
        res = true;
    else
        res = false;
    return (res);
}

