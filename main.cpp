#include "myclient.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MyClient client;
    client.show();
    return app.exec();
}
