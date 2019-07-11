#include "myclient.h"
#include "testexternaladdress.h"

#include <QGroupBox>
#include <QHostAddress>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTcpSocket>
#include <QTextEdit>
#include <QTime>
#include <QVBoxLayout>
#include <qnetworkinterface.h>

MyClient::MyClient() : QWidget() {

    textInfo  = new QTextEdit();
    lineInput = new QLineEdit();
    lineHost  = new QLineEdit();
    linePort  = new QLineEdit();
    buttonSend       = new QPushButton("&Send");
    buttonConnect    = new QPushButton("&Connect");
    buttonDisconnect = new QPushButton("&Disconnect");

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

    socket = new QSslSocket(this);
    socket->addCaCertificates("cert.pem");
}

void MyClient::sendToServer(const QString& message) {

    socket -> write(message.toUtf8());
}

void MyClient::show() {

    connect(new TestExternalAddress(), &TestExternalAddress::gotAddress,
                                 this, &MyClient::gotExternalAddress);

    textInfo  -> setReadOnly(true);
    lineHost  -> setPlaceholderText("#serverHost");
    lineHost  -> setText("localhost");
    linePort  -> setPlaceholderText("#Port");
    linePort  -> setText("5005");
    lineInput -> setPlaceholderText("#message to server");

    lineInput        -> setDisabled(true);
    buttonSend       -> setDisabled(true);
    buttonDisconnect -> setDisabled(true);

    mainLayout          = new QVBoxLayout();
    QHBoxLayout* hPanel = new QHBoxLayout();
    QHBoxLayout* lPanel = new QHBoxLayout();

    QGroupBox* connectBox = new QGroupBox("Connection to Server");

    hPanel -> addWidget(lineHost);
    hPanel -> addWidget(linePort);
    hPanel -> addWidget(buttonConnect);
    hPanel -> addWidget(buttonDisconnect);
    lPanel -> addWidget(lineInput);
    lPanel -> addWidget(buttonSend);

    QGroupBox* clientBox = new QGroupBox("Simple Client Messenger");
    QVBoxLayout* vbox = new QVBoxLayout();

    vbox->addWidget(textInfo);
    vbox->addLayout(lPanel);
    clientBox->setLayout(vbox);
    connectBox->setLayout(hPanel);

    mainLayout -> addWidget(connectBox);
    mainLayout -> addWidget(clientBox);

    setLayout(mainLayout);

    QWidget::show();
}

void MyClient::clearConsole() {

    textInfo -> clear();

    foreach (const QNetworkInterface &netInterface, QNetworkInterface::allInterfaces()) {
        QNetworkInterface::InterfaceFlags flags = netInterface.flags();
        if( bool((flags & QNetworkInterface::IsRunning)) && !bool(flags & QNetworkInterface::IsLoopBack)){
            foreach (const QNetworkAddressEntry &address, netInterface.addressEntries()) {
                if(address.ip().protocol() == QAbstractSocket::IPv4Protocol)
                    textInfo -> append(QString("Local address: ").append(address.ip().toString()));
            }
        }
    }

    textInfo -> append(QString("External address: ").append(externalAddress));
}

void MyClient::gotExternalAddress(QString address)
{
    externalAddress = address;
}

void MyClient::slotReadyRead() {

    textInfo -> append(QString("#").append(QString::fromUtf8(socket->readAll())));
}

void MyClient::slotError(QAbstractSocket::SocketError err) {

    buttonConnect -> setDisabled(false);

    QString errorMessage = "Error: " +
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

//    if (socket->waitForEncrypted(1000)){


//    }
//    else {
//        socket->disconnectFromHost();
//        textInfo -> append("Error encryption");
//    }
    lineInput -> setText("");
}

void MyClient::slotConnected() {

    buttonConnect    -> setDisabled(true);
    lineInput        -> setDisabled(false);
    buttonSend       -> setDisabled(false);
    buttonDisconnect -> setDisabled(false);

    //socket->startClientEncryption();
    textInfo -> append("connection established");
}

void MyClient::slotDisconnected() {

    textInfo -> append("[you've been disconnected from server]");
}

void MyClient::slotSetConnection(){

    lineHost      -> setDisabled(true);
    linePort      -> setDisabled(true);
    buttonConnect -> setDisabled(true);

    clearConsole();

    textInfo -> append(QString("Connecting to ")
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

    socket->connectToHostEncrypted(lineHost->text(), quint16(linePort->text().toInt()));
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
