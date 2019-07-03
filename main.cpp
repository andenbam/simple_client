#include "myclient.h"

#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MyClient client("localhost",2323);
    client.show();

    return app.exec();
}
