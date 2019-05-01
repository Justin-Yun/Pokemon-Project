#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QtCore>
#include <QtGui>
#include <QGraphicsView>
#include <QPushButton>
#include <QTableWidget>
#include <QPainter>



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static int column; //Table column
    static int row; //Table row

    static int player_mode; //Which player mode the player chose

    static int option_turn;


    //stats for players
    static int player1_hp;
    static int player2_hp;
    static int player1_pp;
    static int player2_pp;
    static int player1_money;
    static int player2_money;
    static int player1_win;
    static int player2_win;


    //Position for both players
    static int player1_row;
    static int player1_col;
    static int player2_row;
    static int player2_col;

    void grapher();
    void player1_moneyMove(int row, int column);
    void player2_moneyMove(int row, int column);
    void cpu1_moves();
    void cpu2_moves();
    virtual void paintEvent(QPaintEvent *event);

private slots:

    void on_attack_button_clicked();

    void on_upgrade_hp_button_clicked();

    void on_lower_pp_button_clicked();

    void on_upgrade_pp_button_clicked();

    void on_cellButton_clicked();


    void on_cellWidget_cellClicked(int row, int column);

    void on_cpu_mode_label_clicked();

    void on_player2_mode_label_clicked();

    void on_player1_mode_button_clicked();

    void on_help_button_clicked();

    void on_step_button_clicked();

private:
    Ui::MainWindow *ui;
    QTableWidget *graph;
    QTimer *timer;
    QGraphicsScene *scene;
    QGraphicsRectItem *rectangle;
    QGraphicsRectItem *rectangle2;
    //QBarSet *set;
    //QBarSeries *series;




};

#endif // MAINWINDOW_H
