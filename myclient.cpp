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

    socket            =           new         QTcpSocket(         this   );
    textInfo                    =         new        QTextEdit(                );
    textInfo          ->             setReadOnly                (true         );
    lineInput = new QLineEdit();
    lineHost = new QLineEdit();
    linePort = new QLineEdit();

    lineHost->setPlaceholderText("host name");
    linePort->setPlaceholderText("Port");
    lineHost->setText("localhost");

    sendButton = new QPushButton("&Send");
    connectButton = new QPushButton("&Connect");

    connect(lineHost, &QLineEdit::textChanged,
                this, &MyClient::slotConnectionFieldsListener);

    connect(linePort, &QLineEdit::textChanged,
                this, &MyClient::slotConnectionFieldsListener);

    connect(connectButton, &QPushButton::pressed,
                     this, &MyClient::slotSetConnection);

    connect(sendButton, &QPushButton::pressed,
                  this, &MyClient::slotSendToServer);

    connect(socket, &QTcpSocket::connected,
              this, &MyClient::slotConnected);
    connect(socket, &QTcpSocket::readyRead,
              this, &MyClient::slotReadyRead);

    //НЕ СПРАШИВАЙ почему, я просто решил пользоваться исключительно новым синтаксисом

    connect(socket, static_cast<void (QTcpSocket::*)
               (QAbstractSocket::SocketError)>(&QAbstractSocket::error), this,
                    &MyClient::slotError);

    // вот тут есть ответ, почему так -
    // https://stackoverflow.com/questions/35655512/compile-error-when-connecting-qtcpsocketerror-using-the-new-qt5-signal-slot

    QVBoxLayout* layout = new QVBoxLayout();
    QHBoxLayout* horizontalLayout = new QHBoxLayout();
    horizontalLayout->addWidget(lineHost);
    horizontalLayout->addWidget(linePort);
    horizontalLayout->addWidget(connectButton);
    layout->addLayout(horizontalLayout);
    layout->addWidget(textInfo);
    layout->addWidget(lineInput);
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

    socket->write(lineInput->text().toStdString().c_str());
    lineInput->setText("");

}

void MyClient::slotConnected()
{
    textInfo->append(".Connection established.");
}

void MyClient::slotSetConnection()
{
//    socket->connectToHost("46.0.199.93", 5000);
    textInfo->append("Connecting to ");
    textInfo->append(lineHost->text().append(":").append(linePort->text()));
    socket->connectToHost(lineHost->text(), quint16(linePort->text().toInt()));
}

void MyClient::slotConnectionFieldsListener()
{
    if (lineHost->text().isNull() || linePort->text().isNull()){

        connectButton->setDisabled(true);

    } else {

        connectButton->setDisabled(false);
    };
}
