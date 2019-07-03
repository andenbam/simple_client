#ifndef MYCLIENT_H
#define MYCLIENT_H
#pragma once

#include <QTcpSocket>
#include <QWidget>

class QTextEdit;
class QLineEdit;

class MyClient : public QWidget
{
Q_OBJECT
private:
    QTcpSocket* socket;
    QTextEdit* textInfo;
    QLineEdit* textInput;
    unsigned short nextBlockSize;

public:
    MyClient();

private slots:
    void slotReadyRead();
    void slotError(QAbstractSocket::SocketError);
    void slotSendToServer();
    void slotConnected();
    void slotSetConnection();
};

#endif // MYCLIENT_H
