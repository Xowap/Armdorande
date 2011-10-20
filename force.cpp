#include "force.h"

#include <QHostInfo>
#include <QDebug>
#include <QNetworkInterface>

Force::Force(QObject *parent) :
	advTimer(),
	op(NULL),
	state(WaitingForAction),
    QObject(parent)
{
	selfName = QHostInfo::localHostName();

	// Create initial Jedi
	Jedi *j = new Jedi(0, selfName, QHostAddress(), this);
	jediIndex.append(j);
	jediMap[j->host()] = j;

	// Create UDP listening socket
	s.bind(udpPort, QUdpSocket::ShareAddress);
	connect(&s, SIGNAL(readyRead()), this, SLOT(processDatagrams()));

	// Advertisment timer
	advTimer.setInterval(100);
	connect(&advTimer, SIGNAL(timeout()), this, SLOT(advertise()));
	advTimer.start();

	emit stateChanged(WaitingForAction);
}

Force::~Force() {
	retreat();
}

void Force::processDatagrams() {
	while(s.hasPendingDatagrams()) {
		QHostAddress from;
		QByteArray ba;
		MessageType t;
		quint8 ver, t_uint;

		ba.resize(s.pendingDatagramSize());
		s.readDatagram(ba.data(), ba.size(), &from);

		QDataStream ds(ba);
		ds.setByteOrder(QDataStream::LittleEndian);

		ds >> ver >> t_uint;
		t = (MessageType) t_uint;

		if(ver != FORCE_VERSION) {
			// Version mismatch
			return;
		}

		if(isSelfAddress(from)) {
			// From self, not interesting
			return;
		}

		switch(t) {
		case AnnouncePresence:
			processAnnounce(&ds, &from);
			break;

		case DefyYou:
			processDefy(&ds, &from);
			break;

		case PissedOnMyself:
			processCoward(&ds, &from);
			break;

		case SaberMoved:
			processSaber(&ds, &from);
			break;
		}
	}
}

void Force::processAnnounce(QDataStream *ds, QHostAddress *from) {
	QString name;
	Jedi *j = 0;

	*ds >> name;

	j = jediMap[*from];
	if(!j) {
		j = new Jedi(jediIndex.count(), name, *from, this);
		jediIndex.append(j);
		jediMap[j->host()] = j;
	}

	emit gotJedi(j);
}

void Force::processCoward(QDataStream *ds, QHostAddress *from) {
	processAnnounce(ds, from);

	Jedi *j = jediMap[*from];

	if(j && (state & FightBit)) {
		setState(WaitingForAction);
		emit gotCoward(j);
	}
}

void Force::processDefy(QDataStream *ds, QHostAddress *from) {
	if(state == WaitingForAction) {
		Jedi *j = jediMap[*from];
		if(j) {
			op = j;
			setState(FightingClient);
			emit gotEngagedWith(j);
		}
	}
}

void Force::processSaber(QDataStream *ds, QHostAddress *from) {
	Jedi *j;
	j = jediMap[*from];

	if(j) {
		double x, y, z;
		bool original;
		ForceMessageSaber msg;

		if(state & FightBit) {
			*ds >> original >> x >> y >> z;

			if(state == FightingServer) {
				msg.x = x;
				msg.y = y;
				msg.z = z;
				msg.original = false;

				QByteArray ba = makeMessage(SaberMoved, &msg);
				s.writeDatagram(ba, op->host(), udpPort);
			}

			emit gotSaberUpdate(original ? j : jediIndex[0], x, y, z);
		}
	}
}

bool Force::isSelfAddress(const QHostAddress a) {
	foreach(QHostAddress ia, QNetworkInterface::allAddresses()) {
		if(ia == a) {
			return true;
		}
	}

	return false;
}

void Force::engageWith(int id) {
	if(state == WaitingForAction) {
		ForceMessageDefy defy;
		defy.name = selfName;
		op = jediIndex[id];

		if(op) {
			QByteArray ba = makeMessage(DefyYou, &defy);
			s.writeDatagram(ba, op->host(), udpPort);

			setState(FightingServer);
			emit gotEngagedWith(op);
		}
	}
}

void Force::retreat() {
	if((state & FightBit) && op) {
		QByteArray ba = makeMessage(PissedOnMyself);
		s.writeDatagram(ba, op->host(), udpPort);

		setState(WaitingForAction);
		emit gotCoward(op);
	}
}

void Force::updateSaber(double x, double y, double z) {
	if(op) {
		if(state & FightBit) {
			ForceMessageSaber msg;
			msg.x = x;
			msg.y = y;
			msg.z = z;
			msg.original = true;

			QByteArray ba = makeMessage(SaberMoved, &msg);
			s.writeDatagram(ba, op->host(), udpPort);
		}

		if(state == FightingServer) {
			emit gotSaberUpdate(0, x, y, z);
		}
	}
}

void Force::advertise() {
	if(state == WaitingForAction) {
		ForceMessagePresence msg;
		msg.name = selfName;

		QByteArray ba = makeMessage(AnnouncePresence, &msg);
		s.writeDatagram(ba, QHostAddress(QHostAddress::Broadcast), udpPort);
	}
}

QByteArray Force::makeMessage(const MessageType t, const ForceMessage *msg) {
	ForceMessagePresence *pre_msg = 0;
	ForceMessageSaber *sa_msg = 0;
	QByteArray ret;
	QDataStream st(&ret, QIODevice::WriteOnly);

	st.setByteOrder(QDataStream::LittleEndian);
	st << (quint8) FORCE_VERSION << (quint8) t;

	switch(t) {
	case AnnouncePresence:
	case DefyYou:
		pre_msg = (ForceMessagePresence*) msg;
		st << pre_msg->name;
		break;

	case SaberMoved:
		sa_msg = (ForceMessageSaber*) msg;
		st << sa_msg->original << sa_msg->x << sa_msg->y << sa_msg->z;
		break;

	default:
		break;
	}

	return ret;
}

void Force::setState(Force::FightState s) {
	state = s;
	emit stateChanged(s);
}
