#include "mainwindow.h"
#include <QMessageBox>
#include "circle.h"
#include "customgraphicsscene.h"
#include "ui_mainwindow.h"
#include "engine.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), space(new Space()), drawnspace(new DrawnSpace())
{
    ui->setupUi(this);
    scene = new CustomGraphicsScene(0, 0, ui->graphicsView->width(), ui->graphicsView->height());
    ui->graphicsView->setScene(scene);
}

MainWindow::~MainWindow()
{
    delete (drawnspace);
    delete ui;
    delete (scene);
    delete (space);
}


void MainWindow::on_start_clicked()
{

    ui->start->setText("started");
    ui->stop->setText("stop");
    Engine ::getInstance()->start(space, scene, drawnspace);
}

void MainWindow::on_stop_clicked()
{
    ui->start->setText("start");
    ui->stop->setText("stopped");
    Engine ::getInstance()->stop();
}

void MainWindow::showDefaultScene()
{
    scene->setSceneRect(0, 0, ui->graphicsView->width(), ui->graphicsView->height());
    ui->graphicsView->fitInView(scene->sceneRect());
    Engine ::getInstance()->show(space, scene, drawnspace);
}

void MainWindow::on_reload_clicked()
{
    ui->start->setText("start");
    ui->stop->setText("stop");
    Engine ::getInstance()->reload(space, scene, drawnspace);
}

void MainWindow::on_apply_clicked()
{
    if (!Engine::getInstance()->isEngineStopped())
    {
        ui->label->setText("You should stop the game before rescaling");
        return;
    }
    ui->start->setText("start");
    ui->stop->setText("stop");
    delete (space);
    this->space = new Space(ui->scale->value());
    Engine ::getInstance()->show(space, scene, drawnspace);
    ui->label->setText("");
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    ui->graphicsView->fitInView(scene->sceneRect());
    drawnspace->drawSpace(space, scene);
}

void MainWindow::on_rules_clicked()
{
    QMessageBox::information(this, "Rules Of <Game Of Life>", "* in an empty (dead) cell adjacent to three living cells that generate life. \n * if a living cell has two or three living neighbors, then this cell continues to live. \n * otherwise (if fewer than two or more than three neighbors are alive), the cell dies (“from loneliness” or “from overcrowding”).");
}

void MainWindow::on_speed_valueChanged(int arg1)
{
     Engine :: getInstance()->setSpeed(ui->speed->value());
}



