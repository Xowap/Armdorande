#include "arena.h"
#include "jedi.h"

#include <math.h>
#include <QPainter>

Arena::Arena(Force *f, QWidget *parent) :
	QWidget(parent),
	f_(f),
	timer()
{
	s1.c = Qt::blue;
	s1.o = 1;
	s1.l = 300;
	s1.x = 0;
	s1.y = 0;
	s1.z = 0;

	s2.c = Qt::red;
	s2.o = -1;
	s2.l = 300;
	s2.x = 0;
	s2.y = 0;
	s2.z = 0;

	connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
	timer.setInterval(33);
	timer.start();
}

void Arena::resizeEvent(QResizeEvent *) {
	s1.ox = 200;
	s1.oy = 100;

	s2.ox = width() - 200;
	s2.oy = 100;
}

void Arena::gotPositionUpdate(Jedi *j, double x, double y, double z) {
	LightsaberDescriptor *d = j->id() ? &s2 : &s1;
	static double nalpha = 1 - alpha;
	double a;

	d->x = alpha * x + nalpha * d->x;
	d->y = alpha * y + nalpha * d->y;
	//d->z = alpha * z + nalpha * d->z;

	a = sqrt(pow(d->x, 2) + pow(d->y, 2));
	d->ix = asin(d->x / a);
	d->iy = asin(d->y / a);
}

void Arena::paintEvent(QPaintEvent *) {
	QPainter p(this);
	QGraphicsLineItem *l1, *l2;

	// Draw background
	p.fillRect(0, 0, width(), height(), Qt::black);

	// Draw lines
	l1 = paintSaber(&s1, &p);
	l2 = paintSaber(&s2, &p);

	emit collides(l1->collidesWithItem(l2));

	delete l1;
	delete l2;
}

QGraphicsLineItem *Arena::paintSaber(LightsaberDescriptor *d, QPainter *p) {
	QGraphicsLineItem *li = new QGraphicsLineItem();

	QPen pen;
	pen.setWidth(5);
	pen.setColor(d->c);

	p->setPen(pen);
	p->resetTransform();

	double rho = - atan2(d->ix, d->iy) + M_PI_2;

	li->setLine(
				d->ox,
				height() - d->oy,
				d->ox + d->o * cos(rho) * d->l,
				height() - d->oy + sin(rho) * d->l
	);
	p->drawLine(li->line());

	return li;
}
