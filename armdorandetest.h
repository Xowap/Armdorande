#ifndef ARMDORANDETEST_H
#define ARMDORANDETEST_H

#include <QMainWindow>
#include <QHash>

#include "force.h"

namespace Ui {
    class ArmdorandeTest;
}

class ArmdorandeTest : public QMainWindow
{
    Q_OBJECT

public:
    explicit ArmdorandeTest(QWidget *parent = 0);
    ~ArmdorandeTest();

private:
    Ui::ArmdorandeTest *ui;
	Force *f;
	QHash<int, Jedi*> jedis;

private slots:
	void stateChanged(Force::FightState s);
	void gotJedi(Jedi *j);
	void gotSaberUpdate(Jedi *j, double x, double y, double z);
	void displayMessage(QString str);
	void on_pushButton_clicked();
	void on_pushButton_3_clicked();
};

#endif // ARMDORANDETEST_H
