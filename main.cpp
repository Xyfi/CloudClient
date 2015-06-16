#include "cloud9.hpp"
#include "settings.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    static Settings settings;
    Cloud9 client;

    return a.exec();
}
