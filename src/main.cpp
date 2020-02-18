#include <QApplication>
#include "cpman.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Cpman c;
    c.registerHotkeys(a);
    c.show(); // default to hide

    return a.exec();
}