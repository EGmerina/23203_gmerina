#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "circle.h"
#include "drawnspace.h"
#include "engine.h"


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
    Engine :: getInstance()->start(space, scene);
}


void MainWindow::on_stop_clicked()
{
    Engine :: getInstance()->stop();
}

void MainWindow:: showDefaultScene(){
    scene = new QGraphicsScene(0, 0, ui->graphicsView->width(), ui->graphicsView->height());
    ui->graphicsView->setScene(scene);
    Engine :: getInstance()->show(space, scene);
}

void MainWindow::on_reload_clicked()
{
   Engine :: getInstance()->reload(space, scene);
}

