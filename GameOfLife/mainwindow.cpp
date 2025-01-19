#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "circle.h"
#include "drawnspace.h"
#include "engine.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), space(new Space()), drawnspace(new DrawnSpace())
{
    ui->setupUi(this);
    scene = new QGraphicsScene(0, 0, ui->graphicsView->width(), ui->graphicsView->height());
    ui->graphicsView->setScene(scene);
}

MainWindow::~MainWindow()
{
    delete(drawnspace);
    delete ui;
    delete(scene);
    delete(space);
}


void MainWindow::on_start_clicked()
{
    Engine :: getInstance()->start(space, scene, drawnspace);
}


void MainWindow::on_stop_clicked()
{
    Engine :: getInstance()->stop();
}

void MainWindow:: showDefaultScene(){
    scene->setSceneRect(0, 0, ui->graphicsView->width(), ui->graphicsView->height());
    ui->graphicsView->fitInView(scene->sceneRect());
    Engine :: getInstance()->show(space, scene, drawnspace);
}

void MainWindow::on_reload_clicked()
{
   Engine :: getInstance()->reload(space, scene, drawnspace);
}


void MainWindow::on_apply_clicked()
{
    //TODO написать что надо остановить игру прежде чем менять масштаб!
    delete(space);
    this->space = new Space(ui->scale->value());
    Engine :: getInstance()->show(space, scene, drawnspace);
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    ui->graphicsView->fitInView(scene->sceneRect());
    drawnspace->drawSpace(space, scene);
}
