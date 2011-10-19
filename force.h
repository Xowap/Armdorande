#ifndef FORCE_H
#define FORCE_H

#include <QObject>
#include <QVector>
#include <QHash>
#include <QUdpSocket>
#include <QTimer>

#include "jedi.h"

#define FORCE_VERSION 0x01

struct ForceMessage {
};

struct ForceMessagePresence : public ForceMessage {
	QString name;
};

struct ForceMessageDefy : public ForceMessagePresence {
};

struct ForceMessageSaber : public ForceMessage {
	double x, y, z;
};

class Force : public QObject
{
    Q_OBJECT
public:
    explicit Force(QObject *parent = 0);

private:
	QVector<Jedi*> jediIndex;
	QHash<QHostAddress, Jedi*> jediMap;
	QUdpSocket s;
	QString selfName;
	QTimer advTimer;
	Jedi *op;

	static const quint16 udpPort = 42042;

	enum MessageType {
		AnnouncePresence, DefyYou, PissedOnMyself, SaberMoved
	};

	enum FightState {
		WaitingForAction, FightingClient, FightingServer
	};

	QByteArray makeMessage(const MessageType t, const ForceMessage *msg = 0);
	FightState state;

signals:
	void gotJedi(Jedi*);
	void gotEngagedWith(Jedi*);
	void gotSaberUpdate(Jedi*, double x, double y, double z);
	void gotCoward(Jedi*);

public slots:
	void engageWith(int id);
	void updateSaber(double x, double y, double z);
	void retreat();
	void advertise();

private slots:
	void processDatagrams();
};

#endif // FORCE_H