#include "myclient.h"

#include <QLineEdit>
#include <QPushButton>
#include <QTcpSocket>
#include <QTextEdit>

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

    info = new QTextEdit();
    input = new QLineEdit();

    info->setReadOnly(true);

    QPushButton* pushCommand = new QPushButton("&Send");
}
