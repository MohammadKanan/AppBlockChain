#include "NetClient.h"
#include "src/Network/dnsLockUp.h"

NetClient::NetClient(QObject *parent)
    : QObject{parent}
{
    dnsLockUp* dns = new dnsLockUp;
    this->initiateoutSocket();
}

void NetClient::sendMessage(QByteArray data)
{
    txSocket->write(data);
}

void NetClient::startHandShake()
{
    QByteArray data;
    data.append("F9BEB4D9"); // magic word
    data+= "76657273696F6E0000000000"; // "version"
    data += "55000000"; //Size
    data += "F7639C610"; // checksum
    data = "F9BEB4D976657273696F6E0000000000550000002C2F86F37E1101000000000000000000C515CF6100000000000000000000000000000000000000000000FFFF2E13894A208D000000000000000000000000000000000000FFFF7F000001208D00000000000000000000000000";
    //QDataStream out(&data, QIODevice::WriteOnly);
    const auto data2 = QByteArray::fromHex(data);
    sendMessage(data2);
}

void NetClient::initiateoutSocket()
{
    txSocket = QSharedPointer<QTcpSocket>(new QTcpSocket , &QObject::deleteLater);
    QObject::connect(txSocket.get() , &QTcpSocket::connected , this , [this](){
        qDebug() << "Conn established!";
        startHandShake();
    });
    QObject::connect(txSocket.get() , &QTcpSocket::disconnected , this, [](){
        qDebug() << "Conn disconnect!";
    });
    QObject::connect(txSocket.get(), &QTcpSocket::readyRead , this, [&](){
        // Read data from the socket
        QByteArray data = txSocket->readAll();
        QByteArray hexAsciiData = data.toHex();
        qDebug() << "Data received:" << hexAsciiData;
        if(!verackSent){
            data = "F9BEB4D976657261636B000000000000000000005DF6E0E2";
            const auto data2 = QByteArray::fromHex(data);
            sendMessage(data2);
            verackSent = !verackSent;
        }
    });
    QObject::connect(txSocket.get(), &QTcpSocket::errorOccurred , this , [&](){
        qDebug() << "Socket error:" << txSocket->errorString();
    });
    //txSocket->connectToHost("172.5.222.129",8333); // 69.250.215.150 , 89.125.48.42 , 86.201.225.172
    txSocket->connectToHost("86.201.225.172",8333); // 69.250.215.150 , 89.125.48.42 , 86.201.225.172

}

