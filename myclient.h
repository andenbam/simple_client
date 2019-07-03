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
    qint16 nextBlockSize;

public:
    MyClient(const QString& strHost, quint16 port, QWidget* pwgt = nullptr);

private slots:
    void slotReadyRead();
    void slotError(QAbstractSocket::SocketError);
    void slotSendToServer();
    void slotConnected();
};

#endif // MYCLIENT_H
