#pragma once
#include <string>

struct Clients {
    int id;
    std::string name;
    int age;
    double balance;
    int totalGamesPlayed;
    int blackjackPlayed;
    int blackjackWon;
    int slotsPlayed;
    int slotWon;
    int simonPlayed;
    int simonTotalScore;
    int simonMaxScore;
    double totalBet;
    double totalWin;
    std::string login;
    std::string password;
};
struct Coefficient {
    std::string name;
    double jackpot;
    double twoMatch;
};

Clients* TempolaryDatabase(int& size);
void saveClients(Clients* clients, int size);
void EditClientsData(Clients* client, int size, std::string game, std::string login, bool result, double bet, double coefficient);
int SizeOfFile();
void CasinoGamesMenu(std::string login);
void blackjac(std::string login);
void slots(std::string login);
void simon(std::string login);
void displaySimonGrid(int& score, std::string login);
void CheckingMoney(Clients* client, std::string login, int size);
void userStatistics(Clients* client, int size, std::string login);