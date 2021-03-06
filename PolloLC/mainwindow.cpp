#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <functional>

//Capa de presentacion

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , plcd(new PLCData())
//    , animation_timer(new QTimer(this))
{
    ui->setupUi(this);
    ui->tabWidget->setTabEnabled(1, false);

    scad = new Scadata(plcd);
    plcd->addOnRefreshCallback(std::bind(&MainWindow::updatePLCState, this));

    connect(this->ui->decrementM1, SIGNAL(pressed()), this, SLOT(decrementM1OnClick()));
    connect(this->ui->decrementM2, SIGNAL(pressed()), this, SLOT(decrementM2OnClick()));
    connect(this->ui->decrementM3, SIGNAL(pressed()), this, SLOT(decrementM3OnClick()));
    connect(this->ui->decrementMD, SIGNAL(pressed()), this, SLOT(decrementMDOnClick()));

    //connect(this->animation_timer, SIGNAL(timeout()), this, SLOT(update_animations()));
    //this->animation_timer->start((int) 1000 * 2 / this->frameRate);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::decrementM1OnClick(void) { plcd->decrementNumberAt(0); }
void MainWindow::decrementM2OnClick(void) { plcd->decrementNumberAt(1); }
void MainWindow::decrementM3OnClick(void) { plcd->decrementNumberAt(2); }
void MainWindow::decrementMDOnClick(void) { plcd->decrementNumberAt(3); }

void MainWindow::updatePLCState(void) {
    this->update_UI_1();
//    this->update_UI_2();
}

void MainWindow::update_UI_1(void) {
    this->ui->S0->setStyleSheet(plcd->getBoolAt(0) ? GLED_ON : GLED_OFF);
    this->ui->S1cinta->setStyleSheet(plcd->getBoolAt(1) ? GLED_ON : GLED_OFF);
    this->ui->S2cinta->setStyleSheet(plcd->getBoolAt(2) ? GLED_ON : GLED_OFF);
    this->ui->S3cinta->setStyleSheet(plcd->getBoolAt(3) ? GLED_ON : GLED_OFF);
    this->ui->S1buffer->setStyleSheet(plcd->getBoolAt(4) ? GLED_ON : GLED_OFF);
    this->ui->S2buffer->setStyleSheet(plcd->getBoolAt(5) ? GLED_ON : GLED_OFF);
    this->ui->S3buffer->setStyleSheet(plcd->getBoolAt(6) ? GLED_ON : GLED_OFF);
    this->ui->SDbuffer->setStyleSheet(plcd->getBoolAt(7) ? GLED_ON : GLED_OFF);

    this->ui->C1e->setStyleSheet(plcd->getBoolAt(8) ? GLED_ON : GLED_OFF);
    this->ui->C2e->setStyleSheet(plcd->getBoolAt(9) ? GLED_ON : GLED_OFF);
    this->ui->C3e->setStyleSheet(plcd->getBoolAt(10) ? GLED_ON : GLED_OFF);
    this->ui->C1r->setStyleSheet(plcd->getBoolAt(11) ? GLED_ON : GLED_OFF);
    this->ui->C2r->setStyleSheet(plcd->getBoolAt(12) ? GLED_ON : GLED_OFF);
    this->ui->C3r->setStyleSheet(plcd->getBoolAt(13) ? GLED_ON : GLED_OFF);
    this->ui->C1f->setStyleSheet(plcd->getBoolAt(14) ? GLED_ON : GLED_OFF);
    this->ui->C2f->setStyleSheet(plcd->getBoolAt(15) ? GLED_ON : GLED_OFF);
    this->ui->C3f->setStyleSheet(plcd->getBoolAt(16) ? GLED_ON : GLED_OFF);
    this->ui->C1d->setStyleSheet(plcd->getBoolAt(17) ? GLED_ON : GLED_OFF);
    this->ui->C2d->setStyleSheet(plcd->getBoolAt(18) ? GLED_ON : GLED_OFF);
    this->ui->C3d->setStyleSheet(plcd->getBoolAt(19) ? GLED_ON : GLED_OFF);

    this->ui->M0->setStyleSheet(plcd->getBoolAt(20) ? GLED_ON : GLED_OFF);
    this->ui->M1->setStyleSheet(plcd->getBoolAt(21) ? GLED_ON : GLED_OFF);
    this->ui->M2->setStyleSheet(plcd->getBoolAt(22) ? GLED_ON : GLED_OFF);
    this->ui->M3->setStyleSheet(plcd->getBoolAt(23) ? GLED_ON : GLED_OFF);
    this->ui->M4->setStyleSheet(plcd->getBoolAt(24) ? GLED_ON : GLED_OFF);

    this->ui->M1AceptarProximaCaja->setStyleSheet(plcd->getBoolAt(25) ? GLED_ON : GLED_OFF);
    this->ui->M2AceptarProximaCaja->setStyleSheet(plcd->getBoolAt(26) ? GLED_ON : GLED_OFF);
    this->ui->M3AceptarProximaCaja->setStyleSheet(plcd->getBoolAt(27) ? GLED_ON : GLED_OFF);
    this->ui->DestinoCajaConocido->setStyleSheet(plcd->getBoolAt(28) ? GLED_ON : GLED_OFF);
    this->ui->M1Lleno->setStyleSheet(plcd->getBoolAt(29) ? RLED_ON : RLED_OFF);
    this->ui->M2Lleno->setStyleSheet(plcd->getBoolAt(30) ? RLED_ON : RLED_OFF);
    this->ui->M3Lleno->setStyleSheet(plcd->getBoolAt(31) ? RLED_ON : RLED_OFF);
    this->ui->MDLleno->setStyleSheet(plcd->getBoolAt(32) ? RLED_ON : RLED_OFF);

    this->ui->CajasMuelle1->setText(QString::number(plcd->getNumberAt(0)));
    this->ui->CajasMuelle2->setText(QString::number(plcd->getNumberAt(1)));
    this->ui->CajasMuelle3->setText(QString::number(plcd->getNumberAt(2)));
    this->ui->CajasMuelleD->setText(QString::number(plcd->getNumberAt(3)));
}

//void MainWindow::update_UI_2(void) {
//}

//void MainWindow::update_animations(void) {
//    QWidget* ml = (QWidget*) this->ui->v_ml1;
//    QPoint p = ml->pos();

//    int x = ((p.x() - 1) <= 790) ? 1025 : (p.x() - 1);
//    ml->move(x, p.y());
//}
