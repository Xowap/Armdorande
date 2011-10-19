#include "force.h"

#include <QHostInfo>

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
}

void Force::processDatagrams() {

}

void Force::engageWith(int id) {
	if(state == WaitingForAction) {
		ForceMessageDefy defy;
		defy.name = selfName;
		op = jediIndex[id];

		if(op) {
			QByteArray ba = makeMessage(DefyYou, &defy);
			s.writeDatagram(ba, op->host(), udpPort);
		}

		state = FightingServer;
		emit gotEngagedWith(op);
	}
}

void Force::retreat() {
	if((state == FightingClient || state == FightingServer) && op) {
		QByteArray ba = makeMessage(PissedOnMyself);
		s.writeDatagram(ba, op->host(), udpPort);

		state = WaitingForAction;
		emit gotCoward(op);
	}
}

void Force::updateSaber(double x, double y, double z) {
	if(op) {
		if(state == FightingClient || state == FightingServer) {
			ForceMessageSaber msg;
			msg.x = x;
			msg.y = y;
			msg.z = z;

			QByteArray ba = makeMessage(SaberMoved, &msg);
			s.writeDatagram(ba, op->host(), udpPort);
		}

		if(state == FightingServer) {
			emit gotSaberUpdate(0, x, y, z);
		}
	}
}

void Force::advertise() {
	ForceMessagePresence msg;
	msg.name = selfName;

	QByteArray ba = makeMessage(AnnouncePresence, &msg);
	s.writeDatagram(ba, QHostAddress(QHostAddress::Broadcast), udpPort);
}

QByteArray Force::makeMessage(const MessageType t, const ForceMessage *msg) {
	ForceMessagePresence *pre_msg = 0;
	ForceMessageSaber *sa_msg = 0;
	QByteArray ret;
	QDataStream st(&ret, QIODevice::WriteOnly);

	st.setByteOrder(QDataStream::LittleEndian);
	st << "adr" << (quint8) FORCE_VERSION;

	switch(t) {
	case AnnouncePresence:
	case DefyYou:
		pre_msg = (ForceMessagePresence*) msg;
		st << pre_msg->name;
		break;

	case SaberMoved:
		sa_msg = (ForceMessageSaber*) msg;
		st << sa_msg->x << sa_msg->y << sa_msg->z;
		break;

	default:
		break;
	}

	return ret;
}
