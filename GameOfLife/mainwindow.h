#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QMainWindow>
#include <QtWidgets/QGraphicsScene>

#include "drawnspace.h"
#include "space.h"



QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class CustomGraphicsScene: public QGraphicsScene {
public:
    CustomGraphicsScene(int x, int y, int width, int height) : QGraphicsScene(x, y, width, height){}
    ~CustomGraphicsScene()=default;

protected:

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event){
        QList<QGraphicsItem *> items = this->items(event->scenePos());
        if(items.size() > 0 && event->buttons() == Qt::LeftButton){
            static_cast<Circle *>(items[0])->repaintLive();
        }
        if(items.size() > 0 && event->buttons() == Qt::RightButton){
            static_cast<Circle *>(items[0])->repaintDead();
        }
        QGraphicsScene::mouseMoveEvent(event);
    }
};


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

    void on_rules_clicked();

protected:
    void resizeEvent(QResizeEvent* event);

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    Space * space;
    DrawnSpace * drawnspace;

};

#endif // MAINWINDOW_H
