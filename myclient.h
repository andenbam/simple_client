#ifndef MYCLIENT_H
#define MYCLIENT_H
#pragma once

#include <QAbstractSocket>
#include <QWidget>

class QTextEdit;
class QLineEdit;
class QPushButton;

class MyClient : public QWidget
{
Q_OBJECT
private:
    QAbstractSocket* socket;
    QTextEdit* textInfo;
    QLineEdit* lineInput;
    QLineEdit* lineHost;
    QLineEdit* linePort;
    QPushButton* sendButton;
    QPushButton* connectButton;
    QPushButton* disconnectButton;

public:
    MyClient();
    void sendToServer(const QString&);
private slots:
    void slotReadyRead();
    void slotError(QAbstractSocket::SocketError);
    void slotSendToServer();
    void slotConnected();
    void slotDisconnected();
    void slotSetConnection();
    void slotDropConnection();
    void slotConnectionFieldsListener();
};

#endif // MYCLIENT_H
