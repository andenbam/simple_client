#include "myclient.h"

#include <QHostAddress>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTcpSocket>
#include <QTextEdit>
#include <QTime>
#include <QVBoxLayout>

MyClient::MyClient() : QWidget() {

    textInfo  = new QTextEdit();
    lineInput = new QLineEdit();
    lineHost  = new QLineEdit();
    linePort  = new QLineEdit();
    buttonSend       = new QPushButton("&Send");
    buttonConnect    = new QPushButton("&Connect");
    buttonDisconnect = new QPushButton("&Disconnect");

    textInfo  -> setReadOnly(true);
    lineHost  -> setPlaceholderText("host name");
    lineHost  -> setText("localhost");
    linePort  -> setPlaceholderText("#Port");
    linePort  -> setText("5005");
    lineInput -> setPlaceholderText("$ message to server");

    lineInput        -> setDisabled(true);
    buttonSend       -> setDisabled(true);
    buttonDisconnect -> setDisabled(true);

    connect(lineHost, &QLineEdit::textChanged,
                this, &MyClient::slotConnectionFieldsListener);
    connect(linePort, &QLineEdit::textChanged,
                this, &MyClient::slotConnectionFieldsListener);

    connect(buttonConnect, &QPushButton::pressed,
                     this, &MyClient::slotSetConnection);
    connect(buttonDisconnect, &QPushButton::pressed,
                        this, &MyClient::slotDropConnection);
    connect(buttonSend, &QPushButton::pressed,
                  this, &MyClient::slotSendToServer);

    connect(lineInput, &QLineEdit::returnPressed,
                 this, &MyClient::slotSendToServer);

    QVBoxLayout* layout = new QVBoxLayout();
    QHBoxLayout* hPanel = new QHBoxLayout();
    QHBoxLayout* lPanel = new QHBoxLayout();

    hPanel -> addWidget(lineHost);
    hPanel -> addWidget(linePort);
    hPanel -> addWidget(buttonConnect);
    hPanel -> addWidget(buttonDisconnect);

    lPanel -> addWidget(lineInput);
    lPanel -> addWidget(buttonSend);

    layout -> addLayout(hPanel);
    layout -> addWidget(textInfo);
    layout -> addLayout(lPanel);

    setLayout(layout);

    //Теперь сокет создаётся раз за запуск программы, но это не избавляет от проблемы обновления :(
    socket = new QTcpSocket(this);
}

void MyClient::sendToServer(const QString& message) {

    socket -> write(message.toUtf8());
}

void MyClient::slotReadyRead() {

    textInfo -> append(QString("[SRV]:").append(QString::fromUtf8(socket->readAll())));
}

void MyClient::slotError(QAbstractSocket::SocketError err) {

    buttonConnect -> setDisabled(false);

    QString errorMessage = "\nError: " +
            (err == QAbstractSocket::HostNotFoundError ?
                 "Host was not found" :
             err == QAbstractSocket::RemoteHostClosedError ?
                  "Remote Host is closed" :
             err == QAbstractSocket::ConnectionRefusedError ?
                   "Connection was refused" :
             QString(socket->errorString()));

    textInfo->append(errorMessage);

    slotDropConnection();
}

void MyClient::slotSendToServer() {

    sendToServer(lineInput->text());
    lineInput -> setText("");
}

void MyClient::slotConnected() {

    buttonConnect    -> setDisabled(true);
    lineInput        -> setDisabled(false);
    buttonSend       -> setDisabled(false);
    buttonDisconnect -> setDisabled(false);

    textInfo -> append("connection established");
}

void MyClient::slotDisconnected() {

    textInfo -> append("[you've been disconnected from server]");
}

void MyClient::slotSetConnection(){

    //socket->connectToHost("46.0.199.93", 5000);
    lineHost      -> setDisabled(true);
    linePort      -> setDisabled(true);
    buttonConnect -> setDisabled(true);

    textInfo -> setText(QString("Connecting to ")
                     .append(lineHost->text().append(":")
                     .append(linePort->text())));

    connect(socket, &QAbstractSocket::connected,
              this, &MyClient::slotConnected);
    connect(socket, &QAbstractSocket::disconnected,
              this, &MyClient::slotDisconnected);
    connect(socket, &QAbstractSocket::readyRead,
              this, &MyClient::slotReadyRead);
    connect(socket, static_cast<void (QAbstractSocket::*)(QAbstractSocket::SocketError)>
            (&QAbstractSocket::error),
            this, &MyClient::slotError);

    socket->connectToHost(lineHost->text(), quint16(linePort->text().toInt()));
}

void MyClient::slotDropConnection() {

    lineInput        -> setDisabled(true);
    buttonSend       -> setDisabled(true);
    buttonDisconnect -> setDisabled(true);

    if (socket){

        socket -> disconnectFromHost();

        disconnect(socket, &QAbstractSocket::connected,
                     this, &MyClient::slotConnected);
        disconnect(socket, &QAbstractSocket::disconnected,
                     this, &MyClient::slotDisconnected);
        disconnect(socket, &QAbstractSocket::readyRead,
                     this, &MyClient::slotReadyRead);
        disconnect(socket, static_cast<void (QAbstractSocket::*)(QAbstractSocket::SocketError)>
                   (&QAbstractSocket::error),
                   this, &MyClient::slotError);
    }

    textInfo->append("Connection dropped");

    lineHost         -> setDisabled(false);
    linePort         -> setDisabled(false);
    buttonConnect    -> setDisabled(false);
}

void MyClient::slotConnectionFieldsListener() {

    buttonConnect->setDisabled(lineHost->text() == "" ||
                               linePort->text() == "");
}
