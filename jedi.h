#ifndef JEDI_H
#define JEDI_H

#include <QObject>
#include <QHostAddress>

class Jedi : public QObject
{
    Q_OBJECT
public:
	explicit Jedi(int id, QString name, QHostAddress host, QObject *parent = 0);

	int id();
	QString name();
	QHostAddress host();

protected:
	int id_;
	QString name_;
	QHostAddress host_;

signals:

public slots:

};

#endif // JEDI_H
