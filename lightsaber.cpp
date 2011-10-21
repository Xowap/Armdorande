#include "lightsaber.h"
#include "jedi.h"

#include <cwiid.h>
#include <QDebug>

LightSaber::LightSaber(QObject *parent) :
    QObject(parent),bdaddr(*BDADDR_ANY)
{
}
void LightSaber::staticCallback(cwiid_wiimote_t *wiimote, int mesg_count, union cwiid_mesg mesg[], timespec* test){
   LightSaber *obj= (LightSaber*)cwiid_get_data(wiimote);
   (obj->*&LightSaber::wiimote_callback)(mesg_count,mesg,test);
}


bool LightSaber::configure() {
    if (!(wiimote = cwiid_open(BDADDR_ANY, CWIID_FLAG_REPEAT_BTN | CWIID_FLAG_MESG_IFC ))) {
        qDebug()<< "Unable to connect to wiimote\n";
        return false;
    }

    cwiid_set_data(wiimote, this);
    qDebug()<< "Wiimote connected\n";

    cwiid_command(wiimote,CWIID_CMD_LED, CWIID_LED1_ON);

    cwiid_command(wiimote, CWIID_CMD_RPT_MODE, CWIID_RPT_IR|CWIID_RPT_ACC|CWIID_RPT_BTN);
    cwiid_get_acc_cal(wiimote, CWIID_EXT_NONE, &wm_cal);

    cwiid_set_mesg_callback(wiimote,&LightSaber::staticCallback);

    return true;
}

void LightSaber::setRumble(bool r) {
	cwiid_command(wiimote, CWIID_CMD_RUMBLE, r);
}


void LightSaber::wiimote_callback(int mesg_count, union cwiid_mesg mesg[], timespec* test){
	int i,j;

	for(i=0; i<mesg_count; i++) {
		switch(mesg[i].type) {
        case CWIID_MESG_ACC :
            a_x =((double)mesg[i].acc_mesg.acc[CWIID_X] -wm_cal.zero[CWIID_X]) /(wm_cal.one[CWIID_X] - wm_cal.zero[CWIID_X]);
            a_y = ((double)mesg[i].acc_mesg.acc[CWIID_Y] -wm_cal.zero[CWIID_Y]) /(wm_cal.one[CWIID_Y] - wm_cal.zero[CWIID_Y]);
            a_z = ((double)mesg[i].acc_mesg.acc[CWIID_Z] - wm_cal.zero[CWIID_Z]) / (wm_cal.one[CWIID_Z] - wm_cal.zero[CWIID_Z]);

			emit coordonnees(a_x, a_y, a_z);

            break;

        case CWIID_MESG_BTN :
			if(mesg[i].btn_mesg.buttons!=0) {
                qDebug()<< "bouton : "<<mesg[i].btn_mesg.buttons;
			} else if(mesg[i].btn_mesg.buttons==8){

            }

            break;
        }
    }

}
