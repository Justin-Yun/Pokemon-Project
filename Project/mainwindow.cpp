#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <QtWidgets>
#include <QPushButton>
#include <QCoreApplication>
#include <QTableWidget>
#include <QPalette>
#include <QString>
#include <QHeaderView>
#include <QTimer>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QStyle>
#include <QDesktopWidget>



int MainWindow::column = 6;
int MainWindow::row = 6;
int MainWindow::player_mode = 0;

int MainWindow::option_turn = 0;

int MainWindow::player1_hp = 500;
int MainWindow::player2_hp = 500;
int MainWindow::player1_pp = 50;
int MainWindow::player2_pp = 50;
int MainWindow::player1_money = 500;
int MainWindow::player2_money = 500;
int MainWindow::player1_win = 0;
int MainWindow::player2_win = 0;

int MainWindow::player1_row = 5;
int MainWindow::player1_col = 0;
int MainWindow::player2_row = 0;
int MainWindow::player2_col = 5;




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Set background music
    QMediaPlaylist *playlist = new QMediaPlaylist();
    playlist->addMedia(QUrl("qrc:/sounds/sounds/backgroundmusic.mp3")); //File location of music
    playlist->setPlaybackMode(QMediaPlaylist::Loop); //Loops the music in a playlist

    QMediaPlayer *music = new QMediaPlayer();
    music->setPlaylist(playlist); //Sets playlist to play
    music->play(); //Plays the background music

    qDebug() << "Program Started";
    srand(time(0));

    QTableWidget *view = ui->cellWidget;

    graph = new QTableWidget(row, column,this); //Set Table
    graph->horizontalHeader()->setDefaultSectionSize(60); //Width of cells
    graph->verticalHeader()->setDefaultSectionSize(60); //Heights of cells
    graph->setGeometry(350,25,375, 375); //Adjust size of table
    graph->verticalHeader()->setVisible(false); //Turn off header for table
    graph->horizontalHeader()->setVisible(false);
    graph->setStyleSheet("QTableView {selection-background-color: gray}"); //Changes the select's color
    graph->setEditTriggers(QAbstractItemView::NoEditTriggers); //Makes table uneditable

    connect(graph, SIGNAL(cellClicked(int,int)), this, SLOT(on_cellWidget_cellClicked(int,int)));

    QColor colorLive1(Qt::yellow);
    QColor colorLive2(Qt::red);

    //Fill in Column
    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < column; j++)
        {
            int rand_start = rand() % 100 + 1; //Generate random number to determine how much money each cell will contain
            if((i == 5) && (j == 0)) //Assign player 1
            {
                graph->setItem(i, j, new QTableWidgetItem);
                graph->item(i,j)->setText("Player One");
                graph->item(i,j)->setBackgroundColor(colorLive1);
            }

            else if((i == 0) && (j == 5)) //Assign player 2
            {
                graph->setItem(i, j, new QTableWidgetItem);
                graph->item(i,j)->setText("Player Two");
                graph->item(i,j)->setBackgroundColor(colorLive2);
            }

            else //Assign money
            {
                if(rand_start < 40)
                {
                    graph->setItem(i, j, new QTableWidgetItem);
                    graph->item(i,j)->setText("Money: $10");
                }
                else if((rand_start >= 40) && (rand_start <70))
                {
                    graph->setItem(i, j, new QTableWidgetItem);
                    graph->item(i,j)->setText("Money: $25");
                }
                else if((rand_start >= 70) && (rand_start < 95))
                {
                    graph->setItem(i, j, new QTableWidgetItem);
                    graph->item(i,j)->setText("Money: $50");
                }
                else if(rand_start >= 95)
                {
                    graph->setItem(i, j, new QTableWidgetItem);
                    graph->item(i,j)->setText("Money: $75");
                }
            }
        }
    }





}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_attack_button_clicked()
{
    if(option_turn%2 == 0)
    {
        player2_hp = player2_hp - player1_pp;
        qDebug() << "Player 2 HP:";
        qDebug() << player2_hp;
        std::string hp_label = std::to_string(player2_hp);
        ui->player2_hp_label->setText(QString(hp_label.c_str()));

        option_turn++;
        std::string option_count_lab = std::to_string(option_turn);
        ui->option_count_label->setText(QString(option_count_lab.c_str()));
        ui->player_turn_label->setText(QString("Player 2"));

    }
    else
    {
        player1_hp = player1_hp - player2_pp;
        qDebug() << "Player 1 HP:";
        qDebug() << player1_hp;
        std::string hp_label = std::to_string(player1_hp);
        ui->player1_hp_label->setText(QString(hp_label.c_str()));

        option_turn++;
        std::string option_count_lab = std::to_string(option_turn);
        ui->option_count_label->setText(QString(option_count_lab.c_str()));
        ui->player_turn_label->setText(QString("Player 1"));

    }

    if(player1_hp <= 0)
    {
        qDebug() <<"Player 2 Wins";
        player2_win++;
        std::string win_lab = std::to_string(player1_win);
        ui->player2_win_label->setText(QString(win_lab.c_str()));
        ui->notification_label->setText(QString("Player 2 Won"));

        //Restart Stats
        player1_hp = 500;
        player1_pp = 50;
        player1_money = 500;

        player2_hp = 500;
        player2_pp = 50;
        player2_money = 500;
        option_turn = 0;

        player1_row = 5;
        player1_col = 0;
        player2_row = 0;
        player2_col = 5;

        QColor colorLive1(Qt::yellow);
        QColor colorLive2(Qt::red);

        //Fill in Column
        for(int i = 0; i < row; i++)
        {
            for(int j = 0; j < column; j++)
            {
                int rand_start = rand() % 100 + 1; //Generate random number to determine how much money each cell will contain
                if((i == 5) && (j == 0)) //Assign player 1
                {
                    graph->setItem(i, j, new QTableWidgetItem);
                    graph->item(i,j)->setText("Player One");
                    graph->item(i,j)->setBackgroundColor(colorLive1);
                }

                else if((i == 0) && (j == 5)) //Assign player 2
                {
                    graph->setItem(i, j, new QTableWidgetItem);
                    graph->item(i,j)->setText("Player Two");
                    graph->item(i,j)->setBackgroundColor(colorLive2);
                }

                else //Assign money
                {
                    if(rand_start < 40)
                    {
                        graph->setItem(i, j, new QTableWidgetItem);
                        graph->item(i,j)->setText("Money: $10");
                    }
                    else if((rand_start >= 40) && (rand_start <70))
                    {
                        graph->setItem(i, j, new QTableWidgetItem);
                        graph->item(i,j)->setText("Money: $25");
                    }
                    else if((rand_start >= 70) && (rand_start < 95))
                    {
                        graph->setItem(i, j, new QTableWidgetItem);
                        graph->item(i,j)->setText("Money: $50");
                    }
                    else if(rand_start >= 95)
                    {
                        graph->setItem(i, j, new QTableWidgetItem);
                        graph->item(i,j)->setText("Money: $75");
                    }
                }
            }
        }

        //Update Labels
        ui->player_turn_label->setText(QString("Player 1"));
        std::string option_count_lab = std::to_string(option_turn);
        ui->option_count_label->setText(QString(option_count_lab.c_str()));

        //Player 1 Stats Reset
        std::string hp_label = std::to_string(player1_hp);
        ui->player1_hp_label->setText(QString(hp_label.c_str()));
        std::string money_label = std::to_string(player1_money);
        ui->player1_money_label->setText(QString(money_label.c_str()));
        std::string pp_label = std::to_string(player1_pp);
        ui->player1_pp_label->setText(QString(pp_label.c_str()));

        //Player 2 Stats Reset
        std::string hp_label2 = std::to_string(player2_hp);
        ui->player2_hp_label->setText(QString(hp_label2.c_str()));
        std::string money_label2 = std::to_string(player2_money);
        ui->player2_money_label->setText(QString(money_label2.c_str()));
        std::string pp_label2 = std::to_string(player2_pp);
        ui->player2_pp_label->setText(QString(pp_label2.c_str()));

    }
    else if (player2_hp <= 0)
    {
        qDebug() << "Player 1 Wins";
        player1_win++;
        std::string win_lab = std::to_string(player1_win);
        ui->player1_win_label->setText(QString(win_lab.c_str()));
        ui->notification_label->setText(QString("Player 1 Won"));

        //Restart Stats
        player1_hp = 500;
        player1_pp = 50;
        player1_money = 500;

        player2_hp = 500;
        player2_pp = 50;
        player2_money = 500;
        option_turn = 0;

        player1_row = 5;
        player1_col = 0;
        player2_row = 0;
        player2_col = 5;

        QColor colorLive1(Qt::yellow);
        QColor colorLive2(Qt::red);

        //Fill in Column
        for(int i = 0; i < row; i++)
        {
            for(int j = 0; j < column; j++)
            {
                int rand_start = rand() % 100 + 1; //Generate random number to determine how much money each cell will contain
                if((i == 5) && (j == 0)) //Assign player 1
                {
                    graph->setItem(i, j, new QTableWidgetItem);
                    graph->item(i,j)->setText("Player One");
                    graph->item(i,j)->setBackgroundColor(colorLive1);
                }

                else if((i == 0) && (j == 5)) //Assign player 2
                {
                    graph->setItem(i, j, new QTableWidgetItem);
                    graph->item(i,j)->setText("Player Two");
                    graph->item(i,j)->setBackgroundColor(colorLive2);
                }

                else //Assign money
                {
                    if(rand_start < 40)
                    {
                        graph->setItem(i, j, new QTableWidgetItem);
                        graph->item(i,j)->setText("Money: $10");
                    }
                    else if((rand_start >= 40) && (rand_start <70))
                    {
                        graph->setItem(i, j, new QTableWidgetItem);
                        graph->item(i,j)->setText("Money: $25");
                    }
                    else if((rand_start >= 70) && (rand_start < 95))
                    {
                        graph->setItem(i, j, new QTableWidgetItem);
                        graph->item(i,j)->setText("Money: $50");
                    }
                    else if(rand_start >= 95)
                    {
                        graph->setItem(i, j, new QTableWidgetItem);
                        graph->item(i,j)->setText("Money: $75");
                    }
                }
            }
        }

        //Update Labels
        ui->player_turn_label->setText(QString("Player 1"));
        std::string option_count_lab = std::to_string(option_turn);
        ui->option_count_label->setText(QString(option_count_lab.c_str()));

        //Player 1 Stats Reset
        std::string hp_label = std::to_string(player1_hp);
        ui->player1_hp_label->setText(QString(hp_label.c_str()));
        std::string money_label = std::to_string(player1_money);
        ui->player1_money_label->setText(QString(money_label.c_str()));
        std::string pp_label = std::to_string(player1_pp);
        ui->player1_pp_label->setText(QString(pp_label.c_str()));

        //Player 2 Stats Reset
        std::string hp_label2 = std::to_string(player2_hp);
        ui->player2_hp_label->setText(QString(hp_label2.c_str()));
        std::string money_label2 = std::to_string(player2_money);
        ui->player2_money_label->setText(QString(money_label2.c_str()));
        std::string pp_label2 = std::to_string(player2_pp);
        ui->player2_pp_label->setText(QString(pp_label2.c_str()));
    }

    else {

    }
}

