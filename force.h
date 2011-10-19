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
	bool original;
};

class Force : public QObject
{
    Q_OBJECT
public:
    explicit Force(QObject *parent = 0);

	enum FightState {
		WaitingForAction, FightingClient, FightingServer
	};
	static const int FightBit = 0x2;

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

	QByteArray makeMessage(const MessageType t, const ForceMessage *msg = 0);
	FightState state;

	bool static isSelfAddress(const QHostAddress);

	void processAnnounce(QDataStream *ds, QHostAddress *from);
	void processDefy(QDataStream *ds, QHostAddress *from);
	void processCoward(QDataStream *ds, QHostAddress *from);
	void processSaber(QDataStream *ds, QHostAddress *from);

signals:
	void gotJedi(Jedi*);
	void gotEngagedWith(Jedi*);
	void gotSaberUpdate(Jedi*, double x, double y, double z);
	void gotCoward(Jedi*);
	void stateChanged(Force::FightState s);

public slots:
	void engageWith(int id);
	void updateSaber(double x, double y, double z);
	void retreat();
	void advertise();

private slots:
	void processDatagrams();
	void setState(Force::FightState s);
};

#endif // FORCE_H
