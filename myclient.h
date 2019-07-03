#ifndef MYCLIENT_H
#define MYCLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QTextEdit>
#include <qlineedit.h>


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