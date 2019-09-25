#ifndef MYCLIENT_H
#define MYCLIENT_H
#pragma once

#include <QAbstractSocket>
#include <QSslSocket>
#include <QWidget>

class QTextEdit;
class QLineEdit;
class QCheckBox;
class QPushButton;
class QVBoxLayout;

class MyClient : public QWidget
{
Q_OBJECT
private:
    QSslSocket* socket;
    QString externalAddress = "";

    QVBoxLayout* mainLayout;
    QCheckBox*   checkErrors;
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
private slots:
    void gotExternalAddress(QString address);
    void slotReadyRead();
    void acceptSslErrors(const QList<QSslError> &errors);
    void ignoreSslErrors(const QList<QSslError> &errors);
    void slotError(QAbstractSocket::SocketError);
    void slotSendToServer();
    void slotConnected();
    void slotDisconnected();
    void slotSetConnection();
    void slotDropConnection();
    void listenerConnectionFields();
    void listenerCheckBox();
};

#endif // MYCLIENT_H
