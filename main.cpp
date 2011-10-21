#include <QtGui/QApplication>
#include "armdorande.h"
#include "armdorandetest.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Armdorande w;
	//ArmdorandeTest w;
	w.showFullScreen();

    return a.exec();
}
