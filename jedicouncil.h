#ifndef JEDICOUNCIL_H
#define JEDICOUNCIL_H

#include <QWidget>
#include <QHash>
#include <QListWidgetItem>

#include "force.h"

namespace Ui {
    class JediCouncil;
}

class JediCouncil : public QWidget
{
    Q_OBJECT

public:
	explicit JediCouncil(Force *f, QWidget *parent = 0);
    ~JediCouncil();

protected slots:
	void gotJedi(Jedi *j);
	void oponentSelected(QListWidgetItem *i);

private slots:
	void on_grab_clicked();

signals:
	void configLightsaber();

private:
    Ui::JediCouncil *ui;
	Force *f_;
	QHash<int, Jedi*> jedis;
};

#endif // JEDICOUNCIL_H
