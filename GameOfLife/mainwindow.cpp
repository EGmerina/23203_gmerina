#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "circle.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(0, 0, 100, 100);
    ui->graphicsView->setScene(scene);
    new Circle(  5,  5, 20, 20, scene);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete(scene);
}



void MainWindow::on_pushButton_3_clicked()
{

}


void MainWindow::on_pushButton_2_clicked()
{

}

