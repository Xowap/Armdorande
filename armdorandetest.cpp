#include "armdorandetest.h"
#include "ui_armdorandetest.h"

#include <QLocale>

ArmdorandeTest::ArmdorandeTest(QWidget *parent) :
	f(new Force(this)),
    QMainWindow(parent),
	ui(new Ui::ArmdorandeTest)
{
    ui->setupUi(this);

	connect(f, SIGNAL(stateChanged(Force::FightState)), this, SLOT(stateChanged(Force::FightState)));
	connect(f, SIGNAL(gotJedi(Jedi*)), this, SLOT(gotJedi(Jedi*)));
}

ArmdorandeTest::~ArmdorandeTest()
{
    delete ui;
}

void ArmdorandeTest::stateChanged(Force::FightState s) {
	QString states[3] = { "Waiting", "Fighting (Client)", "Fighting (Server)" };
	displayMessage("State Changed: " + states[s]);
}

void ArmdorandeTest::gotJedi(Jedi *j) {
	displayMessage("Jedi Sensed: " + j->name());

	Jedi *js = jedis[j->id()];

	if(!js) {
		jedis[j->id()] = j;
		ui->oponents->addItem(j->name(), j->id());
	}
}

void ArmdorandeTest::displayMessage(QString str) {
	ui->eventsList->addItem(new QListWidgetItem(str));
	ui->eventsList->scrollToBottom();
}

void ArmdorandeTest::gotSaberUpdate(Jedi *j, double x, double y, double z) {
	displayMessage("Got saber update: " + QString::number(x) + " " + QString::number(y) + " " + QString::number(z));
}

void ArmdorandeTest::on_pushButton_clicked()
{
	int id = ui->oponents->itemData(ui->oponents->currentIndex()).toInt();

	f->engageWith(id);
}

void ArmdorandeTest::on_pushButton_3_clicked()
{
	f->updateSaber(ui->x->value(), ui->y->value(), ui->z->value());
}
