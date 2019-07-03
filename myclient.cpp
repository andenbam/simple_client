#include "myclient.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTcpSocket>
#include <QTextEdit>
#include <QTime>
#include <QVBoxLayout>

MyClient::MyClient(const QString &host,
                   quint16 port,
                   QWidget *pwgt) : QWidget (pwgt), nextBlockSize(0)
{
    socket = new QTcpSocket(this);
    socket->connectToHost(host, port);

    textInfo = new QTextEdit();
    textInfo->setReadOnly(true);
    textInput = new QLineEdit();

    QPushButton* pushButton = new QPushButton("&Send");

    connect(pushButton, &QPushButton::pressed, this, &MyClient::slotSendToServer);
    connect(socket, &QTcpSocket::connected, this, &MyClient::slotConnected);
    connect(socket, &QTcpSocket::readyRead, this, &MyClient::slotReadyRead);

    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this,
                    SLOT(slotError(QAbstractSocket::SocketError)));


    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(textInfo);
    layout->addWidget(textInput);
    layout->addWidget(pushButton);
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

}

void MyClient::slotConnected()
{

}
