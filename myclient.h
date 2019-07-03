#ifndef MYCLIENT_H
#define MYCLIENT_H
#pragma once

#include <QWidget>

class QTextEdit;
class QLineEdit;
class QTcpSocket;

class MyClient : public QWidget
{
Q_OBJECT
private:
    QTcpSocket* socket;
    QTextEdit* info;
    QLineEdit* input;
    qint16 nextBlockSize;

public:
    MyClient(const QString& strHost, quint16 port, QWidget* pwgt = nullptr);

private slots:
    void slotReadyRead();
    void slotError();
    void slotSendToServer();
    void slotConnected();
};

#endif // MYCLIENT_H
