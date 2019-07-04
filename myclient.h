#ifndef MYCLIENT_H
#define MYCLIENT_H
#pragma once

#include <QTcpSocket>
#include <QWidget>

class QTextEdit;
class QLineEdit;
class QPushButton;

class MyClient : public QWidget
{
Q_OBJECT
private:
    QTcpSocket* socket;
    QTextEdit* textInfo;
    QLineEdit* lineInput;
    QLineEdit* lineHost;
    QLineEdit* linePort;
    QPushButton* sendButton;
    QPushButton* connectButton;
    QPushButton* disconnectButton;
    unsigned short nextBlockSize;

public:
    MyClient();

private slots:
    void slotReadyRead();
    void slotError(QAbstractSocket::SocketError);
    void slotSendToServer();
    void slotConnected();
    void slotSetConnection();
    void slotDropConnection();
    void slotConnectionFieldsListener();
};

#endif // MYCLIENT_H
