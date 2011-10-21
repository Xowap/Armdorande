#include "intro.h"

#include <QLabel>
#include <QDebug>
#include <QFile>
#include <QPainter>

Intro::Intro(Force *f, QWidget *parent) :
	QWidget(parent)
{
	QFile file(":/texts/intro.txt");
	QTextStream in(&file);
	QString tmp;
	QTextOption tops;

	in.setCodec("UTF-8");

	if(file.open(QFile::ReadOnly)) {
		do {
			tmp = in.readLine();
			text.append(tmp + "<br />");
		} while(!(tmp.isNull()));
	}

	st = QStaticText(text);
	st.setTextFormat(Qt::RichText);

	tops.setAlignment(Qt::AlignJustify);
	tops.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);

	font.setBold(true);
	font.setPixelSize(60);

	connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
	timer.setInterval(66);
	timer.start();
}

void Intro::resizeEvent(QResizeEvent *) {
	st.setTextWidth(width() - 100);
	st.prepare(QTransform(), font);

	tsize = st.size();
	tpos = height() + 15 * step;
}

void Intro::paintEvent(QPaintEvent *) {
	tpos -= step;

	//qDebug() << st.size() << tsize.height() << height() << tpos;

	if(tpos < -tsize.height()) {
		emit introFinished();
		return;
	}

	QPainter p(this);
	p.setPen(Qt::yellow);
	p.setFont(font);

	p.fillRect(0, 0, width(), height(), Qt::black);

	p.drawStaticText(50, tpos, st);
}
