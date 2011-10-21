#include <QCursor>
#include <QVBoxLayout>
#include "armdorande.h"

Armdorande::Armdorande(QWidget *parent) :
	QWidget(parent),
	jc(0), it(0), ar(0),
	state(ShowingCouncil),
	//state(ShowingIntro),
	f(new Force(this)),
	ls(new LightSaber(this))
{
	setCursor(QCursor(Qt::BlankCursor));
	setLayout(new QVBoxLayout(this));

	showCouncil();
	//showIntro();

	qDebug() << width() << height();

	connect(f, SIGNAL(stateChanged(Force::FightState)), this, SLOT(forcePerturbation(Force::FightState)));
	connect(ls, SIGNAL(coordonnees(double,double,double)), f, SLOT(updateSaber(double,double,double)));
}

Armdorande::~Armdorande()
{
	killall();
}

void Armdorande::setState(AppState s) {
	state = s;

	killall();

	switch(s) {
	case ShowingCouncil:
		showCouncil();
		break;
	case ShowingIntro:
		/*showIntro();
		break;*/
	case Fighting:
		showFight();
		break;
	}

	emit stateChanged(s);
}

void Armdorande::forcePerturbation(Force::FightState s) {
	if(s & Force::FightBit) {
		setState(ShowingIntro);
	} else {
		setState(ShowingCouncil);
	}
}

void Armdorande::killall() {
	// TODO: fuite ?

	if(jc) {
		layout()->removeWidget(jc);
	}

	if(it) {
		layout()->removeWidget(it);
	}

	if(ar) {
		layout()->removeWidget(ar);
	}
}

void Armdorande::showCouncil() {
	jc = new JediCouncil(f, this);
	layout()->addWidget(jc);
	connect(jc, SIGNAL(configLightsaber()), this, SLOT(lightSaberUp()));
}

void Armdorande::showIntro() {
	it = new Intro(f, this);
	layout()->addWidget(it);
	connect(it, SIGNAL(introFinished()), this, SLOT(handleIntroEnd()));
}

void Armdorande::showFight() {
	ar = new Arena(f, this);
	layout()->addWidget(ar);
	connect(f, SIGNAL(gotSaberUpdate(Jedi*,double,double,double)), ar, SLOT(gotPositionUpdate(Jedi*,double,double,double)));
	connect(ar, SIGNAL(collides(bool)), ls, SLOT(setRumble(bool)));
}

void Armdorande::handleIntroEnd() {
	setState(Fighting);
}

void Armdorande::lightSaberUp() {
	ls->configure();
}
