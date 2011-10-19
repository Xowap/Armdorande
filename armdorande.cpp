#include "armdorande.h"
#include "ui_armdorande.h"

Armdorande::Armdorande(QWidget *parent) :
    QMainWindow(parent),
	f(),
    ui(new Ui::Armdorande)
{
    ui->setupUi(this);
}

Armdorande::~Armdorande()
{
    delete ui;
}
