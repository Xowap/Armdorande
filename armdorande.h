#ifndef ARMDORANDE_H
#define ARMDORANDE_H

#include <QMainWindow>

#include "force.h"

namespace Ui {
    class Armdorande;
}

class Armdorande : public QMainWindow
{
    Q_OBJECT

public:
    explicit Armdorande(QWidget *parent = 0);
    ~Armdorande();

private:
    Ui::Armdorande *ui;
	Force f;
};

#endif // ARMDORANDE_H
