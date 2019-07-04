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
    textInfo =  new QTextEdit();
    lineInput = new QLineEdit();
    lineHost =  new QLineEdit();
    linePort =  new QLineEdit();
    sendButton =       new QPushButton("&Send");
    connectButton =    new QPushButton("&Connect");
    disconnectButton = new QPushButton("&Disconnect");

    textInfo  -> setReadOnly(true);
    lineHost  -> setPlaceholderText("host name");
    lineHost  -> setText("localhost");
    linePort  -> setPlaceholderText("#Port");
    lineInput -> setPlaceholderText("$ message to server");

    lineInput        -> setDisabled(true);
    sendButton       -> setDisabled(true);
    connectButton    -> setDisabled(true);
    disconnectButton -> setDisabled(true);

    connect(lineHost, &QLineEdit::textChanged,
                this, &MyClient::slotConnectionFieldsListener);
    connect(linePort, &QLineEdit::textChanged,
                this, &MyClient::slotConnectionFieldsListener);

    connect(connectButton, &QPushButton::pressed,
                     this, &MyClient::slotSetConnection);
    connect(disconnectButton, &QPushButton::pressed,
                        this, &MyClient::slotDropConnection);
    connect(sendButton, &QPushButton::pressed,
                  this, &MyClient::slotSendToServer);

    QVBoxLayout* layout = new QVBoxLayout();
    QHBoxLayout* hPanel = new QHBoxLayout();
    QHBoxLayout* lPanel = new QHBoxLayout();

    hPanel -> addWidget(lineHost);
    hPanel -> addWidget(linePort);
    hPanel -> addWidget(connectButton);
    hPanel -> addWidget(disconnectButton);

    lPanel -> addWidget(lineInput);
    lPanel -> addWidget(sendButton);

    layout -> addLayout(hPanel);
    layout -> addWidget(textInfo);
    layout -> addLayout(lPanel);

    setLayout(layout);
}

void MyClient::slotReadyRead() {

    textInfo -> append(QString::fromUtf8(socket->readAll()));
}

void MyClient::slotError(QAbstractSocket::SocketError err) {

    connectButton -> setDisabled(false);

    QString errorMessage = "\nError: " +
            (err == QTcpSocket::HostNotFoundError ?
                 "Host was not found" :
             err == QTcpSocket::RemoteHostClosedError ?
                  "Remote Host is closed" :
             err == QTcpSocket::ConnectionRefusedError ?
                   "Connection was refused" :
             QString(socket->errorString()));

    textInfo->append(errorMessage);

    slotDropConnection();
}

void MyClient::slotSendToServer() {

    socket -> write(lineInput->text().append("\0").toUtf8());
    lineInput -> setText("");
}

void MyClient::slotConnected() {

    lineInput        -> setDisabled(false);
    sendButton       -> setDisabled(false);
    disconnectButton -> setDisabled(false);

    textInfo -> append("\nconnection established\n");
}

void MyClient::slotSetConnection()
{
//  socket->connectToHost("46.0.199.93", 5000);
    lineHost      -> setDisabled(true);
    linePort      -> setDisabled(true);
    connectButton -> setDisabled(true);

    textInfo -> setText(QString("Connecting to ")
                     .append(lineHost->text().append(":")
                     .append(linePort->text())));

    socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::connected,
              this, &MyClient::slotConnected);
    connect(socket, &QTcpSocket::disconnected,
              this, &MyClient::slotDropConnection);
    connect(socket, &QTcpSocket::readyRead,
              this, &MyClient::slotReadyRead);
    connect(socket, static_cast<void (QTcpSocket::*)
               (QAbstractSocket::SocketError)>(&QAbstractSocket::error), this,
                    &MyClient::slotError);

    socket->connectToHost(lineHost->text(), quint16(linePort->text().toInt()));
}

void MyClient::slotDropConnection() {

    lineInput        -> setDisabled(true);
    sendButton       -> setDisabled(true);
    disconnectButton -> setDisabled(true);

    if (socket){

        disconnect(socket, &QTcpSocket::connected,
                     this, &MyClient::slotConnected);
        disconnect(socket, &QTcpSocket::disconnected,
                     this, &MyClient::slotDropConnection);
        disconnect(socket, &QTcpSocket::readyRead,
                     this, &MyClient::slotReadyRead);
        disconnect(socket, static_cast<void (QTcpSocket::*)
               (QAbstractSocket::SocketError)>(&QAbstractSocket::error),
                     this, &MyClient::slotError);

        socket -> close();
        socket = nullptr;
    }

    textInfo->append("\nConnection dropped");

    lineHost         -> setDisabled(false);
    linePort         -> setDisabled(false);
    connectButton    -> setDisabled(false);
}

void MyClient::slotConnectionFieldsListener() {

    connectButton->setDisabled(lineHost->text() == "" ||
                               linePort->text() == "");
}
