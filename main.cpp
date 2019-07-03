#include <QApplication>
#include <QStyleFactory>
#include <clientwindow.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    ClientWindow window;

    window.show();
    return app.exec();
}
