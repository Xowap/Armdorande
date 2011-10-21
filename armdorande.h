#ifndef ARMDORANDE_H
#define ARMDORANDE_H

#include <QWidget>

#include "jedicouncil.h"
#include "intro.h"
#include "lightsaber.h"
#include "arena.h"

class Armdorande : public QWidget
{
    Q_OBJECT

public:
    explicit Armdorande(QWidget *parent = 0);
    ~Armdorande();

	enum AppState {
		ShowingCouncil, ShowingIntro, Fighting
	};
	AppState state;

protected slots:
	void killall();
	void setState(AppState);
	void forcePerturbation(Force::FightState);

private slots:
	void showCouncil();
	void showIntro();
	void showFight();
	void handleIntroEnd();
	void lightSaberUp();

signals:
	void stateChanged(AppState);

private:
	JediCouncil *jc;
	Intro *it;
	Force *f;
	LightSaber *ls;
	Arena *ar;
};

#endif // ARMDORANDE_H
