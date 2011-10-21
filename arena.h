#ifndef ARENA_H
#define ARENA_H

#include <QWidget>
#include <QColor>
#include <QGraphicsLineItem>

#include "force.h"
#include "jedi.h"

struct LightsaberDescriptor {
	double x, y, z;
	double ix, iy;
	double ox, oy, o, l;
	QColor c;
};

class Arena : public QWidget
{
    Q_OBJECT
public:
	explicit Arena(Force *f, QWidget *parent = 0);
	void paintEvent(QPaintEvent *);
	void resizeEvent(QResizeEvent *);

private:
	Force *f_;
	LightsaberDescriptor s1, s2;
	QTimer timer;

	QGraphicsLineItem *paintSaber(LightsaberDescriptor *d, QPainter *p);

	static const double alpha = 0.1;

signals:
	void collides(bool);

public slots:
	void gotPositionUpdate(Jedi *j, double x, double y, double z);

};

#endif // ARENA_H
