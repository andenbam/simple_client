#include "myclient.h"

#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MyClient* client = new MyClient("",10);
    client->show();

    return app.exec();
}
