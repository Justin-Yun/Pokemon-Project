//Justin Yun
//CSCI 3010
//Final Project

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "function.h"
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
#include <QMessageBox>
#include <QPainter>



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

int CPU::cpu_move = 0;
bool CPU::mode_bool = false;
int CPU::cpu_row = 0;
int CPU::cpu_col = 5;



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

    graph = new QTableWidget(row, column,this); //Set Table
    graph->horizontalHeader()->setDefaultSectionSize(60); //Width of cells
    graph->verticalHeader()->setDefaultSectionSize(60); //Heights of cells
    graph->setGeometry(350,25,375, 375); //Adjust size of table
    graph->verticalHeader()->setVisible(false); //Turn off header for table
    graph->horizontalHeader()->setVisible(false);
    graph->setStyleSheet("QTableView {selection-background-color: gray}"); //Changes the select's color
    graph->setEditTriggers(QAbstractItemView::NoEditTriggers); //Makes table uneditable

    connect(graph, SIGNAL(cellClicked(int,int)), this, SLOT(on_cellWidget_cellClicked(int,int)));
    createGraph();//Creates money graph
    ui->notification_label->setText("Click on the help \nbutton for instructions!");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_attack_button_clicked()
{
    if(player_mode == 0) //Player did not select mode
    {
        qDebug() << "Player mode NOT selected.";
        ui->notification_label->setText("Please select Player Mode.");
    }

    else if(player_mode == 2) //Player selected 2 player mode
    {
        if(option_turn%2 == 0) //First player's turn
        {
            player2_hp = player2_hp - player1_pp; //Update health
            qDebug() << "Player 2 HP:";
            qDebug() << player2_hp;
            std::string hp_label = std::to_string(player2_hp); //Convert integer to string
            ui->player2_hp_label->setText(QString(hp_label.c_str())); //Update player 2's HP

            option_turn++; //Increment turn
            std::string option_count_lab = std::to_string(option_turn); //Convert integer to string
            ui->option_count_label->setText(QString(option_count_lab.c_str())); //Update turn count label
            ui->player_turn_label->setText(QString("Player 2")); //Update to the next player's turn label

        }
        else
        {
            //Exact same function from above, but player 1's stats are updated
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

        if(player1_hp <= 0) //If player's 1 hp is 0 or less
        {
            win2(); //Function that updatess player's 2 win count and resets the game
        }
        else if (player2_hp <= 0)
        {
            win1(); //Function that updates player 1's win count and resets the game
        }

        else {

        }
    }

    else if(player_mode == 1) //If user chooses 1 player mode
    {
        player2_hp = player2_hp - player1_pp; //Update HP
        qDebug() << "Player 2 HP:";
        qDebug() << player2_hp;
        std::string hp_label = std::to_string(player2_hp);
        ui->player2_hp_label->setText(QString(hp_label.c_str())); //Update label

        cpu2_moves(); //Function that chooses what the CPU will do.

        option_turn++; //Increment turn count
        std::string option_count_lab = std::to_string(option_turn);
        ui->option_count_label->setText(QString(option_count_lab.c_str())); //Update label

        CPU::cpu_move++; //Increment CPU move


        if (player2_hp <= 0)
        {
            win1(); //Function that updatess player's 2 win count and resets the game
        }

        else if(player1_hp <= 0)
        {
            win2();//Function that updatess player's 1 win count and resets the game
        }

        else {

        }

    }
}

void MainWindow::on_upgrade_hp_button_clicked()
{
    if(player_mode == 0) //If player doesn't choose a mode
    {
        qDebug() << "Player mode NOT selected.";
        ui->notification_label->setText("Please select Player Mode.");
    }

    else if(player_mode == 2) //User chooses player mode
    {
        if(option_turn%2 == 0) //Player 1's Turn
        {
            qDebug() << option_turn;
            if((player1_money-250) >= 0) //If the player has the funds to purchase the upgrade
            {
                player1_money = player1_money - 250; //Update player's money
                player1_hp = player1_hp + 50; //Update player's hp
                std::string hp_label = std::to_string(player1_hp);
                ui->player1_hp_label->setText(QString(hp_label.c_str())); //Update label
                std::string money_label = std::to_string(player1_money);
                ui->player1_money_label->setText(QString(money_label.c_str())); //Update label

                option_turn++; //Increment turn count
                std::string option_count_lab = std::to_string(option_turn);
                ui->option_count_label->setText(QString(option_count_lab.c_str())); //Update label
                ui->player_turn_label->setText(QString("Player 2")); //Update label
            }
            else{
                qDebug() << "Insufficient Funds.";
                ui->notification_label->setText(QString("Insufficient Funds"));

            }
        }
        else
        {
            if((player2_money-250) >= 0) //If the player has enough money to buy the upgrade
            {
                player2_money = player2_money - 250; //Update money
                player2_hp = player2_hp + 50; //Update HP
                std::string hp_label = std::to_string(player2_hp);
                ui->player2_hp_label->setText(QString(hp_label.c_str())); //Update label
                std::string money_label = std::to_string(player2_money);
                ui->player2_money_label->setText(QString(money_label.c_str())); //Update label

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

    else if(player_mode == 1) //1 Player
    {
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
        else
        {
            qDebug() << "Insufficient Funds.";
            ui->notification_label->setText(QString("Insufficient Funds"));
        }

        //CPU Moves
        cpu2_moves();

        option_turn++;
        std::string option_count_lab = std::to_string(option_turn);
        ui->option_count_label->setText(QString(option_count_lab.c_str()));

        CPU::cpu_move++;
        if (player1_hp <= 0)
        {
            win2();
        }
        else if(player2_hp <=0)
        {
            win1();
        }
    }

}

void MainWindow::on_lower_pp_button_clicked()
{
    if(player_mode == 0)
    {
        qDebug() << "Player mode NOT selected.";
        ui->notification_label->setText("Please select Player Mode.");
    }

    else if(player_mode == 2)
    {
        if(option_turn%2 == 0) //Player 1's Turn
        {
            qDebug() << option_turn;
            if((player1_money-300) >= 0) //If the player has the funds to purchase the upgrade
            {
                player1_money = player1_money - 300; //Update player's money
                player2_pp = player2_pp - 50; //Update other player's pp
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
            if((player2_money-300) >= 0) //If player 2 has enough money to purchase upgrade
            {
                player2_money = player2_money - 300;
                player1_pp = player1_pp - 50; //Update other player's pp
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

    else if(player_mode == 1) //If user chooses one player mode
    {
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

        //CPU Moves
        cpu2_moves();

        option_turn++;
        std::string option_count_lab = std::to_string(option_turn);
        ui->option_count_label->setText(QString(option_count_lab.c_str()));

        CPU::cpu_move++;
        if(player1_hp <=0)
        {
            win2();
        }
        else if(player2_hp <=0)
        {
            win1();
        }
    }
}

void MainWindow::on_upgrade_pp_button_clicked()
{
    if(player_mode == 0)
    {
        qDebug() << "Player mode NOT selected.";
        ui->notification_label->setText("Please select Player Mode.");
    }

    else if(player_mode == 2)
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
            if((player2_money-250) >= 0) //If user has enough money to buy upgrade
            {
                player2_money = player2_money - 250; //Update money
                player2_pp = player2_pp + 50; //Update pp
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

    else if(player_mode == 1) //If user chooses 1 player mode
    {
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

        //CPU Moves
        cpu2_moves();

        option_turn++;
        std::string option_count_lab = std::to_string(option_turn);
        ui->option_count_label->setText(QString(option_count_lab.c_str()));

        CPU::cpu_move++;
        if(player1_hp <= 0)
        {
            win2();
        }
        else if(player2_hp <= 0)
        {
            win1();
        }
    }
}

void MainWindow::on_cellButton_clicked() //Slot for user clicking on a celll
{
    qDebug() << "Cell Button Clicked";
}


void MainWindow::on_cellWidget_cellClicked(int row, int column) //Signal for user clicking on a cell
{
    QColor colorLive1(Qt::yellow);
    QColor colorLive2(Qt::red);
    if(player_mode == 0)
    {
        qDebug() << "Player mode NOT selected.";
        ui->notification_label->setText("Please select Player Mode.");
    }

    else if(player_mode == 2) //Player 2 mode
    {
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
                    qDebug() << "Function";
                    player1_moneyMove(row, column);

                }

                else if(row == (player1_row) && (column == (player1_col+1))) //Right box of starting point
                {
                    player1_moneyMove(row, column);

                }

                else if(row == (player1_row-1) && (column == (player1_col+1))) //Top-Right box of starting point
                {
                    player1_moneyMove(row, column);


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
                    player1_moneyMove(row, column);


                }

                else if(row == (player1_row-1) && (column == (player1_col))) //Up
                {
                    player1_moneyMove(row, column);


                }

                else if(row == (player1_row) && (column == (player1_col-1))) //Left
                {
                    player1_moneyMove(row, column);


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
                    player1_moneyMove(row, column);


                }

                else if(row == (player1_row+1) && (column == (player1_col))) //Up
                {
                    player1_moneyMove(row, column);


                }

                else if(row == (player1_row) && (column == (player1_col+1))) //Left
                {
                    player1_moneyMove(row, column);


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
                    player1_moneyMove(row, column);


                }

                else if(row == (player1_row+1) && (column == (player1_col))) //Up
                {
                    player1_moneyMove(row, column);


                }

                else if(row == (player1_row) && (column == (player1_col-1))) //Left
                {
                    player1_moneyMove(row, column);


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

                else if(row == (player1_row) && (column == (player1_col+1))) //Right
                {
                    player1_moneyMove(row, column);


                }
                else if(row == (player1_row-1) && (column == (player1_col+1))) //Top Right
                {
                    player1_moneyMove(row, column);


                }
                else if(row == (player1_row+1) && (column == (player1_col+1))) //Bottom Right
                {
                    player1_moneyMove(row, column);

                }

                else if(row == (player1_row-1) && (column == (player1_col))) //Up
                {
                    player1_moneyMove(row, column);


                }
                else if(row == (player1_row+1) && (column == (player1_col))) //Down
                {
                    player1_moneyMove(row, column);


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
                    player1_moneyMove(row, column);


                }
                else if(row == (player1_row) && (column == (player1_col+1))) //right
                {
                    player1_moneyMove(row, column);


                }
                else if(row == (player1_row+1) && (column == (player1_col-1))) //Bottom Left
                {
                    player1_moneyMove(row, column);


                }

                else if(row == (player1_row+1) && (column == (player1_col))) //Down
                {
                    player1_moneyMove(row, column);


                }
                else if(row == (player1_row+1) && (column == (player1_col+1))) //Down Right
                {
                    player1_moneyMove(row, column);


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
                    player1_moneyMove(row, column);


                }
                else if(row == (player1_row) && (column == (player1_col+1))) //right
                {
                    player1_moneyMove(row, column);


                }
                else if(row == (player1_row-1) && (column == (player1_col-1))) //Bottom Left
                {
                    player1_moneyMove(row, column);


                }

                else if(row == (player1_row-1) && (column == (player1_col))) //Up
                {
                    player1_moneyMove(row, column);


                }
                else if(row == (player1_row-1) && (column == (player1_col+1))) //Down Right
                {
                    player1_moneyMove(row, column);


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
                    player1_moneyMove(row, column);


                }
                else if(row == (player1_row-1) && (column == (player1_col-1))) //Diagonal
                {
                    player1_moneyMove(row, column);


                }
                else if(row == (player1_row+1) && (column == (player1_col-1))) //Diagonal
                {
                    player1_moneyMove(row, column);


                }

                else if(row == (player1_row-1) && (column == (player1_col))) //Diagonal
                {
                    player1_moneyMove(row, column);


                }
                else if(row == (player1_row+1) && (column == (player1_col))) //Diagonal
                {
                    player1_moneyMove(row, column);


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
                    player1_moneyMove(row, column);


                }
                else if(row == (player1_row-1) && (column == (player1_col))) //Diagonal
                {
                    player1_moneyMove(row, column);


                }

                else if(row == (player1_row) && (column == (player1_col+1))) //Diagonal
                {
                    player1_moneyMove(row, column);


                }

                else if(row == (player1_row) && (column == (player1_col-1))) //Diagonal
                {
                    player1_moneyMove(row, column);


                }

                else if(row == (player1_row+1) && (column == (player1_col+1))) //Diagonal
                {
                    player1_moneyMove(row, column);


                }

                else if(row == (player1_row-1) && (column == (player1_col-1))) //Diagonal
                {
                    player1_moneyMove(row, column);


                }

                else if(row == (player1_row-1) && (column == (player1_col+1))) //Diagonal
                {
                    player1_moneyMove(row, column);


                }

                else if(row == (player1_row+1) && (column == (player1_col-1))) //Diagonal
                {
                    player1_moneyMove(row, column);


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
                    player2_moneyMove(row, column);


                }

                else if(row == (player2_row) && (column == (player2_col+1))) //Right box of starting point
                {
                    player2_moneyMove(row, column);


                }

                else if(row == (player2_row-1) && (column == (player2_col+1))) //Top-Right box of starting point
                {
                    player2_moneyMove(row, column);


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
                    player2_moneyMove(row, column);


                }

                else if(row == (player2_row-1) && (column == (player2_col))) //Up
                {
                    player2_moneyMove(row, column);

                }

                else if(row == (player2_row) && (column == (player2_col-1))) //Left
                {
                    player2_moneyMove(row, column);


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
                    player2_moneyMove(row, column);


                }

                else if(row == (player2_row+1) && (column == (player2_col))) //Up
                {
                    player2_moneyMove(row, column);


                }

                else if(row == (player2_row) && (column == (player2_col+1))) //Left
                {
                    player2_moneyMove(row, column);


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
                    player2_moneyMove(row, column);


                }

                else if(row == (player2_row+1) && (column == (player2_col))) //Up
                {
                    player2_moneyMove(row, column);


                }

                else if(row == (player2_row) && (column == (player2_col-1))) //Left
                {
                    player2_moneyMove(row, column);


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
                    player2_moneyMove(row, column);


                }
                else if(row == (player2_row-1) && (column == (player2_col+1))) //Diagonal
                {
                    player2_moneyMove(row, column);


                }
                else if(row == (player2_row+1) && (column == (player2_col+1))) //Diagonal
                {
                    player2_moneyMove(row, column);


                }

                else if(row == (player2_row-1) && (column == (player2_col))) //Diagonal
                {
                    player2_moneyMove(row, column);


                }
                else if(row == (player2_row+1) && (column == (player2_col))) //Diagonal
                {
                    player2_moneyMove(row, column);


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
                    player2_moneyMove(row, column);


                }

                else if(row == (player2_row) && (column == (player2_col+1))) //right
                {
                    player2_moneyMove(row, column);


                }
                else if(row == (player2_row+1) && (column == (player2_col-1))) //Bottom Left
                {
                    player2_moneyMove(row, column);


                }

                else if(row == (player2_row+1) && (column == (player2_col))) //Bottom
                {
                    player2_moneyMove(row, column);


                }
                else if(row == (player2_row+1) && (column == (player2_col+1))) //Down Right
                {
                    player2_moneyMove(row, column);


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
                    player2_moneyMove(row, column);


                }
                else if(row == (player2_row) && (column == (player2_col+1))) //right
                {
                    player2_moneyMove(row, column);


                }
                else if(row == (player2_row-1) && (column == (player2_col-1))) //Bottom Left
                {
                    player2_moneyMove(row, column);


                }

                else if(row == (player2_row-1) && (column == (player2_col))) //Bottom
                {
                    player2_moneyMove(row, column);


                }
                else if(row == (player2_row-1) && (column == (player2_col+1))) //Down Right
                {
                    player2_moneyMove(row, column);


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
                    player2_moneyMove(row, column);


                }
                else if(row == (player2_row-1) && (column == (player2_col-1))) //Diagonal
                {
                    player2_moneyMove(row, column);


                }
                else if(row == (player2_row+1) && (column == (player2_col-1))) //Diagonal
                {
                    player2_moneyMove(row, column);


                }

                else if(row == (player2_row-1) && (column == (player2_col))) //Diagonal
                {
                    player2_moneyMove(row, column);


                }
                else if(row == (player2_row+1) && (column == (player2_col))) //Diagonal
                {
                    player2_moneyMove(row, column);


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
                    player2_moneyMove(row, column);


                }
                else if(row == (player2_row-1) && (column == (player2_col))) //Diagonal
                {
                    player2_moneyMove(row, column);


                }

                else if(row == (player2_row) && (column == (player2_col+1))) //Diagonal
                {
                    player2_moneyMove(row, column);


                }

                else if(row == (player2_row) && (column == (player2_col-1))) //Diagonal
                {
                    player2_moneyMove(row, column);

                }

                else if(row == (player2_row+1) && (column == (player2_col+1))) //Diagonal
                {
                    player2_moneyMove(row, column);


                }

                else if(row == (player2_row-1) && (column == (player2_col-1))) //Diagonal
                {
                    player2_moneyMove(row, column);


                }

                else if(row == (player2_row-1) && (column == (player2_col+1))) //Diagonal
                {
                    player2_moneyMove(row, column);


                }

                else if(row == (player2_row+1) && (column == (player2_col-1))) //Diagonal
                {
                    player2_moneyMove(row, column);


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

    else if(player_mode == 1)
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
                player1_moneyMove(row, column);
            }

            else if(row == (player1_row) && (column == (player1_col+1))) //Right box of starting point
            {
                player1_moneyMove(row, column);


            }

            else if(row == (player1_row-1) && (column == (player1_col+1))) //Top-Right box of starting point
            {
                player1_moneyMove(row, column);

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
                player1_moneyMove(row, column);


            }

            else if(row == (player1_row-1) && (column == (player1_col))) //Up
            {
                player1_moneyMove(row, column);


            }

            else if(row == (player1_row) && (column == (player1_col-1))) //Left
            {
                player1_moneyMove(row, column);

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
                player1_moneyMove(row, column);


            }

            else if(row == (player1_row+1) && (column == (player1_col))) //Up
            {
                player1_moneyMove(row, column);


            }

            else if(row == (player1_row) && (column == (player1_col+1))) //Left
            {
                player1_moneyMove(row, column);


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
                player1_moneyMove(row, column);


            }

            else if(row == (player1_row+1) && (column == (player1_col))) //Up
            {
                player1_moneyMove(row, column);


            }

            else if(row == (player1_row) && (column == (player1_col-1))) //Left
            {
                player1_moneyMove(row, column);


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
                player1_moneyMove(row, column);


            }
            else if(row == (player1_row-1) && (column == (player1_col+1))) //Diagonal
            {
                player1_moneyMove(row, column);


            }
            else if(row == (player1_row+1) && (column == (player1_col+1))) //Diagonal
            {
                player1_moneyMove(row, column);


            }

            else if(row == (player1_row-1) && (column == (player1_col))) //Diagonal
            {
                player1_moneyMove(row, column);


            }
            else if(row == (player1_row+1) && (column == (player1_col))) //Diagonal
            {
                player1_moneyMove(row, column);


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
                player1_moneyMove(row, column);


            }
            else if(row == (player1_row) && (column == (player1_col+1))) //right
            {
                player1_moneyMove(row, column);


            }
            else if(row == (player1_row+1) && (column == (player1_col-1))) //Bottom Left
            {
                player1_moneyMove(row, column);


            }

            else if(row == (player1_row+1) && (column == (player1_col))) //Bottom
            {
                player1_moneyMove(row, column);


            }
            else if(row == (player1_row+1) && (column == (player1_col+1))) //Down Right
            {
                player1_moneyMove(row, column);


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
                player1_moneyMove(row, column);


            }
            else if(row == (player1_row) && (column == (player1_col+1))) //right
            {
                player1_moneyMove(row, column);


            }
            else if(row == (player1_row-1) && (column == (player1_col-1))) //Bottom Left
            {
                player1_moneyMove(row, column);


            }

            else if(row == (player1_row-1) && (column == (player1_col))) //Bottom
            {
                player1_moneyMove(row, column);


            }
            else if(row == (player1_row-1) && (column == (player1_col+1))) //Down Right
            {
                player1_moneyMove(row, column);


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
                player1_moneyMove(row, column);


            }
            else if(row == (player1_row-1) && (column == (player1_col-1))) //Diagonal
            {
                player1_moneyMove(row, column);


            }
            else if(row == (player1_row+1) && (column == (player1_col-1))) //Diagonal
            {
                player1_moneyMove(row, column);


            }

            else if(row == (player1_row-1) && (column == (player1_col))) //Diagonal
            {
                player1_moneyMove(row, column);


            }
            else if(row == (player1_row+1) && (column == (player1_col))) //Diagonal
            {
                player1_moneyMove(row, column);

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
                player1_moneyMove(row, column);


            }
            else if(row == (player1_row-1) && (column == (player1_col))) //Diagonal
            {
                player1_moneyMove(row, column);


            }

            else if(row == (player1_row) && (column == (player1_col+1))) //Diagonal
            {
                player1_moneyMove(row, column);


            }

            else if(row == (player1_row) && (column == (player1_col-1))) //Diagonal
            {
                player1_moneyMove(row, column);

            }

            else if(row == (player1_row+1) && (column == (player1_col+1))) //Diagonal
            {
                player1_moneyMove(row, column);


            }

            else if(row == (player1_row-1) && (column == (player1_col-1))) //Diagonal
            {
                player1_moneyMove(row, column);


            }

            else if(row == (player1_row-1) && (column == (player1_col+1))) //Diagonal
            {
                player1_moneyMove(row, column);


            }

            else if(row == (player1_row+1) && (column == (player1_col-1))) //Diagonal
            {
                player1_moneyMove(row, column);


            }

            else
            {
                ui->notification_label->setText(QString("Invalid Move."));
            }
        }

        //CPU Moves
        cpu2_moves();

        option_turn++;
        std::string option_count_lab = std::to_string(option_turn);
        ui->option_count_label->setText(QString(option_count_lab.c_str()));
        CPU::cpu_move++;

        if(player1_hp <= 0)
        {
            win2();
        }

        else if(player2_hp <= 0)
        {
            win1();
        }
    }
}

void MainWindow::on_cpu_mode_label_clicked() //CPU only mode
{
    player_mode = 3;
    qDebug() << "CPU Only";
    QMessageBox::about(this, "CPU Notice", "Press the Step button to simulate the game!");


}


void MainWindow::on_player1_mode_button_clicked() //1 Player mode
{
    player_mode = 1;
    qDebug() << "1 Player";

}

void MainWindow::on_player2_mode_label_clicked() //2 Player Mode
{
    player_mode = 2;
    qDebug() << "2 Player";

}


void MainWindow::on_help_button_clicked() //Button that contains instruction for the game
{
    QString instruction = "Welcome! \n\nThe objective of the game is to get the other player's HP to zero.\n";
    QString instruction2 = "\n\nFirst, choose the player mode (1 Player, 2 Player, etc.). \n\nThen, you will have 5 total options to choose from (Attack, Upgrade HP/PP, Lower PP, and clicking on the grid to earn money.";
    QString instruction3 = "\nEach of the options will count as a turn.";
    QString moving_instruction = "\n\nFor moving on the grid, player's can only move on a space that is one space away from their current position.";
    QString instruction4 = "\n\nAttacking will lower the other player's HP. \nPurchasing Upgrading HP/PP will increase the player's specified stats. \nPurchasing the Lower PP will lower the other player's PP.";
    QString instruction5 = "\n\nThe top-left corner displays the current player's turn and the bottom-right corner displays the amount of wins.";
    QMessageBox::about(this, "Help", instruction+instruction2+instruction3+moving_instruction+instruction4+instruction5); //Creates pop-up box

}

void MainWindow::on_step_button_clicked() //Steps for the CPU mode
{
    if(player_mode == 3) //User chooses only CPU mode
    {
        ui->notification_label->setText("Step Button Pressed.");

        cpu1_moves();//Calls CPU 1 moveset
        cpu2_moves(); //Calls CPU 2 moveset

        option_turn++; //Increment turn
        std::string option_count_lab = std::to_string(option_turn);
        ui->option_count_label->setText(QString(option_count_lab.c_str())); //Update label
        CPU::cpu_move++; //Update cpu move counter

        //Condition that checks if either player has zero or less hp
        if (player2_hp <= 0)
        {
            win1();//Function that increments the win and resets the game
        }

        else if(player1_hp <= 0)
        {
            win2();
        }

    }

    else{
        ui->notification_label->setText("Cannot use step in this mode.");
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
}


void MainWindow::createGraph()
{
    QColor colorLive1(Qt::yellow);
    QColor colorLive2(Qt::red);
    //Fill in Column
    for(int i = 0; i < row; i++) //Loop that increments by row, then columns
    {
        for(int j = 0; j < column; j++)
        {
            int rand_start = rand() % 100 + 1; //Generate random number to determine how much money each cell will contain
            if((i == 5) && (j == 0)) //Assign player 1
            {
                graph->setItem(i, j, new QTableWidgetItem);
                graph->item(i,j)->setText("Player One"); //Set the bottom left corner for the player 1
                graph->item(i,j)->setBackgroundColor(colorLive1); //Sets color of player 1
            }

            else if((i == 0) && (j == 5)) //Assign player 2
            {
                graph->setItem(i, j, new QTableWidgetItem);
                graph->item(i,j)->setText("Player Two"); //Sets the top right corner for player 2
                graph->item(i,j)->setBackgroundColor(colorLive2); //Sets color of player 2
            }

            else //Assign money
            {
                //Conditions that randomly assign each cell a random amount of money
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

void MainWindow::grapher()
{
    QBrush redBrush(Qt::red);
    QBrush yellowBrush(Qt::yellow);
    QPen blackPen(Qt::black);
    int ply1_height = 380-player1_win*20; //Scale for the height of the cell
    int ply2_height = 380-player2_win*20;

    if(player1_win >= 20) //Condition that prevents the graph height from bugging out
    {
        ply1_height = 0;
    }
    else if(player2_win >= 20) //Condition that prevents the graph's height from bugging out
    {
        ply2_height = 0;
    }

    else if((player1_win >= 20) && (player2_win >= 20)) //Condition that prevents the graph's height from bugging out
    {
        ply1_height = 0;
        ply2_height = 0;
    }

    QGraphicsView *view = ui->graphicsView;
    scene = new QGraphicsScene;
    view->setScene(scene);
    view->setSceneRect(0,0,view->frameSize().width(),view->frameSize().height()); //Intialize scene size

    blackPen.setWidth(1); //set width
    //Scale rectange by adjusting the height each time a player wins.
    rectangle = scene->addRect(0, ply1_height,65, 380,blackPen, yellowBrush); //First player win height
    rectangle2 = scene->addRect(65, ply2_height,65, 380,blackPen, redBrush); //Second player win height


    view->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff ); //Disable scrollbar
    view->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );

}

void MainWindow::player1_moneyMove(int row, int column)
{
    QColor colorLive1(Qt::yellow);
    QColor colorLive2(Qt::red);
    QString money = graph->item(row,column)->text(); //Grab string for cell
    QString new_money = money.right(2); //Gets the money value as a string
    qDebug() << new_money;
    int final_money = new_money.toInt(); //Convert the string into a integer
    qDebug() << final_money;

    player1_money = player1_money + final_money; //Update money
    std::string money_label = std::to_string(player1_money);
    ui->player1_money_label->setText(QString(money_label.c_str())); //Update label

    //Update Player's Position
    graph->item(player1_row, player1_col)->setText(""); //Leaves previous location blank
    graph->item(player1_row,player1_col)->setBackgroundColor("white"); //Changes the previous location space to white
    player1_row = row; //Update row to current position
    player1_col = column; //Update column to current position
    graph->item(player1_row, player1_col)->setText("Player One"); //Update new cell with player 1 location
    graph->item(player1_row,player1_col)->setBackgroundColor(colorLive1); //Update new player's color in updated position
    ui->notification_label->setText(QString("Successfully Moved.")); //Lets the user know the move was successful by updating the label
}

void MainWindow::player2_moneyMove(int row, int column)
{
    //Same functionality as the last part, but this code is focused on player 2.
    QColor colorLive1(Qt::yellow);
    QColor colorLive2(Qt::red);
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

void MainWindow::cpu1_moves()
{
    // Player 1 CPU Moves
    if(player1_money-250 >= 0) //If the user has enough money to purchase upgrades
    {
        int choice_random = rand() % 100 + 1;
        qDebug() << choice_random;
        if(choice_random <= 33) //Upgrade HP
        {
            qDebug() << "CPU Upgrade HP";
            player1_money = player1_money - 250; //Update money
            player1_hp = player1_hp + 50; //Update hp
            std::string hp_label = std::to_string(player1_hp);
            ui->player1_hp_label->setText(QString(hp_label.c_str())); //Update label
            std::string money_label = std::to_string(player1_money);
            ui->player1_money_label->setText(QString(money_label.c_str())); //Update label
            std::string option_count_lab = std::to_string(option_turn);
            ui->option_count_label->setText(QString(option_count_lab.c_str())); //Update label
        }

        else if((choice_random > 33) && (choice_random <= 66)) //Upgrade PP
        {
            qDebug() << "CPU Upgrade PP";
            player1_money = player1_money - 250; //Update money
            player1_pp = player1_pp + 50; //Update pp
            std::string pp_label = std::to_string(player1_pp);
            ui->player1_pp_label->setText(QString(pp_label.c_str()));
            std::string money_label = std::to_string(player1_money);
            ui->player1_money_label->setText(QString(money_label.c_str()));
            std::string option_count_lab = std::to_string(option_turn);
            ui->option_count_label->setText(QString(option_count_lab.c_str()));
        }

        else //Attack
        {
            qDebug() << "CPU Attack";
            player2_hp = player2_hp - player1_pp; //Update other player's HP
            qDebug() << player2_hp;
            std::string hp_label2 = std::to_string(player2_hp);
            ui->player2_hp_label->setText(QString(hp_label2.c_str()));
        }
    }

    else //Attack
    {
        qDebug() << "CPU Attack";
        player2_hp = player2_hp - player1_pp; //Update other player's HP
        qDebug() << player2_hp;
        std::string hp_label2 = std::to_string(player2_hp);
        ui->player2_hp_label->setText(QString(hp_label2.c_str()));
    }
}

void MainWindow::cpu2_moves()
{
    //Same functionality as the last function above, but this is focused for player 2 (CPU 2)
    //Player 2 CPU Moves
    if(player2_money-250 >= 0)
    {
        int choice_random = rand() % 100 + 1;
        qDebug() << choice_random;
        if(choice_random <= 33) //Upgrade HP
        {
            qDebug() << "CPU Upgrade HP";
            player2_money = player2_money - 250;
            player2_hp = player2_hp + 50;
            std::string hp_label = std::to_string(player2_hp);
            ui->player2_hp_label->setText(QString(hp_label.c_str()));
            std::string money_label = std::to_string(player2_money);
            ui->player2_money_label->setText(QString(money_label.c_str()));
            std::string option_count_lab = std::to_string(option_turn);
            ui->option_count_label->setText(QString(option_count_lab.c_str()));
        }

        else if((choice_random > 33) && (choice_random <= 66)) //Upgrade PP
        {
            qDebug() << "CPU Upgrade PP";
            player2_money = player2_money - 250;
            player2_pp = player2_pp + 50;
            std::string pp_label = std::to_string(player2_pp);
            ui->player2_pp_label->setText(QString(pp_label.c_str()));
            std::string money_label = std::to_string(player2_money);
            ui->player2_money_label->setText(QString(money_label.c_str()));
            std::string option_count_lab = std::to_string(option_turn);
            ui->option_count_label->setText(QString(option_count_lab.c_str()));
        }

        else //Attack
        {
            qDebug() << "CPU Attack";
            player1_hp = player1_hp - player2_pp;
            qDebug() << "Player 1 HP:";
            qDebug() << player1_hp;
            std::string hp_label2 = std::to_string(player1_hp);
            ui->player1_hp_label->setText(QString(hp_label2.c_str()));
        }
    }

    else //Attack
    {
        qDebug() << "CPU Attack";
        player1_hp = player1_hp - player2_pp;
        qDebug() << "Player 1 HP:";
        qDebug() << player1_hp;
        std::string hp_label2 = std::to_string(player1_hp);
        ui->player1_hp_label->setText(QString(hp_label2.c_str()));
    }
}

void MainWindow::win1()
{
    qDebug() << "Player 1 Wins";
    player1_win++; //Increment amount of wins for player 1
    std::string win_lab = std::to_string(player1_win);
    ui->player1_win_label->setText(QString(win_lab.c_str())); //Update label
    ui->notification_label->setText(QString("Player 1 Won")); //Updates the notification label

    //Restart Stats
    player1_hp = 500;
    player1_pp = 50;
    player1_money = 500;

    player2_hp = 500;
    player2_pp = 50;
    player2_money = 500;
    option_turn = 0;

    //Reset position
    player1_row = 5;
    player1_col = 0;
    player2_row = 0;
    player2_col = 5;

    QColor colorLive1(Qt::yellow);
    QColor colorLive2(Qt::red);

    //Same code from creating the money cell graph
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
                //Assigns each cell with a certain amount of money that is determined by a random number
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
    QMessageBox::about(this, "Win Notification", "Player 1 Won!");
    grapher();
}

void MainWindow::win2()
{
    //Same function as the code above, but it is focused on the second player
    qDebug() <<"Player 2 Wins";
    player2_win++;
    std::string win_lab = std::to_string(player2_win);
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

    QMessageBox::about(this, "Win Notification", "Player 2 Won!");
    grapher();
}


void Player1::test()
{
    qDebug() << "Test 1";
}

void Player2::test()
{
    qDebug() << "Test 2";
}
