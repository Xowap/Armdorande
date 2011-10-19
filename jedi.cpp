#include "jedi.h"

Jedi::Jedi(int id, QString name, QHostAddress host, QObject *parent) :
	id_(id),
	name_(name),
	host_(host),
	QObject(parent)
{
}

int Jedi::id() {
	return id_;
}

QString Jedi::name() {
	return name_;
}

QHostAddress Jedi::host() {
	return host_;
}
