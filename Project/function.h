#ifndef FUNCTION_H
#define FUNCTION_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QtCore>
#include <QtGui>
#include <QGraphicsView>
#include <QPushButton>
#include <QTableWidget>

class CPU : public QMainWindow
{
    Q_OBJECT

public:
    static int cpu_move;
    static bool mode_bool;
    static int cpu_row;
    static int cpu_col;

    void grapher();
    void cpu_moves();

private:
    int test1_;

};

#endif // FUNCTION_H
