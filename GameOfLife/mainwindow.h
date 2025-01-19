#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets/QGraphicsScene>
#include "space.h"
#include "drawnspace.h"



QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void showDefaultScene();

private slots:

    void on_start_clicked();

    void on_stop_clicked();

    void on_reload_clicked();

    void on_apply_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    Space * space;
    DrawnSpace * drawnspace;

};

#endif // MAINWINDOW_H
