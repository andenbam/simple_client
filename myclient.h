#ifndef MYCLIENT_H
#define MYCLIENT_H

#include <QWidget>
#include <QTcpSocket>

class QTextEdit;
class QLineEdit;

class myclient : public QWidget
{
Q_OBJECT
private:
    QTcpSocket* socket;
    QTextEdit* textEdit;
    QLineEdit* lineEdit;

public:
    myclient();
};

#endif // MYCLIENT_H
