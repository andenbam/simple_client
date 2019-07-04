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
    textHost = new QLineEdit();
    textPort = new QLineEdit();

    textHost->setPlaceholderText("host name");
    textPort->setPlaceholderText("Port");
    textHost->setText("localhost");

    sendButton = new QPushButton("&Send");
    connectButton = new QPushButton("&Connect");

    connect(connectButton, &QPushButton::pressed, this, &MyClient::slotSetConnection);
    connect(sendButton, &QPushButton::pressed, this, &MyClient::slotSendToServer);
    connect(socket, &QTcpSocket::connected, this, &MyClient::slotConnected);
    connect(socket, &QTcpSocket::readyRead, this, &MyClient::slotReadyRead);
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this,
                    SLOT(slotError(QAbstractSocket::SocketError)));


    QVBoxLayout* layout = new QVBoxLayout();
    QHBoxLayout* horizontalLayout = new QHBoxLayout();
    horizontalLayout->addWidget(textHost);
    horizontalLayout->addWidget(textPort);
    horizontalLayout->addWidget(connectButton);
    layout->addLayout(horizontalLayout);
    layout->addWidget(textInfo);
    layout->addWidget(textInput);
    layout->addWidget(sendButton);
    setLayout(layout);
}

void MyClient::slotReadyRead()
{
    QDataStream dataStream(socket);
    dataStream.setVersion(QDataStream::Qt_5_9);
    char* str = new char[256];
    socket->read(str, 256);
    textInfo->append(str);
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

    socket->write(textInput->text().toStdString().c_str());
    textInput->setText("");

}

void MyClient::slotConnected()
{
    textInfo->append(".Connection established.");
}

void MyClient::slotSetConnection()
{
//    socket->connectToHost("46.0.199.93", 5000);
    socket->connectToHost(textHost->text(), textPort->text().toInt());
}
