#include <QtGui/QApplication>
#include "armdorande.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Armdorande w;
    w.show();

    return a.exec();
}
