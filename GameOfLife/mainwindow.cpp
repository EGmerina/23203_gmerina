#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "circle.h"
#include "drawnspace.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), space(new Space())
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete(scene);
    delete(space);
}


void MainWindow::on_start_clicked()
{
    engine.start(space, scene);
}


void MainWindow::on_stop_clicked()
{
    engine.stop();
}

void MainWindow:: showDefaultScene(){
    scene = new QGraphicsScene(0, 0, ui->graphicsView->width(), ui->graphicsView->height());
    ui->graphicsView->setScene(scene);
    space->updateSpace();
    DrawnSpace::drawSpace(space, scene);
}



