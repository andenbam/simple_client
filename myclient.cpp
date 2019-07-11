#include "myclient.h"
#include "testexternaladdress.h"
#include <QCheckBox>
#include <QGroupBox>
#include <QHostAddress>
#include <QLabel>
#include <QLineEdit>
#include <QNetworkConfigurationManager>
#include <QPushButton>
#include <QTcpSocket>
#include <QTextEdit>
#include <QThread>
#include <QTime>
#include <QVBoxLayout>
#include <qnetworkinterface.h>

MyClient::MyClient() : QWidget() {

    checkErrors      = new QCheckBox();
    textInfo         = new QTextEdit();
    lineInput        = new QLineEdit();
    lineHost         = new QLineEdit();
    linePort         = new QLineEdit();
    buttonSend       = new QPushButton("&Send");
    buttonConnect    = new QPushButton("&Connect");
    buttonDisconnect = new QPushButton("&Disconnect");

    connect(checkErrors, &QCheckBox::clicked,
                   this, &MyClient::listenerCheckBox);

    connect(lineHost, &QLineEdit::textChanged,
                this, &MyClient::listenerConnectionFields);
    connect(linePort, &QLineEdit::textChanged,
                this, &MyClient::listenerConnectionFields);

    connect(buttonConnect, &QPushButton::pressed,
                     this, &MyClient::slotSetConnection);
    connect(buttonDisconnect, &QPushButton::pressed,
                        this, &MyClient::slotDropConnection);
    connect(buttonSend, &QPushButton::pressed,
                  this, &MyClient::slotSendToServer);

    connect(lineInput, &QLineEdit::returnPressed,
                 this, &MyClient::slotSendToServer);

    QNetworkConfigurationManager confManag;
    confManag.defaultConfiguration().setConnectTimeout(1000);

}

void MyClient::sendToServer(const QString& message) {

    socket -> write(message.toUtf8());
}

void MyClient::show() {

    socket = new QSslSocket(this);

    connect(new TestExternalAddress(), &TestExternalAddress::gotAddress,
                                 this, &MyClient::gotExternalAddress);

    checkErrors->setText("ignore SSL errors");
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
    hPanel -> addWidget(checkErrors);
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


void MyClient::acceptSslErrors(const QList<QSslError> &errors)
{
    for (int i = 1; i < errors.size(); i++){
        textInfo->append("Error: " +  errors.at(i).errorString());
    }
}

void MyClient::ignoreSslErrors(const QList<QSslError> & error)
{
    socket -> ignoreSslErrors(error);
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
    lineInput -> setText("");
}


void MyClient::slotSetConnection(){

    socket -> open(QIODevice::OpenModeFlag::ReadWrite);

    lineHost      -> setDisabled(true);
    linePort      -> setDisabled(true);
    buttonConnect -> setDisabled(true);
    checkErrors   -> setDisabled(true);

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

    buttonDisconnect -> setDisabled(false);
}

void MyClient::slotDropConnection() {

    lineInput        -> setDisabled(true);
    buttonSend       -> setDisabled(true);
    buttonDisconnect -> setDisabled(true);

    if (socket){

        socket -> abort();
        socket -> disconnectFromHost();
        socket -> close();

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
    checkErrors      -> setDisabled(false);
}

void MyClient::slotConnected() {

    buttonConnect    -> setDisabled(true);
    checkErrors      -> setDisabled(true);
    lineInput        -> setDisabled(false);
    buttonSend       -> setDisabled(false);
    buttonDisconnect -> setDisabled(false);

    textInfo -> append("Connection established");
}

void MyClient::slotDisconnected() {

    textInfo -> append("[you've been disconnected from server]");
}

void MyClient::listenerConnectionFields() {

    buttonConnect->setDisabled(lineHost->text() == "" ||
                               linePort->text() == "");
}

void MyClient::listenerCheckBox() {

    if (checkErrors->isChecked()){
        disconnect(socket, static_cast<void (QSslSocket::*)(const QList<QSslError> &)>(&QSslSocket::sslErrors),
                   this, &MyClient::acceptSslErrors);
        connect(socket, static_cast<void (QSslSocket::*)(const QList<QSslError> &)>(&QSslSocket::sslErrors),
                  this, &MyClient::ignoreSslErrors);

    } else {
        disconnect(socket, static_cast<void (QSslSocket::*)(const QList<QSslError> &)>(&QSslSocket::sslErrors),
                  this, &MyClient::ignoreSslErrors);
        connect(socket, static_cast<void (QSslSocket::*)(const QList<QSslError> &)>(&QSslSocket::sslErrors),
                   this, &MyClient::acceptSslErrors);
        QList<QSslError> list;
        socket->ignoreSslErrors(list);
    }
}
