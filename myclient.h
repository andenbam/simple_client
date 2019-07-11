#ifndef MYCLIENT_H
#define MYCLIENT_H
#pragma once
//test
#include <QAbstractSocket>
#include <QSslSocket>
#include <QWidget>

class QTextEdit;
class QLineEdit;
class QPushButton;
class QVBoxLayout;

class MyClient : public QWidget
{
Q_OBJECT
private:
    QSslSocket* socket;
    QString externalAddress = "";
    QVBoxLayout* mainLayout;
    QTextEdit*   textInfo;
    QLineEdit*   lineInput;
    QLineEdit*   lineHost;
    QLineEdit*   linePort;
    QPushButton* buttonSend;
    QPushButton* buttonConnect;
    QPushButton* buttonDisconnect;

public:
    MyClient();
    void sendToServer(const QString&);
    void show();
private:
    void clearConsole();
    void sslErrorOccured(const QList<QSslError> &errors);
private slots:
    void gotExternalAddress(QString address);
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
