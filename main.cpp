#include "myclient.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    (new MyClient())->show();
    return app.exec();
}
