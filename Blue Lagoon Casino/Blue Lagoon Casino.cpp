#include "casino.h"
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
using namespace std;

int main()
{
    srand(static_cast<int>(time(0)));
    cout << "===============================\n"
        << "   \033[36mWelcome to the Casino App\033[0m   \n"
        << "===============================\n";
    int user = 0;
    while (user != 1 && user != 2) {
        cout << "Are you a new player or existing?\n";
        cout << "1. New player\n2. Existing player\nChoose 1 or 2: ";
        cin >> user;
        if (user != 1 && user != 2) {
            system("cls");
            cout << "\033[31mInvalid input. Please enter 1 or 2.\033[0m\n";
        }
    }
    system("cls");
    int size;
    Clients* clients = TempolaryDatabase(size);
    if (size == 1) {
        if (user == 2) {
            cout << "Lier lier pants on fire" << endl;
            return 2;
        }
        user = 1;
    }
    string login, password;
    if (user == 1) {
        Clients* newClients = new Clients[size + 1];
        for (int i = 0; i < size; i++) newClients[i] = clients[i];
        bool loginIsFree = false;
        do {
            int a = 0;
            cout << "Enter a login for your account: ";
            cin >> login;
            for (int i = 0; i < size; i++) {
                Clients& w = newClients[i];
                if (w.login == login) {
                    system("cls");
                    cout << "Login \"" << login
                        << "\" is taken\n";
                    break;
                }
                a++;
                if (a == size) loginIsFree = true;
            }
        } while (!loginIsFree);
        cin.ignore();
        cout << "Enter your name: ";
        cin >> newClients[size].name;
        cin.ignore();
        do
        {
            cout << "Enter your age: ";
            cin >> newClients[size].age;
            if (newClients[size].age >= 90) {
                cout << "\033[31mPeople don't live that long!\033[0m\n";
                continue;
            }
            else if (newClients[size].age <= 0) {
                cout << "\033[31mYou haven't been born yet!\033[0m\n";
                continue;
            }
            else break;
        } while (true);
        cin.ignore();
        cout << "Create a password: ";
        cin >> newClients[size].password;
        newClients[size].id = size + 1;
        newClients[size].login = login;
        newClients[size].balance = 1000;
        newClients[size].totalGamesPlayed = 0;
        newClients[size].blackjackPlayed = 0;
        newClients[size].blackjackWon = 0;
        newClients[size].slotsPlayed = 0;
        newClients[size].slotWon = 0;
        newClients[size].simonPlayed = 0;
        newClients[size].simonTotalScore = 0;
        newClients[size].simonMaxScore = 0;
        newClients[size].totalBet = 0.0;
        newClients[size].totalWin = 0.0;
        size++;
        saveClients(newClients, size);
        delete[] clients;
        clients = newClients;
        system("cls");
        cout << "\nHello, " << clients[size - 1].name
            << "! Your account is created with balance: 1000 credits.\n";
    }
    else if (user == 2) {
        bool correct = false;
        int attempts = 0;
        while (!correct) {
            cout << "Enter your login: ";
            cin >> login;
            cout << "Enter your password: ";
            cin >> password;
            for (int i = 0; i < size; i++) {
                if (clients[i].login == login && clients[i].password == password) {
                    correct = true;
                    cout << "\nWelcome back, " << clients[i].name << "!\n";
                    break;
                }
            }
            if (!correct) {
                system("cls");
                cout << "\033[31mIncorrect login or password. Try again.\033[0m\n";
            }
            if (attempts == 5) {
                return 12;
            }
            attempts++;
        }
    }
    int clientCount;
    clients = TempolaryDatabase(clientCount);
    CheckingMoney(clients, login, clientCount);
    CasinoGamesMenu(login);
}