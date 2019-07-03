#include "myclient.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTcpSocket>
#include <QTextEdit>
#include <QVBoxLayout>

MyClient::MyClient(const QString &host,
                   quint16 port,
                   QWidget *pwgt) : QWidget (pwgt), nextBlockSize(0)
{
    socket = new QTcpSocket(this);
    socket->connectToHost(host, port);
    connect(socket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(socket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this,  SLOT(slotError(QAbstractSocket::SocketError)));

    textInfo = new QTextEdit();
    textInput = new QLineEdit();

    textInfo->setReadOnly(true);

    QPushButton* pushCommand = new QPushButton("&Send");
    connect(pushCommand, SIGNAL(returnPressed()), SLOT(slotSendToServer()));
    connect(textInput, SIGNAL(returnPressed()), this, SLOT(slotSendToServer()));

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(new QLabel("<HI>Client</HI"));
    layout->addWidget(textInfo);
    layout->addWidget(textInput);
    layout->addWidget(pushCommand);
    setLayout(layout);
}

void MyClient::slotReadyRead()
{

}

void MyClient::slotError(QAbstractSocket::SocketError)
{

}

void MyClient::slotSendToServer()
{

}

void MyClient::slotConnected()
{

}
