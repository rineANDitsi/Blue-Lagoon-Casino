#include "casino.h"
#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <thread>
#include <iomanip>
#include <conio.h>
using namespace std;
int SizeOfFile() {
    ifstream fin("CasinoDatabase.txt");
    int size = 0;
    string line;
    while (getline(fin, line)) size++;
    fin.close();
    return size;
}
void CheckingMoney(Clients* client, string login, int size, int bet) {
    for (int i = 0; i < size; i++) {
        Clients& w = client[i];
        if (w.login == login) {
            if (w.balance <= bet) {
                do
                {
                    system("cls");
                    cout << "\033[31mYou have run out of credits!\033[0m\n"
                        << "---------------------\n"
                        << "Balnse - " << w.balance 
                        << "\nPlease top up your balance or die.\n"
                        << "[1] Dodep 1000\n";
                    string choice;
                    cin >> choice;
                    if (choice == "1") {
                        w.balance += 1000;
                        saveClients(client, size);
                    }
                } while (w.balance < 100);
                break;
            }
        }
    }
}
void userStatistics(Clients* client, int size, string login) {
    system("cls");
    for (int i = 0; i < size; i++) {
        Clients& w = client[i];
        if (w.login == login) {
            cout << fixed << setprecision(0);
            cout << "==============================\n"
                << "   \033[36mPLAYER STATISTICS\033[0m\n"
                << "==============================\n"
                << "\033[33mName:\033[0m " << w.name << " (Login: " << w.login << ")\n"
                << "\033[33mAge:\033[0m " << w.age << "\n"
                << "\033[33mBalance:\033[0m " << w.balance << " credits\n"
                << "------------------------------\n"
                << "\033[32mGames Played:\033[0m " << w.totalGamesPlayed << "\n"
                << "\033[32mTotal Wins:\033[0m " << (w.blackjackWon + w.slotWon) << "\n"
                << "------------------------------\n"
                << "\033[35mBlackjack:\033[0m " << w.blackjackPlayed
                << " played, " << w.blackjackWon << " won, " 
                << w.blackjackDraw << " draw\n"
                << "\033[35mSlots:\033[0m " << w.slotsPlayed
                << " played, " << w.slotWon << " won\n"
                << "\033[35mSimon:\033[0m " << w.simonPlayed
                << " played | Total Score: " << w.simonTotalScore
                << " | Max Score: " << w.simonMaxScore << "\n"
                << "------------------------------\n"
                << "\033[36mTotal Bet:\033[0m " << w.totalBet
                << " | \033[36mTotal Win:\033[0m " << w.totalWin << "\n"
                << "==============================\nPress any key to exit";
            char a;
            a = _getch();
            break;
        }
    }
}
void EditClientsData(Clients* client, int size, string game, string login, bool result, double bet, double coefficient) {
    for (int i = 0; i < size; i++) {
        Clients& w = client[i];
        if (w.login == login) {
            if (game == "slots") {
                w.slotsPlayed++;
                w.balance -= bet;
                w.totalBet += bet;
                if (result) {
                    w.balance += bet * coefficient;
                    w.slotWon++;
                    w.totalWin += bet * coefficient;
                }
            }
            if (game == "simon") {
                w.simonPlayed++;
                if (coefficient > w.simonMaxScore) {
                    w.simonMaxScore = coefficient;
                }
                w.simonTotalScore += coefficient;
                w.balance += (20 * coefficient) - bet;
                if (((20 * coefficient) - bet) > 0) w.totalWin += (20 * coefficient) - bet;
            }
            if (game == "blackjac") {
                w.blackjackPlayed++;
                w.totalBet += coefficient;
                if (bet == 0) {
                    w.blackjackDraw;
                }
                else if (result) {
                    w.balance += bet;
                    w.totalWin += bet;
                    w.blackjackWon++;
                }
                else w.balance -= coefficient;
            }
            w.totalGamesPlayed = w.blackjackPlayed + w.simonPlayed + w.slotsPlayed;
            saveClients(client, size);
            break;
        }
    }
}
Clients* TempolaryDatabase(int& size) {
    size = SizeOfFile();
    ifstream fin("CasinoDatabase.txt");
    fin.clear();
    fin.seekg(0, ios::beg);
    if (size == 0) {
        Clients* clients1 = new Clients[1];
        clients1[0].id = 1;
        clients1[0].name = "Admin";
        clients1[0].age = 0;
        clients1[0].balance = 0;
        clients1[0].totalGamesPlayed = 0;
        clients1[0].blackjackPlayed = 0;
        clients1[0].blackjackWon = 0;
        clients1[0].blackjackDraw = 0;
        clients1[0].slotsPlayed = 0;
        clients1[0].slotWon = 0;
        clients1[0].simonPlayed = 0;
        clients1[0].simonTotalScore = 0;
        clients1[0].simonMaxScore = 0;
        clients1[0].totalBet = 0;
        clients1[0].totalWin = 0;
        clients1[0].login = "Admin";
        clients1[0].password = "12";
        size = 1;
        saveClients(clients1, size);
        return clients1;
    }
    Clients* clients = new Clients[size];
    for (int i = 0; i < size; i++) {
        fin >> clients[i].id;
        fin >> clients[i].name;
        fin >> clients[i].age;
        fin >> clients[i].balance;
        fin >> clients[i].totalGamesPlayed;
        fin >> clients[i].blackjackPlayed;
        fin >> clients[i].blackjackWon;
        fin >> clients[i].blackjackDraw;
        fin >> clients[i].slotsPlayed;
        fin >> clients[i].slotWon;
        fin >> clients[i].simonPlayed;
        fin >> clients[i].simonTotalScore;
        fin >> clients[i].simonMaxScore;
        fin >> clients[i].totalBet;
        fin >> clients[i].totalWin;
        fin >> clients[i].login;
        fin >> clients[i].password;
    }
    fin.close();
    return clients;
}

void saveClients(Clients* clients, int size) {
    ofstream fout("CasinoDatabase.txt");
    for (int i = 0; i < size; i++) {
        fout << clients[i].id << " "
            << clients[i].name << " "
            << clients[i].age << " "
            << clients[i].balance << " "
            << clients[i].totalGamesPlayed << " "
            << clients[i].blackjackPlayed << " "
            << clients[i].blackjackWon << " "
            << clients[i].blackjackDraw << " "
            << clients[i].slotsPlayed << " "
            << clients[i].slotWon << " "
            << clients[i].simonPlayed << " "
            << clients[i].simonTotalScore << " "
            << clients[i].simonMaxScore << " "
            << clients[i].totalBet << " "
            << clients[i].totalWin << " "
            << clients[i].login << " "
            << clients[i].password << endl;
    }
    fout.close();
}