void MainWindow::on_upgrade_hp_button_clicked()
{
    if(option_turn%2 == 0) //Player 1's Turn
    {
        qDebug() << option_turn;
        if((player1_money-250) >= 0) //If the player has the funds to purchase the upgrade
        {
            player1_money = player1_money - 250; //Update player's money
            player1_hp = player1_hp + 50; //Update player's hp
            std::string hp_label = std::to_string(player1_hp);
            ui->player1_hp_label->setText(QString(hp_label.c_str()));
            std::string money_label = std::to_string(player1_money);
            ui->player1_money_label->setText(QString(money_label.c_str()));

            option_turn++;
            std::string option_count_lab = std::to_string(option_turn);
            ui->option_count_label->setText(QString(option_count_lab.c_str()));
            ui->player_turn_label->setText(QString("Player 2"));
        }
        else{
            qDebug() << "Insufficient Funds.";
            ui->notification_label->setText(QString("Insufficient Funds"));

        }
    }
    else
    {
        if((player2_money-250) >= 0)
        {
            player2_money = player2_money - 250;
            player2_hp = player2_hp + 50;
            std::string hp_label = std::to_string(player2_hp);
            ui->player2_hp_label->setText(QString(hp_label.c_str()));
            std::string money_label = std::to_string(player2_money);
            ui->player2_money_label->setText(QString(money_label.c_str()));

            option_turn++;
            std::string option_count_lab = std::to_string(option_turn);
            ui->option_count_label->setText(QString(option_count_lab.c_str()));
            ui->player_turn_label->setText(QString("Player 1"));

        }
        else{
            qDebug() << "Insufficient Funds.";
            ui->notification_label->setText(QString("Insufficient Funds"));

        }
    }
}

void MainWindow::on_lower_pp_button_clicked()
{
    if(option_turn%2 == 0) //Player 1's Turn
    {
        qDebug() << option_turn;
        if((player1_money-300) >= 0) //If the player has the funds to purchase the upgrade
        {
            player1_money = player1_money - 300; //Update player's money
            player2_pp = player2_pp - 50; //Update player's hp
            std::string pp_label = std::to_string(player2_pp);
            ui->player2_pp_label->setText(QString(pp_label.c_str()));
            std::string money_label = std::to_string(player1_money);
            ui->player1_money_label->setText(QString(money_label.c_str()));

            option_turn++;
            std::string option_count_lab = std::to_string(option_turn);
            ui->option_count_label->setText(QString(option_count_lab.c_str()));
            ui->player_turn_label->setText(QString("Player 2"));
        }
        else{
            qDebug() << "Insufficient Funds.";
            ui->notification_label->setText(QString("Insufficient Funds"));

        }
    }
    else
    {
        if((player2_money-300) >= 0)
        {
            player2_money = player2_money - 300;
            player1_pp = player1_pp - 50;
            std::string pp_label = std::to_string(player1_pp);
            ui->player1_pp_label->setText(QString(pp_label.c_str()));
            std::string money_label = std::to_string(player2_money);
            ui->player2_money_label->setText(QString(money_label.c_str()));

            option_turn++;
            std::string option_count_lab = std::to_string(option_turn);
            ui->option_count_label->setText(QString(option_count_lab.c_str()));
            ui->player_turn_label->setText(QString("Player 1"));

        }
        else{
            qDebug() << "Insufficient Funds.";
            ui->notification_label->setText(QString("Insufficient Funds"));
        }
    }
}

void MainWindow::on_upgrade_pp_button_clicked()
{
    if(option_turn%2 == 0) //Player 1's Turn
    {
        qDebug() << option_turn;
        if((player1_money-250) >= 0) //If the player has the funds to purchase the upgrade
        {
            player1_money = player1_money - 250; //Update player's money
            player1_pp = player1_pp + 50; //Update player's hp
            std::string pp_label = std::to_string(player1_pp);
            ui->player1_pp_label->setText(QString(pp_label.c_str()));
            std::string money_label = std::to_string(player1_money);
            ui->player1_money_label->setText(QString(money_label.c_str()));

            option_turn++;
            std::string option_count_lab = std::to_string(option_turn);
            ui->option_count_label->setText(QString(option_count_lab.c_str()));
            ui->player_turn_label->setText(QString("Player 2"));
        }
        else{
            qDebug() << "Insufficient Funds.";
            ui->notification_label->setText(QString("Insufficient Funds"));
        }
    }
    else
    {
        if((player2_money-250) >= 0)
        {
            player2_money = player2_money - 250;
            player2_pp = player2_pp + 50;
            std::string pp_label = std::to_string(player2_pp);
            ui->player2_pp_label->setText(QString(pp_label.c_str()));
            std::string money_label = std::to_string(player2_money);
            ui->player2_money_label->setText(QString(money_label.c_str()));

            option_turn++;
            std::string option_count_lab = std::to_string(option_turn);
            ui->option_count_label->setText(QString(option_count_lab.c_str()));
            ui->player_turn_label->setText(QString("Player 1"));

        }
        else{
            qDebug() << "Insufficient Funds.";
            ui->notification_label->setText(QString("Insufficient Funds"));

        }
    }
}

