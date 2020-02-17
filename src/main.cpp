#include <QApplication>
#include "cpman.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Cpman c;
    c.hide();

    return a.exec();
}