#include "jedicouncil.h"
#include "ui_jedicouncil.h"
#include "lightsaber.h"

#include <QDebug>

JediCouncil::JediCouncil(Force *f, QWidget *parent) :
    QWidget(parent),
	ui(new Ui::JediCouncil),
	f_(f),
	jedis()
{
    ui->setupUi(this);

	connect(f_, SIGNAL(gotJedi(Jedi*)), this, SLOT(gotJedi(Jedi*)));
	connect(ui->registry, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(oponentSelected(QListWidgetItem*)));
}

JediCouncil::~JediCouncil()
{
    delete ui;
}

void JediCouncil::gotJedi(Jedi *j) {
	Jedi *js = jedis[j->id()];

	if(!js) {
			jedis[j->id()] = j;

			QListWidgetItem *wi = new QListWidgetItem(j->name());
			wi->setData(Qt::UserRole, j->id());

			ui->registry->addItem(wi);
	}
}

void JediCouncil::oponentSelected(QListWidgetItem *i) {
	int id = i->data(Qt::UserRole).toInt();
	f_->engageWith(id);
}

void JediCouncil::on_grab_clicked()
{
	emit configLightsaber();
}
