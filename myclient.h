#ifndef MYCLIENT_H
#define MYCLIENT_H

#include <QWidget>
#include <QTcpSocket>

class QTextEdit;
class QLineEdit;

class MyClient : public QWidget
{
Q_OBJECT
private:
    QTcpSocket* socket;
    QTextEdit* textEdit;
    QLineEdit* lineEdit;
    qint16 nextBlockSize;

public:
    MyClient(const QString& strHost, int nPort, QWidget* pwgt = nullptr);

private slots:
    void slotReadyRead();
    void slotError();
    void slotSendToServer();
    void slotConnected();
};

#endif // MYCLIENT_H