void MainWindow::on_cellButton_clicked() //Slot for user clicking on a celll
{
    qDebug() << "CellButton Clicked";
}


void MainWindow::on_cellWidget_cellClicked(int row, int column) //Signal for user clicking on a cell
{
    QColor colorLive1(Qt::yellow);
    QColor colorLive2(Qt::red);

    qDebug() << "Pressed Cell";
    qDebug() << "Row, Column";
    qDebug() << row << column;
    qDebug() << graph->item(row,column)->text();

    if(option_turn%2 == 0) //Player 1's Turn
    {
        if((player1_row == 5) && (player1_col == 0)) //Starting Point
        {
            if(row == (player2_row) && (column == (player2_col))) //If player clicks on other player
            {
                ui->notification_label->setText(QString("Invalid Move."));
                //option_turn--;

            }

            else if(row == (player1_row-1) && (column == (player1_col))) //Top box of starting point
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player1_money = player1_money + final_money;
                std::string money_label = std::to_string(player1_money);
                ui->player1_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player1_row, player1_col)->setText("");
                graph->item(player1_row,player1_col)->setBackgroundColor("white");
                player1_row = row;
                player1_col = column;
                graph->item(player1_row, player1_col)->setText("Player One");
                graph->item(player1_row,player1_col)->setBackgroundColor(colorLive1);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else if(row == (player1_row) && (column == (player1_col+1))) //Right box of starting point
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player1_money = player1_money + final_money;
                std::string money_label = std::to_string(player1_money);
                ui->player1_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player1_row, player1_col)->setText("");
                graph->item(player1_row,player1_col)->setBackgroundColor("white");
                player1_row = row;
                player1_col = column;
                graph->item(player1_row, player1_col)->setText("Player One");
                graph->item(player1_row,player1_col)->setBackgroundColor(colorLive1);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else if(row == (player1_row-1) && (column == (player1_col+1))) //Top-Right box of starting point
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player1_money = player1_money + final_money;
                std::string money_label = std::to_string(player1_money);
                ui->player1_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player1_row, player1_col)->setText("");
                graph->item(player1_row,player1_col)->setBackgroundColor("white");
                player1_row = row;
                player1_col = column;
                graph->item(player1_row, player1_col)->setText("Player One");
                graph->item(player1_row,player1_col)->setBackgroundColor(colorLive1);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else
            {
                ui->notification_label->setText(QString("Invalid Move."));

            }
        }

        else if((player1_row == 5) && (player1_col == 5)) //Bot Right Corner
        {
            if(row == (player2_row) && (column == (player2_col))) //If player clicks on other player
            {
                ui->notification_label->setText(QString("Invalid Move."));

            }

            else if(row == (player1_row-1) && (column == (player1_col-1))) //Diagonal
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player1_money = player1_money + final_money;
                std::string money_label = std::to_string(player1_money);
                ui->player1_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player1_row, player1_col)->setText("");
                graph->item(player1_row,player1_col)->setBackgroundColor("white");
                player1_row = row;
                player1_col = column;
                graph->item(player1_row, player1_col)->setText("Player One");
                graph->item(player1_row,player1_col)->setBackgroundColor(colorLive1);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else if(row == (player1_row-1) && (column == (player1_col))) //Up
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player1_money = player1_money + final_money;
                std::string money_label = std::to_string(player1_money);
                ui->player1_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player1_row, player1_col)->setText("");
                graph->item(player1_row,player1_col)->setBackgroundColor("white");
                player1_row = row;
                player1_col = column;
                graph->item(player1_row, player1_col)->setText("Player One");
                graph->item(player1_row,player1_col)->setBackgroundColor(colorLive1);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else if(row == (player1_row) && (column == (player1_col-1))) //Left
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player1_money = player1_money + final_money;
                std::string money_label = std::to_string(player1_money);
                ui->player1_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player1_row, player1_col)->setText("");
                graph->item(player1_row,player1_col)->setBackgroundColor("white");
                player1_row = row;
                player1_col = column;
                graph->item(player1_row, player1_col)->setText("Player One");
                graph->item(player1_row,player1_col)->setBackgroundColor(colorLive1);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else
            {
                ui->notification_label->setText(QString("Invalid Move."));

            }
        }

        else if((player1_row == 0) && (player1_col == 0)) //Top Left Corner
        {
            if(row == (player2_row) && (column == (player2_col))) //If player clicks on other player
            {
                ui->notification_label->setText(QString("Invalid Move."));

            }
            else if(row == (player1_row+1) && (column == (player1_col+1))) //Diagonal
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player1_money = player1_money + final_money;
                std::string money_label = std::to_string(player1_money);
                ui->player1_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player1_row, player1_col)->setText("");
                graph->item(player1_row,player1_col)->setBackgroundColor("white");
                player1_row = row;
                player1_col = column;
                graph->item(player1_row, player1_col)->setText("Player One");
                graph->item(player1_row,player1_col)->setBackgroundColor(colorLive1);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else if(row == (player1_row+1) && (column == (player1_col))) //Up
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player1_money = player1_money + final_money;
                std::string money_label = std::to_string(player1_money);
                ui->player1_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player1_row, player1_col)->setText("");
                graph->item(player1_row,player1_col)->setBackgroundColor("white");
                player1_row = row;
                player1_col = column;
                graph->item(player1_row, player1_col)->setText("Player One");
                graph->item(player1_row,player1_col)->setBackgroundColor(colorLive1);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else if(row == (player1_row) && (column == (player1_col+1))) //Left
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player1_money = player1_money + final_money;
                std::string money_label = std::to_string(player1_money);
                ui->player1_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player1_row, player1_col)->setText("");
                graph->item(player1_row,player1_col)->setBackgroundColor("white");
                player1_row = row;
                player1_col = column;
                graph->item(player1_row, player1_col)->setText("Player One");
                graph->item(player1_row,player1_col)->setBackgroundColor(colorLive1);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else
            {
                ui->notification_label->setText(QString("Invalid Move."));

            }
        }

        else if((player1_row == 0) && (player1_col == 5)) //Top Right Corner
        {
            if(row == (player2_row) && (column == (player2_col))) //If player clicks on other player
            {
                ui->notification_label->setText(QString("Invalid Move."));

            }

            else if(row == (player1_row+1) && (column == (player1_col-1))) //Diagonal
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player1_money = player1_money + final_money;
                std::string money_label = std::to_string(player1_money);
                ui->player1_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player1_row, player1_col)->setText("");
                graph->item(player1_row,player1_col)->setBackgroundColor("white");
                player1_row = row;
                player1_col = column;
                graph->item(player1_row, player1_col)->setText("Player One");
                graph->item(player1_row,player1_col)->setBackgroundColor(colorLive1);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else if(row == (player1_row+1) && (column == (player1_col))) //Up
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player1_money = player1_money + final_money;
                std::string money_label = std::to_string(player1_money);
                ui->player1_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player1_row, player1_col)->setText("");
                graph->item(player1_row,player1_col)->setBackgroundColor("white");
                player1_row = row;
                player1_col = column;
                graph->item(player1_row, player1_col)->setText("Player One");
                graph->item(player1_row,player1_col)->setBackgroundColor(colorLive1);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else if(row == (player1_row) && (column == (player1_col-1))) //Left
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player1_money = player1_money + final_money;
                std::string money_label = std::to_string(player1_money);
                ui->player1_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player1_row, player1_col)->setText("");
                graph->item(player1_row,player1_col)->setBackgroundColor("white");
                player1_row = row;
                player1_col = column;
                graph->item(player1_row, player1_col)->setText("Player One");
                graph->item(player1_row,player1_col)->setBackgroundColor(colorLive1);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else
            {
                ui->notification_label->setText(QString("Invalid Move."));

            }
        }


        else if(((player1_row > 0) && (player1_row < 5)) && (player1_col == 0)) //Left most Column
        {
            if(row == (player2_row) && (column == (player2_col))) //If player clicks on other player
            {
                ui->notification_label->setText(QString("Invalid Move."));

            }

            else if(row == (player1_row) && (column == (player1_col+1))) //Diagonal
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player1_money = player1_money + final_money;
                std::string money_label = std::to_string(player1_money);
                ui->player1_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player1_row, player1_col)->setText("");
                graph->item(player1_row,player1_col)->setBackgroundColor("white");
                player1_row = row;
                player1_col = column;
                graph->item(player1_row, player1_col)->setText("Player One");
                graph->item(player1_row,player1_col)->setBackgroundColor(colorLive1);
                ui->notification_label->setText(QString("Successfully Moved."));

            }
            else if(row == (player1_row-1) && (column == (player1_col+1))) //Diagonal
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player1_money = player1_money + final_money;
                std::string money_label = std::to_string(player1_money);
                ui->player1_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player1_row, player1_col)->setText("");
                graph->item(player1_row,player1_col)->setBackgroundColor("white");
                player1_row = row;
                player1_col = column;
                graph->item(player1_row, player1_col)->setText("Player One");
                graph->item(player1_row,player1_col)->setBackgroundColor(colorLive1);
                ui->notification_label->setText(QString("Successfully Moved."));

            }
            else if(row == (player1_row+1) && (column == (player1_col+1))) //Diagonal
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player1_money = player1_money + final_money;
                std::string money_label = std::to_string(player1_money);
                ui->player1_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player1_row, player1_col)->setText("");
                graph->item(player1_row,player1_col)->setBackgroundColor("white");
                player1_row = row;
                player1_col = column;
                graph->item(player1_row, player1_col)->setText("Player One");
                graph->item(player1_row,player1_col)->setBackgroundColor(colorLive1);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else if(row == (player1_row-1) && (column == (player1_col))) //Diagonal
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player1_money = player1_money + final_money;
                std::string money_label = std::to_string(player1_money);
                ui->player1_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player1_row, player1_col)->setText("");
                graph->item(player1_row,player1_col)->setBackgroundColor("white");
                player1_row = row;
                player1_col = column;
                graph->item(player1_row, player1_col)->setText("Player One");
                graph->item(player1_row,player1_col)->setBackgroundColor(colorLive1);
                ui->notification_label->setText(QString("Successfully Moved."));

            }
            else if(row == (player1_row+1) && (column == (player1_col))) //Diagonal
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player1_money = player1_money + final_money;
                std::string money_label = std::to_string(player1_money);
                ui->player1_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player1_row, player1_col)->setText("");
                graph->item(player1_row,player1_col)->setBackgroundColor("white");
                player1_row = row;
                player1_col = column;
                graph->item(player1_row, player1_col)->setText("Player One");
                graph->item(player1_row,player1_col)->setBackgroundColor(colorLive1);
                ui->notification_label->setText(QString("Successfully Moved."));

            }
            else
            {
                ui->notification_label->setText(QString("Invalid Move."));

            }
        }


        else if(((player1_col > 0) && (player1_col < 5)) && (player1_row == 0)) //Top Row
        {
            if(row == (player2_row) && (column == (player2_col))) //If player clicks on other player
            {
                ui->notification_label->setText(QString("Invalid Move."));

            }
            else if(row == (player1_row) && (column == (player1_col-1))) //left
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player1_money = player1_money + final_money;
                std::string money_label = std::to_string(player1_money);
                ui->player1_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player1_row, player1_col)->setText("");
                graph->item(player1_row,player1_col)->setBackgroundColor("white");
                player1_row = row;
                player1_col = column;
                graph->item(player1_row, player1_col)->setText("Player One");
                graph->item(player1_row,player1_col)->setBackgroundColor(colorLive1);
                ui->notification_label->setText(QString("Successfully Moved."));

            }
            else if(row == (player1_row) && (column == (player1_col+1))) //right
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player1_money = player1_money + final_money;
                std::string money_label = std::to_string(player1_money);
                ui->player1_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player1_row, player1_col)->setText("");
                graph->item(player1_row,player1_col)->setBackgroundColor("white");
                player1_row = row;
                player1_col = column;
                graph->item(player1_row, player1_col)->setText("Player One");
                graph->item(player1_row,player1_col)->setBackgroundColor(colorLive1);
                ui->notification_label->setText(QString("Successfully Moved."));

            }
            else if(row == (player1_row+1) && (column == (player1_col-1))) //Bottom Left
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player1_money = player1_money + final_money;
                std::string money_label = std::to_string(player1_money);
                ui->player1_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player1_row, player1_col)->setText("");
                graph->item(player1_row,player1_col)->setBackgroundColor("white");
                player1_row = row;
                player1_col = column;
                graph->item(player1_row, player1_col)->setText("Player One");
                graph->item(player1_row,player1_col)->setBackgroundColor(colorLive1);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else if(row == (player1_row+1) && (column == (player1_col))) //Bottom
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player1_money = player1_money + final_money;
                std::string money_label = std::to_string(player1_money);
                ui->player1_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player1_row, player1_col)->setText("");
                graph->item(player1_row,player1_col)->setBackgroundColor("white");
                player1_row = row;
                player1_col = column;
                graph->item(player1_row, player1_col)->setText("Player One");
                graph->item(player1_row,player1_col)->setBackgroundColor(colorLive1);
                ui->notification_label->setText(QString("Successfully Moved."));

            }
            else if(row == (player1_row+1) && (column == (player1_col+1))) //Down Right
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player1_money = player1_money + final_money;
                std::string money_label = std::to_string(player1_money);
                ui->player1_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player1_row, player1_col)->setText("");
                graph->item(player1_row,player1_col)->setBackgroundColor("white");
                player1_row = row;
                player1_col = column;
                graph->item(player1_row, player1_col)->setText("Player One");
                graph->item(player1_row,player1_col)->setBackgroundColor(colorLive1);
                ui->notification_label->setText(QString("Successfully Moved."));

            }
            else
            {
                ui->notification_label->setText(QString("Invalid Move."));

            }
        }

        else if(((player1_col > 0) && (player1_col < 5)) && (player1_row == 5)) //Bottom Row
        {
            if(row == (player2_row) && (column == (player2_col))) //If player clicks on other player
            {
                ui->notification_label->setText(QString("Invalid Move."));

            }
            else if(row == (player1_row) && (column == (player1_col-1))) //left
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player1_money = player1_money + final_money;
                std::string money_label = std::to_string(player1_money);
                ui->player1_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player1_row, player1_col)->setText("");
                graph->item(player1_row,player1_col)->setBackgroundColor("white");
                player1_row = row;
                player1_col = column;
                graph->item(player1_row, player1_col)->setText("Player One");
                graph->item(player1_row,player1_col)->setBackgroundColor(colorLive1);
                ui->notification_label->setText(QString("Successfully Moved."));

            }
            else if(row == (player1_row) && (column == (player1_col+1))) //right
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player1_money = player1_money + final_money;
                std::string money_label = std::to_string(player1_money);
                ui->player1_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player1_row, player1_col)->setText("");
                graph->item(player1_row,player1_col)->setBackgroundColor("white");
                player1_row = row;
                player1_col = column;
                graph->item(player1_row, player1_col)->setText("Player One");
                graph->item(player1_row,player1_col)->setBackgroundColor(colorLive1);
                ui->notification_label->setText(QString("Successfully Moved."));

            }
            else if(row == (player1_row-1) && (column == (player1_col-1))) //Bottom Left
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player1_money = player1_money + final_money;
                std::string money_label = std::to_string(player1_money);
                ui->player1_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player1_row, player1_col)->setText("");
                graph->item(player1_row,player1_col)->setBackgroundColor("white");
                player1_row = row;
                player1_col = column;
                graph->item(player1_row, player1_col)->setText("Player One");
                graph->item(player1_row,player1_col)->setBackgroundColor(colorLive1);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else if(row == (player1_row-1) && (column == (player1_col))) //Bottom
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player1_money = player1_money + final_money;
                std::string money_label = std::to_string(player1_money);
                ui->player1_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player1_row, player1_col)->setText("");
                graph->item(player1_row,player1_col)->setBackgroundColor("white");
                player1_row = row;
                player1_col = column;
                graph->item(player1_row, player1_col)->setText("Player One");
                graph->item(player1_row,player1_col)->setBackgroundColor(colorLive1);
                ui->notification_label->setText(QString("Successfully Moved."));

            }
            else if(row == (player1_row-1) && (column == (player1_col+1))) //Down Right
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player1_money = player1_money + final_money;
                std::string money_label = std::to_string(player1_money);
                ui->player1_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player1_row, player1_col)->setText("");
                graph->item(player1_row,player1_col)->setBackgroundColor("white");
                player1_row = row;
                player1_col = column;
                graph->item(player1_row, player1_col)->setText("Player One");
                graph->item(player1_row,player1_col)->setBackgroundColor(colorLive1);
                ui->notification_label->setText(QString("Successfully Moved."));

            }
            else
            {
                ui->notification_label->setText(QString("Invalid Move."));

            }
        }

        else if(((player1_row > 0) && (player1_row < 5)) && (player1_col == 5)) //Left most Column
        {
            if(row == (player2_row) && (column == (player2_col))) //If player clicks on other player
            {
                ui->notification_label->setText(QString("Invalid Move."));

            }
            else if(row == (player1_row) && (column == (player1_col-1))) //Diagonal
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player1_money = player1_money + final_money;
                std::string money_label = std::to_string(player1_money);
                ui->player1_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player1_row, player1_col)->setText("");
                graph->item(player1_row,player1_col)->setBackgroundColor("white");
                player1_row = row;
                player1_col = column;
                graph->item(player1_row, player1_col)->setText("Player One");
                graph->item(player1_row,player1_col)->setBackgroundColor(colorLive1);
                ui->notification_label->setText(QString("Successfully Moved."));

            }
            else if(row == (player1_row-1) && (column == (player1_col-1))) //Diagonal
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player1_money = player1_money + final_money;
                std::string money_label = std::to_string(player1_money);
                ui->player1_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player1_row, player1_col)->setText("");
                graph->item(player1_row,player1_col)->setBackgroundColor("white");
                player1_row = row;
                player1_col = column;
                graph->item(player1_row, player1_col)->setText("Player One");
                graph->item(player1_row,player1_col)->setBackgroundColor(colorLive1);
                ui->notification_label->setText(QString("Successfully Moved."));

            }
            else if(row == (player1_row+1) && (column == (player1_col-1))) //Diagonal
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player1_money = player1_money + final_money;
                std::string money_label = std::to_string(player1_money);
                ui->player1_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player1_row, player1_col)->setText("");
                graph->item(player1_row,player1_col)->setBackgroundColor("white");
                player1_row = row;
                player1_col = column;
                graph->item(player1_row, player1_col)->setText("Player One");
                graph->item(player1_row,player1_col)->setBackgroundColor(colorLive1);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else if(row == (player1_row-1) && (column == (player1_col))) //Diagonal
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player1_money = player1_money + final_money;
                std::string money_label = std::to_string(player1_money);
                ui->player1_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player1_row, player1_col)->setText("");
                graph->item(player1_row,player1_col)->setBackgroundColor("white");
                player1_row = row;
                player1_col = column;
                graph->item(player1_row, player1_col)->setText("Player One");
                graph->item(player1_row,player1_col)->setBackgroundColor(colorLive1);
                ui->notification_label->setText(QString("Successfully Moved."));

            }
            else if(row == (player1_row+1) && (column == (player1_col))) //Diagonal
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player1_money = player1_money + final_money;
                std::string money_label = std::to_string(player1_money);
                ui->player1_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player1_row, player1_col)->setText("");
                graph->item(player1_row,player1_col)->setBackgroundColor("white");
                player1_row = row;
                player1_col = column;
                graph->item(player1_row, player1_col)->setText("Player One");
                graph->item(player1_row,player1_col)->setBackgroundColor(colorLive1);
                ui->notification_label->setText(QString("Successfully Moved."));

            }
            else
            {
                ui->notification_label->setText(QString("Invalid Move."));

            }
        }

        else //Spaces inside the "border"
        {
            if(row == (player2_row) && (column == (player2_col))) //If player clicks on other player
            {
                ui->notification_label->setText(QString("Invalid Move."));

            }
            else if(row == (player1_row+1) && (column == (player1_col))) //Diagonal
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player1_money = player1_money + final_money;
                std::string money_label = std::to_string(player1_money);
                ui->player1_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player1_row, player1_col)->setText("");
                graph->item(player1_row,player1_col)->setBackgroundColor("white");
                player1_row = row;
                player1_col = column;
                graph->item(player1_row, player1_col)->setText("Player One");
                graph->item(player1_row,player1_col)->setBackgroundColor(colorLive1);
                ui->notification_label->setText(QString("Successfully Moved."));

            }
            else if(row == (player1_row-1) && (column == (player1_col))) //Diagonal
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player1_money = player1_money + final_money;
                std::string money_label = std::to_string(player1_money);
                ui->player1_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player1_row, player1_col)->setText("");
                graph->item(player1_row,player1_col)->setBackgroundColor("white");
                player1_row = row;
                player1_col = column;
                graph->item(player1_row, player1_col)->setText("Player One");
                graph->item(player1_row,player1_col)->setBackgroundColor(colorLive1);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else if(row == (player1_row) && (column == (player1_col+1))) //Diagonal
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player1_money = player1_money + final_money;
                std::string money_label = std::to_string(player1_money);
                ui->player1_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player1_row, player1_col)->setText("");
                graph->item(player1_row,player1_col)->setBackgroundColor("white");
                player1_row = row;
                player1_col = column;
                graph->item(player1_row, player1_col)->setText("Player One");
                graph->item(player1_row,player1_col)->setBackgroundColor(colorLive1);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else if(row == (player1_row) && (column == (player1_col-1))) //Diagonal
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player1_money = player1_money + final_money;
                std::string money_label = std::to_string(player1_money);
                ui->player1_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player1_row, player1_col)->setText("");
                graph->item(player1_row,player1_col)->setBackgroundColor("white");
                player1_row = row;
                player1_col = column;
                graph->item(player1_row, player1_col)->setText("Player One");
                graph->item(player1_row,player1_col)->setBackgroundColor(colorLive1);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else if(row == (player1_row+1) && (column == (player1_col+1))) //Diagonal
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player1_money = player1_money + final_money;
                std::string money_label = std::to_string(player1_money);
                ui->player1_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player1_row, player1_col)->setText("");
                graph->item(player1_row,player1_col)->setBackgroundColor("white");
                player1_row = row;
                player1_col = column;
                graph->item(player1_row, player1_col)->setText("Player One");
                graph->item(player1_row,player1_col)->setBackgroundColor(colorLive1);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else if(row == (player1_row-1) && (column == (player1_col-1))) //Diagonal
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player1_money = player1_money + final_money;
                std::string money_label = std::to_string(player1_money);
                ui->player1_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player1_row, player1_col)->setText("");
                graph->item(player1_row,player1_col)->setBackgroundColor("white");
                player1_row = row;
                player1_col = column;
                graph->item(player1_row, player1_col)->setText("Player One");
                graph->item(player1_row,player1_col)->setBackgroundColor(colorLive1);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else if(row == (player1_row-1) && (column == (player1_col+1))) //Diagonal
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player1_money = player1_money + final_money;
                std::string money_label = std::to_string(player1_money);
                ui->player1_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player1_row, player1_col)->setText("");
                graph->item(player1_row,player1_col)->setBackgroundColor("white");
                player1_row = row;
                player1_col = column;
                graph->item(player1_row, player1_col)->setText("Player One");
                graph->item(player1_row,player1_col)->setBackgroundColor(colorLive1);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else if(row == (player1_row+1) && (column == (player1_col-1))) //Diagonal
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player1_money = player1_money + final_money;
                std::string money_label = std::to_string(player1_money);
                ui->player1_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player1_row, player1_col)->setText("");
                graph->item(player1_row,player1_col)->setBackgroundColor("white");
                player1_row = row;
                player1_col = column;
                graph->item(player1_row, player1_col)->setText("Player One");
                graph->item(player1_row,player1_col)->setBackgroundColor(colorLive1);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else
            {
                ui->notification_label->setText(QString("Invalid Move."));
            }
        }

        option_turn++;
        std::string option_count_lab = std::to_string(option_turn);
        ui->option_count_label->setText(QString(option_count_lab.c_str()));
        ui->player_turn_label->setText(QString("Player 2"));
    }

    else //Player's 2 Turn
    {

        if((player1_row == 0) && (player1_col == 5)) //Starting Point
        {
            if(row == (player1_row) && (column == (player1_col))) //If player clicks on other player
            {
                ui->notification_label->setText(QString("Invalid Move."));
                //option_turn--;

            }

            else if(row == (player2_row-1) && (column == (player2_col))) //Top box of starting point
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player2_money = player2_money + final_money;
                std::string money_label = std::to_string(player2_money);
                ui->player2_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player2_row, player2_col)->setText("");
                graph->item(player2_row,player2_col)->setBackgroundColor("white");
                player2_row = row;
                player2_col = column;
                graph->item(player2_row, player2_col)->setText("Player Two");
                graph->item(player2_row,player2_col)->setBackgroundColor(colorLive2);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else if(row == (player2_row) && (column == (player2_col+1))) //Right box of starting point
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player2_money = player2_money + final_money;
                std::string money_label = std::to_string(player2_money);
                ui->player2_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player2_row, player2_col)->setText("");
                graph->item(player2_row,player2_col)->setBackgroundColor("white");
                player2_row = row;
                player2_col = column;
                graph->item(player2_row, player2_col)->setText("Player Two");
                graph->item(player2_row,player2_col)->setBackgroundColor(colorLive2);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else if(row == (player2_row-1) && (column == (player2_col+1))) //Top-Right box of starting point
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player2_money = player2_money + final_money;
                std::string money_label = std::to_string(player2_money);
                ui->player2_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player2_row, player2_col)->setText("");
                graph->item(player2_row,player2_col)->setBackgroundColor("white");
                player2_row = row;
                player2_col = column;
                graph->item(player2_row, player2_col)->setText("Player Two");
                graph->item(player2_row,player2_col)->setBackgroundColor(colorLive2);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else
            {
                ui->notification_label->setText(QString("Invalid Move."));

            }
        }

        else if((player2_row == 5) && (player2_col == 5)) //Bot Right Corner
        {
            if(row == (player1_row) && (column == (player1_col))) //If player clicks on other player
            {
                ui->notification_label->setText(QString("Invalid Move."));

            }

            else if(row == (player2_row-1) && (column == (player2_col-1))) //Diagonal
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player2_money = player2_money + final_money;
                std::string money_label = std::to_string(player2_money);
                ui->player2_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player2_row, player2_col)->setText("");
                graph->item(player2_row,player2_col)->setBackgroundColor("white");
                player2_row = row;
                player2_col = column;
                graph->item(player2_row, player2_col)->setText("Player Two");
                graph->item(player2_row,player2_col)->setBackgroundColor(colorLive2);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else if(row == (player2_row-1) && (column == (player2_col))) //Up
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player2_money = player2_money + final_money;
                std::string money_label = std::to_string(player2_money);
                ui->player2_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player2_row, player2_col)->setText("");
                graph->item(player2_row,player2_col)->setBackgroundColor("white");
                player2_row = row;
                player2_col = column;
                graph->item(player2_row, player2_col)->setText("Player Two");
                graph->item(player2_row,player2_col)->setBackgroundColor(colorLive2);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else if(row == (player2_row) && (column == (player2_col-1))) //Left
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player2_money = player2_money + final_money;
                std::string money_label = std::to_string(player2_money);
                ui->player2_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player2_row, player2_col)->setText("");
                graph->item(player2_row,player2_col)->setBackgroundColor("white");
                player2_row = row;
                player2_col = column;
                graph->item(player2_row, player2_col)->setText("Player Two");
                graph->item(player2_row,player2_col)->setBackgroundColor(colorLive2);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else
            {
                ui->notification_label->setText(QString("Invalid Move."));

            }
        }

        else if((player2_row == 0) && (player2_col == 0)) //Top Left Corner
        {
            if(row == (player1_row) && (column == (player1_col))) //If player clicks on other player
            {
                ui->notification_label->setText(QString("Invalid Move."));

            }
            else if(row == (player2_row+1) && (column == (player2_col+1))) //Diagonal
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player2_money = player2_money + final_money;
                std::string money_label = std::to_string(player2_money);
                ui->player2_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player2_row, player2_col)->setText("");
                graph->item(player2_row,player2_col)->setBackgroundColor("white");
                player2_row = row;
                player2_col = column;
                graph->item(player2_row, player2_col)->setText("Player Two");
                graph->item(player2_row,player2_col)->setBackgroundColor(colorLive2);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else if(row == (player2_row+1) && (column == (player2_col))) //Up
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player2_money = player2_money + final_money;
                std::string money_label = std::to_string(player2_money);
                ui->player2_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player2_row, player2_col)->setText("");
                graph->item(player2_row,player2_col)->setBackgroundColor("white");
                player2_row = row;
                player2_col = column;
                graph->item(player2_row, player2_col)->setText("Player Two");
                graph->item(player2_row,player2_col)->setBackgroundColor(colorLive2);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else if(row == (player2_row) && (column == (player2_col+1))) //Left
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player2_money = player2_money + final_money;
                std::string money_label = std::to_string(player2_money);
                ui->player2_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player2_row, player2_col)->setText("");
                graph->item(player2_row,player2_col)->setBackgroundColor("white");
                player2_row = row;
                player2_col = column;
                graph->item(player2_row, player2_col)->setText("Player Two");
                graph->item(player2_row,player2_col)->setBackgroundColor(colorLive2);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else
            {
                ui->notification_label->setText(QString("Invalid Move."));

            }
        }

        else if((player2_row == 0) && (player2_col == 5)) //Top Right Corner
        {
            if(row == (player1_row) && (column == (player1_col))) //If player clicks on other player
            {
                ui->notification_label->setText(QString("Invalid Move."));

            }

            else if(row == (player2_row+1) && (column == (player2_col-1))) //Diagonal
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player2_money = player2_money + final_money;
                std::string money_label = std::to_string(player2_money);
                ui->player2_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player2_row, player2_col)->setText("");
                graph->item(player2_row,player2_col)->setBackgroundColor("white");
                player2_row = row;
                player2_col = column;
                graph->item(player2_row, player2_col)->setText("Player Two");
                graph->item(player2_row,player2_col)->setBackgroundColor(colorLive2);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else if(row == (player2_row+1) && (column == (player2_col))) //Up
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player2_money = player2_money + final_money;
                std::string money_label = std::to_string(player2_money);
                ui->player2_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player2_row, player2_col)->setText("");
                graph->item(player2_row,player2_col)->setBackgroundColor("white");
                player2_row = row;
                player2_col = column;
                graph->item(player2_row, player2_col)->setText("Player Two");
                graph->item(player2_row,player2_col)->setBackgroundColor(colorLive2);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else if(row == (player2_row) && (column == (player2_col-1))) //Left
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player2_money = player2_money + final_money;
                std::string money_label = std::to_string(player2_money);
                ui->player2_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player2_row, player2_col)->setText("");
                graph->item(player2_row,player2_col)->setBackgroundColor("white");
                player2_row = row;
                player2_col = column;
                graph->item(player2_row, player2_col)->setText("Player Two");
                graph->item(player2_row,player2_col)->setBackgroundColor(colorLive2);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else
            {
                ui->notification_label->setText(QString("Invalid Move."));

            }
        }


        else if(((player2_row > 0) && (player2_row < 5)) && (player2_col == 0)) //Left most Column
        {
            if(row == (player1_row) && (column == (player1_col))) //If player clicks on other player
            {
                ui->notification_label->setText(QString("Invalid Move."));

            }

            else if(row == (player2_row) && (column == (player2_col+1))) //Diagonal
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player2_money = player2_money + final_money;
                std::string money_label = std::to_string(player2_money);
                ui->player2_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player2_row, player2_col)->setText("");
                graph->item(player2_row,player2_col)->setBackgroundColor("white");
                player2_row = row;
                player2_col = column;
                graph->item(player2_row, player2_col)->setText("Player Two");
                graph->item(player2_row,player2_col)->setBackgroundColor(colorLive2);
                ui->notification_label->setText(QString("Successfully Moved."));

            }
            else if(row == (player2_row-1) && (column == (player2_col+1))) //Diagonal
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player2_money = player2_money + final_money;
                std::string money_label = std::to_string(player2_money);
                ui->player2_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player2_row, player2_col)->setText("");
                graph->item(player2_row,player2_col)->setBackgroundColor("white");
                player2_row = row;
                player2_col = column;
                graph->item(player2_row, player2_col)->setText("Player Two");
                graph->item(player2_row,player2_col)->setBackgroundColor(colorLive2);
                ui->notification_label->setText(QString("Successfully Moved."));

            }
            else if(row == (player2_row+1) && (column == (player2_col+1))) //Diagonal
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player2_money = player2_money + final_money;
                std::string money_label = std::to_string(player2_money);
                ui->player2_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player2_row, player2_col)->setText("");
                graph->item(player2_row,player2_col)->setBackgroundColor("white");
                player2_row = row;
                player2_col = column;
                graph->item(player2_row, player2_col)->setText("Player Two");
                graph->item(player2_row,player2_col)->setBackgroundColor(colorLive2);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else if(row == (player2_row-1) && (column == (player2_col))) //Diagonal
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player2_money = player2_money + final_money;
                std::string money_label = std::to_string(player2_money);
                ui->player2_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player2_row, player2_col)->setText("");
                graph->item(player2_row,player2_col)->setBackgroundColor("white");
                player2_row = row;
                player2_col = column;
                graph->item(player2_row, player2_col)->setText("Player Two");
                graph->item(player2_row,player2_col)->setBackgroundColor(colorLive2);
                ui->notification_label->setText(QString("Successfully Moved."));

            }
            else if(row == (player2_row+1) && (column == (player2_col))) //Diagonal
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player2_money = player2_money + final_money;
                std::string money_label = std::to_string(player2_money);
                ui->player2_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player2_row, player2_col)->setText("");
                graph->item(player2_row,player2_col)->setBackgroundColor("white");
                player2_row = row;
                player2_col = column;
                graph->item(player2_row, player2_col)->setText("Player Two");
                graph->item(player2_row,player2_col)->setBackgroundColor(colorLive2);
                ui->notification_label->setText(QString("Successfully Moved."));

            }
            else
            {
                ui->notification_label->setText(QString("Invalid Move."));

            }
        }


        else if(((player2_col > 0) && (player2_col < 5)) && (player2_row == 0)) //Top Row
        {
            if(row == (player1_row) && (column == (player1_col))) //If player clicks on other player
            {
                ui->notification_label->setText(QString("Invalid Move."));

            }
            else if(row == (player2_row) && (column == (player2_col-1))) //left
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player2_money = player2_money + final_money;
                std::string money_label = std::to_string(player2_money);
                ui->player2_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player2_row, player2_col)->setText("");
                graph->item(player2_row,player2_col)->setBackgroundColor("white");
                player2_row = row;
                player2_col = column;
                graph->item(player2_row, player2_col)->setText("Player Two");
                graph->item(player2_row,player2_col)->setBackgroundColor(colorLive2);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else if(row == (player2_row) && (column == (player2_col+1))) //right
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player2_money = player2_money + final_money;
                std::string money_label = std::to_string(player2_money);
                ui->player2_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player2_row, player2_col)->setText("");
                graph->item(player2_row,player2_col)->setBackgroundColor("white");
                player2_row = row;
                player2_col = column;
                graph->item(player2_row, player2_col)->setText("Player Two");
                graph->item(player2_row,player2_col)->setBackgroundColor(colorLive2);
                ui->notification_label->setText(QString("Successfully Moved."));

            }
            else if(row == (player2_row+1) && (column == (player2_col-1))) //Bottom Left
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player2_money = player2_money + final_money;
                std::string money_label = std::to_string(player2_money);
                ui->player2_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player2_row, player2_col)->setText("");
                graph->item(player2_row,player2_col)->setBackgroundColor("white");
                player2_row = row;
                player2_col = column;
                graph->item(player2_row, player2_col)->setText("Player Two");
                graph->item(player2_row,player2_col)->setBackgroundColor(colorLive2);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else if(row == (player2_row+1) && (column == (player2_col))) //Bottom
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player2_money = player2_money + final_money;
                std::string money_label = std::to_string(player2_money);
                ui->player2_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player2_row, player2_col)->setText("");
                graph->item(player2_row,player2_col)->setBackgroundColor("white");
                player2_row = row;
                player2_col = column;
                graph->item(player2_row, player2_col)->setText("Player Two");
                graph->item(player2_row,player2_col)->setBackgroundColor(colorLive2);
                ui->notification_label->setText(QString("Successfully Moved."));

            }
            else if(row == (player2_row+1) && (column == (player2_col+1))) //Down Right
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player2_money = player2_money + final_money;
                std::string money_label = std::to_string(player2_money);
                ui->player2_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player2_row, player2_col)->setText("");
                graph->item(player2_row,player2_col)->setBackgroundColor("white");
                player2_row = row;
                player2_col = column;
                graph->item(player2_row, player2_col)->setText("Player Two");
                graph->item(player2_row,player2_col)->setBackgroundColor(colorLive2);
                ui->notification_label->setText(QString("Successfully Moved."));

            }
            else
            {
                ui->notification_label->setText(QString("Invalid Move."));

            }
        }

        else if(((player2_col > 0) && (player2_col < 5)) && (player2_row == 5)) //Bottom Row
        {
            if(row == (player1_row) && (column == (player1_col))) //If player clicks on other player
            {
                ui->notification_label->setText(QString("Invalid Move."));

            }
            else if(row == (player2_row) && (column == (player2_col-1))) //left
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player2_money = player2_money + final_money;
                std::string money_label = std::to_string(player2_money);
                ui->player2_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player2_row, player2_col)->setText("");
                graph->item(player2_row,player2_col)->setBackgroundColor("white");
                player2_row = row;
                player2_col = column;
                graph->item(player2_row, player2_col)->setText("Player Two");
                graph->item(player2_row,player2_col)->setBackgroundColor(colorLive2);
                ui->notification_label->setText(QString("Successfully Moved."));

            }
            else if(row == (player2_row) && (column == (player2_col+1))) //right
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player2_money = player2_money + final_money;
                std::string money_label = std::to_string(player2_money);
                ui->player2_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player2_row, player2_col)->setText("");
                graph->item(player2_row,player2_col)->setBackgroundColor("white");
                player2_row = row;
                player2_col = column;
                graph->item(player2_row, player2_col)->setText("Player Two");
                graph->item(player2_row,player2_col)->setBackgroundColor(colorLive2);
                ui->notification_label->setText(QString("Successfully Moved."));

            }
            else if(row == (player2_row-1) && (column == (player2_col-1))) //Bottom Left
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player2_money = player2_money + final_money;
                std::string money_label = std::to_string(player2_money);
                ui->player2_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player2_row, player2_col)->setText("");
                graph->item(player2_row,player2_col)->setBackgroundColor("white");
                player2_row = row;
                player2_col = column;
                graph->item(player2_row, player2_col)->setText("Player Two");
                graph->item(player2_row,player2_col)->setBackgroundColor(colorLive2);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else if(row == (player2_row-1) && (column == (player2_col))) //Bottom
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player2_money = player2_money + final_money;
                std::string money_label = std::to_string(player2_money);
                ui->player2_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player2_row, player2_col)->setText("");
                graph->item(player2_row,player2_col)->setBackgroundColor("white");
                player2_row = row;
                player2_col = column;
                graph->item(player2_row, player2_col)->setText("Player Two");
                graph->item(player2_row,player2_col)->setBackgroundColor(colorLive2);
                ui->notification_label->setText(QString("Successfully Moved."));

            }
            else if(row == (player2_row-1) && (column == (player2_col+1))) //Down Right
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player2_money = player2_money + final_money;
                std::string money_label = std::to_string(player2_money);
                ui->player2_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player2_row, player2_col)->setText("");
                graph->item(player2_row,player2_col)->setBackgroundColor("white");
                player2_row = row;
                player2_col = column;
                graph->item(player2_row, player2_col)->setText("Player Two");
                graph->item(player2_row,player2_col)->setBackgroundColor(colorLive2);
                ui->notification_label->setText(QString("Successfully Moved."));

            }
            else
            {
                ui->notification_label->setText(QString("Invalid Move."));

            }
        }

        else if(((player2_row > 0) && (player2_row < 5)) && (player2_col == 5)) //Left most Column
        {
            if(row == (player1_row) && (column == (player1_col))) //If player clicks on other player
            {
                ui->notification_label->setText(QString("Invalid Move."));

            }
            else if(row == (player2_row) && (column == (player2_col-1))) //Diagonal
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player2_money = player2_money + final_money;
                std::string money_label = std::to_string(player2_money);
                ui->player2_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player2_row, player2_col)->setText("");
                graph->item(player2_row,player2_col)->setBackgroundColor("white");
                player2_row = row;
                player2_col = column;
                graph->item(player2_row, player2_col)->setText("Player Two");
                graph->item(player2_row,player2_col)->setBackgroundColor(colorLive2);
                ui->notification_label->setText(QString("Successfully Moved."));

            }
            else if(row == (player2_row-1) && (column == (player2_col-1))) //Diagonal
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player2_money = player2_money + final_money;
                std::string money_label = std::to_string(player2_money);
                ui->player2_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player2_row, player2_col)->setText("");
                graph->item(player2_row,player2_col)->setBackgroundColor("white");
                player2_row = row;
                player2_col = column;
                graph->item(player2_row, player2_col)->setText("Player Two");
                graph->item(player2_row,player2_col)->setBackgroundColor(colorLive2);
                ui->notification_label->setText(QString("Successfully Moved."));

            }
            else if(row == (player2_row+1) && (column == (player2_col-1))) //Diagonal
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player2_money = player2_money + final_money;
                std::string money_label = std::to_string(player2_money);
                ui->player2_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player2_row, player2_col)->setText("");
                graph->item(player2_row,player2_col)->setBackgroundColor("white");
                player2_row = row;
                player2_col = column;
                graph->item(player2_row, player2_col)->setText("Player Two");
                graph->item(player2_row,player2_col)->setBackgroundColor(colorLive2);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else if(row == (player2_row-1) && (column == (player2_col))) //Diagonal
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player2_money = player2_money + final_money;
                std::string money_label = std::to_string(player2_money);
                ui->player2_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player2_row, player2_col)->setText("");
                graph->item(player2_row,player2_col)->setBackgroundColor("white");
                player2_row = row;
                player2_col = column;
                graph->item(player2_row, player2_col)->setText("Player Two");
                graph->item(player2_row,player2_col)->setBackgroundColor(colorLive2);
                ui->notification_label->setText(QString("Successfully Moved."));

            }
            else if(row == (player2_row+1) && (column == (player2_col))) //Diagonal
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player2_money = player2_money + final_money;
                std::string money_label = std::to_string(player2_money);
                ui->player2_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player2_row, player2_col)->setText("");
                graph->item(player2_row,player2_col)->setBackgroundColor("white");
                player2_row = row;
                player2_col = column;
                graph->item(player2_row, player2_col)->setText("Player Two");
                graph->item(player2_row,player2_col)->setBackgroundColor(colorLive2);
                ui->notification_label->setText(QString("Successfully Moved."));

            }
            else
            {
                ui->notification_label->setText(QString("Invalid Move."));

            }
        }

        else //Spaces inside the "border"
        {
            if(row == (player1_row) && (column == (player1_col))) //If player clicks on other player
            {
                ui->notification_label->setText(QString("Invalid Move."));

            }
            else if(row == (player2_row+1) && (column == (player2_col))) //Diagonal
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player2_money = player2_money + final_money;
                std::string money_label = std::to_string(player2_money);
                ui->player2_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player2_row, player2_col)->setText("");
                graph->item(player2_row,player2_col)->setBackgroundColor("white");
                player2_row = row;
                player2_col = column;
                graph->item(player2_row, player2_col)->setText("Player Two");
                graph->item(player2_row,player2_col)->setBackgroundColor(colorLive2);
                ui->notification_label->setText(QString("Successfully Moved."));

            }
            else if(row == (player2_row-1) && (column == (player2_col))) //Diagonal
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player2_money = player2_money + final_money;
                std::string money_label = std::to_string(player2_money);
                ui->player2_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player2_row, player2_col)->setText("");
                graph->item(player2_row,player2_col)->setBackgroundColor("white");
                player2_row = row;
                player2_col = column;
                graph->item(player2_row, player2_col)->setText("Player Two");
                graph->item(player2_row,player2_col)->setBackgroundColor(colorLive2);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else if(row == (player2_row) && (column == (player2_col+1))) //Diagonal
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player2_money = player2_money + final_money;
                std::string money_label = std::to_string(player2_money);
                ui->player2_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player2_row, player2_col)->setText("");
                graph->item(player2_row,player2_col)->setBackgroundColor("white");
                player2_row = row;
                player2_col = column;
                graph->item(player2_row, player2_col)->setText("Player Two");
                graph->item(player2_row,player2_col)->setBackgroundColor(colorLive2);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else if(row == (player2_row) && (column == (player2_col-1))) //Diagonal
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player2_money = player2_money + final_money;
                std::string money_label = std::to_string(player2_money);
                ui->player2_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player2_row, player2_col)->setText("");
                graph->item(player2_row,player2_col)->setBackgroundColor("white");
                player2_row = row;
                player2_col = column;
                graph->item(player2_row, player2_col)->setText("Player Two");
                graph->item(player2_row,player2_col)->setBackgroundColor(colorLive2);
                ui->notification_label->setText(QString("Successfully Moved."));
            }

            else if(row == (player2_row+1) && (column == (player2_col+1))) //Diagonal
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player2_money = player2_money + final_money;
                std::string money_label = std::to_string(player2_money);
                ui->player2_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player2_row, player2_col)->setText("");
                graph->item(player2_row,player2_col)->setBackgroundColor("white");
                player2_row = row;
                player2_col = column;
                graph->item(player2_row, player2_col)->setText("Player Two");
                graph->item(player2_row,player2_col)->setBackgroundColor(colorLive2);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else if(row == (player2_row-1) && (column == (player2_col-1))) //Diagonal
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player2_money = player2_money + final_money;
                std::string money_label = std::to_string(player2_money);
                ui->player2_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player2_row, player2_col)->setText("");
                graph->item(player2_row,player2_col)->setBackgroundColor("white");
                player2_row = row;
                player2_col = column;
                graph->item(player2_row, player2_col)->setText("Player Two");
                graph->item(player2_row,player2_col)->setBackgroundColor(colorLive2);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else if(row == (player2_row-1) && (column == (player2_col+1))) //Diagonal
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player2_money = player2_money + final_money;
                std::string money_label = std::to_string(player2_money);
                ui->player2_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player2_row, player2_col)->setText("");
                graph->item(player2_row,player2_col)->setBackgroundColor("white");
                player2_row = row;
                player2_col = column;
                graph->item(player2_row, player2_col)->setText("Player Two");
                graph->item(player2_row,player2_col)->setBackgroundColor(colorLive2);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else if(row == (player2_row+1) && (column == (player2_col-1))) //Diagonal
            {
                QString money = graph->item(row,column)->text();
                QString new_money = money.right(2);
                qDebug() << new_money;
                int final_money = new_money.toInt();
                qDebug() << final_money;

                player2_money = player2_money + final_money;
                std::string money_label = std::to_string(player2_money);
                ui->player2_money_label->setText(QString(money_label.c_str()));

                //Update Player's Position
                graph->item(player2_row, player2_col)->setText("");
                graph->item(player2_row,player2_col)->setBackgroundColor("white");
                player2_row = row;
                player2_col = column;
                graph->item(player2_row, player2_col)->setText("Player Two");
                graph->item(player2_row,player2_col)->setBackgroundColor(colorLive2);
                ui->notification_label->setText(QString("Successfully Moved."));

            }

            else
            {
                ui->notification_label->setText(QString("Invalid Move."));
            }
        }


        option_turn++;
        std::string option_count_lab = std::to_string(option_turn);
        ui->option_count_label->setText(QString(option_count_lab.c_str()));
        ui->player_turn_label->setText(QString("Player 1"));
    }

}

void MainWindow::on_cpu_mode_label_clicked()
{
    player_mode = 3;
    qDebug() << "CPU Only";

}


void MainWindow::on_player1_mode_button_clicked()
{
    player_mode = 1;
    qDebug() << "1 Player";

}

void MainWindow::on_player2_mode_label_clicked()
{
    player_mode = 2;
    qDebug() << "2 Player";

}

