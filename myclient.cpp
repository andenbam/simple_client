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

    textInfo-> setReadOnly(true);

    lineHost  -> setPlaceholderText("host name");
    linePort  -> setPlaceholderText("#Port");
    lineInput -> setPlaceholderText("$ message to server");
    lineHost  -> setText("localhost");

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

    hPanel -> addWidget(lineHost);
    hPanel -> addWidget(linePort);
    hPanel -> addWidget(connectButton);
    hPanel -> addWidget(disconnectButton);

    layout -> addLayout(hPanel);
    layout -> addWidget(textInfo);
    layout -> addWidget(lineInput);
    layout -> addWidget(sendButton);

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
    connectButton -> setDisabled(false);
    slotDropConnection();
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

void MyClient::slotSendToServer() {

    socket -> write(lineInput->text().toStdString().c_str());
    lineInput -> setText("");
}

void MyClient::slotConnected() {

    lineInput        -> setDisabled(false);
    sendButton       -> setDisabled(false);
    disconnectButton -> setDisabled(false);
    textInfo -> append(".Connection established.");
}

void MyClient::slotSetConnection()
{
//  socket->connectToHost("46.0.199.93", 5000);

    textInfo->append(QString("Connecting to \"")
                     .append(lineHost->text().append(":").append(linePort->text())
                     .append("\"")));

    socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::connected,
              this, &MyClient::slotConnected);
    connect(socket, &QTcpSocket::readyRead,
              this, &MyClient::slotReadyRead);
    connect(socket, static_cast<void (QTcpSocket::*)
               (QAbstractSocket::SocketError)>(&QAbstractSocket::error), this,
                    &MyClient::slotError);

    socket->connectToHost(lineHost->text(), quint16(linePort->text().toInt()));

    connectButton->setDisabled(true);
}

void MyClient::slotDropConnection() {

    disconnect(socket, &QTcpSocket::connected,
              this, &MyClient::slotConnected);
    disconnect(socket, &QTcpSocket::readyRead,
              this, &MyClient::slotReadyRead);
    disconnect(socket, static_cast<void (QTcpSocket::*)
               (QAbstractSocket::SocketError)>(&QAbstractSocket::error), this,
                    &MyClient::slotError);

    if (socket->isOpen())
        socket->close();

    delete socket;
    socket = nullptr;
    textInfo->append("\nConnection dropped");
    lineInput        -> setDisabled(true);
    sendButton       -> setDisabled(true);
    disconnectButton -> setDisabled(true);
    connectButton    -> setDisabled(false);

}

void MyClient::slotConnectionFieldsListener() {

    connectButton->setDisabled(lineHost->text() == "" ||
                               linePort->text() == "");
}
