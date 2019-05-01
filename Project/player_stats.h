#ifndef PLAYER_STATS_H
#define PLAYER_STATS_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QtCore>
#include <QtGui>
#include <QGraphicsView>
#include <QPushButton>
#include <QTableWidget>


class Player : public QMainWindow
{
    Q_OBJECT
public:
    static int player1_hp;
    static int player2_hp;
    static int player1_pp;
    static int player2_pp;
    static int player1_money;
    static int player2_money;
    static int player1_win;
    static int player2_win;

};

#endif // PLAYER_STATS_H
