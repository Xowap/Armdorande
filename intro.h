#ifndef INTRO_H
#define INTRO_H

#include <QWidget>
#include <QTimer>
#include <QStaticText>
#include <QFont>

#include "force.h"

class Intro : public QWidget
{
    Q_OBJECT
public:
	explicit Intro(Force *f, QWidget *parent = 0);
	void paintEvent(QPaintEvent *);
	void resizeEvent(QResizeEvent *);

signals:
	void introFinished();

public slots:

private:
	QString text;
	QStaticText st;
	QTimer timer;
	QFont font;
	QSizeF tsize;
	int tpos;

	static const int step = 3;
};

#endif // INTRO_H
