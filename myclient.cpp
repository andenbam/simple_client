#include "myclient.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTcpSocket>
#include <QTextEdit>
#include <QTime>
#include <QVBoxLayout>

MyClient::MyClient() : QWidget(), nextBlockSize(0)
{

    socket = new QTcpSocket(this);

    textInfo = new QTextEdit();
    textInfo->setReadOnly(true);
    textInput = new QLineEdit();

    sendButton = new QPushButton("&Send");
    connectButton = new QPushButton("&Connect");

    connect(connectButton, &QPushButton::pressed, this, &MyClient::slotSetConnection);
    connect(sendButton, &QPushButton::pressed, this, &MyClient::slotSendToServer);
    connect(socket, &QTcpSocket::connected, this, &MyClient::slotConnected);
    connect(socket, &QTcpSocket::readyRead, this, &MyClient::slotReadyRead);
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this,
                    SLOT(slotError(QAbstractSocket::SocketError)));


    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(connectButton);
    layout->addWidget(textInfo);
    layout->addWidget(textInput);
    layout->addWidget(sendButton);
    setLayout(layout);
}

void MyClient::slotReadyRead()
{
    QDataStream dataStream(socket);
    dataStream.setVersion(QDataStream::Qt_5_9);

    while(true){

        if (!nextBlockSize){

            if (socket->bytesAvailable() < sizeof(nextBlockSize))
                return;

            dataStream >> nextBlockSize;
        }

        if (socket->bytesAvailable() < nextBlockSize)
            return;

        QTime time;
        QString str;
        dataStream >> time >> str;

        textInfo->append(time.toString() + " " + str);
        nextBlockSize = 0;
    }
}

void MyClient::slotError(QAbstractSocket::SocketError err)
{
    QString errorMessage = "Error: " +
            (err == QTcpSocket::HostNotFoundError ?
                 "Host was not found" :
             err == QTcpSocket::RemoteHostClosedError ?
                  "Remote Host is closed" :
             err == QTcpSocket::ConnectionRefusedError ?
                   "Connection was refused" :
             QString(socket->errorString()));

    textInfo->append(errorMessage);
}

void MyClient::slotSendToServer()
{
    QByteArray block;

    QDataStream outputStream(&block, QIODevice::WriteOnly);
    outputStream.setVersion(QDataStream::Qt_5_9);

    outputStream << quint16(0)
                 << QTime::currentTime()
                 << textInput->text();

    outputStream.device()->seek(0);
    outputStream << quint16(
                        block.size() - sizeof(quint16));

    socket->write(block);
    textInput->setText("");
}

void MyClient::slotConnected()
{
    textInfo->append(".Connection established.");
}

void MyClient::slotSetConnection()
{
    socket->connectToHost("localhost",2323);
}
