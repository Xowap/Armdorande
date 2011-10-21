#ifndef LIGHTSABER_H
#define LIGHTSABER_H

#include <QObject>
#include <cwiid.h>
#include <bluetooth/bluetooth.h>

#include "jedi.h"

class LightSaber : public QObject
{
    Q_OBJECT
public:
    explicit LightSaber(QObject *parent = 0);
    bool configure();
    void wiimote_callback( int mesg_count, union cwiid_mesg mesg[], timespec* test);
    static void staticCallback(cwiid_wiimote_t *wiimote, int mesg_count, union cwiid_mesg mesg[], timespec* test);

signals:
	 void coordonnees(double x, double y, double z);
public slots:
	 void setRumble(bool r);

private :

    cwiid_wiimote_t *wiimote;
    bdaddr_t bdaddr;
    struct acc_cal wm_cal;
    double a_x,a_y,a_z,a;
};

#endif // LIGHTSABER_H
