#include "casino.h"
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <conio.h>
using namespace std;
void coefficientCalc(Coefficient* a, string* result, double bet, Clients* clients, int size, string& login) {
    string game = "slots";
    bool win = false;
    if (result[0] == result[1] && result[1] == result[2]) {
        win = true;
        for (int i = 0; i < 5; i++) {
            if (a[i].name == result[0]) {
                EditClientsData(clients, size, game, login, win, bet, a[i].jackpot);
                break;
            }
        }
    }
    else if (result[0] == result[1] || result[0] == result[2] || result[1] == result[2]) {
        win = true;
        for (int i = 0; i < 5; i++) {
            if (a[i].name == result[0] || a[i].name == result[1] || a[i].name == result[2]) {
                EditClientsData(clients, size, game, login, win, bet, a[i].twoMatch);
                break;
            }
        }
    }
    else {
        win = false;
        EditClientsData(clients, size, game, login, win, bet, 1.0);
    }
}
int hand_value(string* cards, int cardsCount) {
    int sum = 0;
    int aces = 0;
    for (int i = 0; i < cardsCount; ++i) {
        string& c = cards[i];
        if (c == "J" || c == "Q" || c == "K") sum += 10;
        else if (c == "A") ++aces;
        else sum += stoi(c);
    }
    sum += aces * 11;
    while (sum > 21 && aces > 0) {
        sum -= 10;
        --aces;
    }
    return sum;
}

void blackjac(string login) {
    string ranks[] = { "2","3","4","5","6","7","8","9","10","J","Q","K","A" };
    string deck[312];

    int index = 0;
    for (int d = 0; d < 6; d++) {
        for (int r = 0; r < 13; r++) {
            for (int i = 0; i < 4; i++) {
                deck[index++] = ranks[r];
            }
        }
    }

    int playerDeckSize = 2, dealerDeckSize = 2;
    string* hand = new string[dealerDeckSize];
    string* playerHand = new string[playerDeckSize];

    hand[0] = deck[rand() % 312];
    hand[1] = deck[rand() % 312];

    playerHand[0] = deck[rand() % 312];
    playerHand[1] = deck[rand() % 312];

    bool playerTurn = true, win = true;
    double bet = 0;
    double coefficient;
    int clientCount;
    Clients* clients = TempolaryDatabase(clientCount);
    while (bet < 20) {
        system("cls");
        cout << "Enter your bet (minimum 20): ";
        cin >> bet;
        for (int i = 0; i < clientCount; i++) {
            Clients& w = clients[i];
            if (w.login == login) {
                if (bet > w.balance) {
                    cout << "Not enough funds! Your balance: " << w.balance << "\n";
                    bet = 0;
                    this_thread::sleep_for(chrono::milliseconds(1400));
                }
                break;
            }
        }
    }
    coefficient = bet;
    while (true) {
        system("cls");
        cout << "==============================\n"
            << "|       \033[36mBLACKJACK\033[0m         |\n"
            << "==============================\n"
            << "| Dealer: [hidden] ";
        for (int i = 1; i < dealerDeckSize; i++) cout << hand[i] << " ";
        cout << "\n------------------------------\n"
            << "| Player: ";
        for (int i = 0; i < playerDeckSize; i++) cout << playerHand[i] << " ";
        cout << "\n==============================\n";
        string user;
        if (playerTurn) {
            cout << "Choose your action:\n"
                << "[1] Hit\n"
                << "[2] Stand\n"
                << "------------------------------\n";
            cin >> user;
            if (user == "1") {
                string* newPlayerHand = new string[playerDeckSize + 1];
                for (int i = 0; i < playerDeckSize; i++) newPlayerHand[i] = playerHand[i];
                delete[] playerHand;
                playerHand = newPlayerHand;
                playerHand[playerDeckSize] = deck[rand() % 312];
                playerDeckSize++;
            }
            else if (user == "2") {
                playerTurn = false;
            }
            else continue;
        }
        int checkP = hand_value(playerHand, playerDeckSize);
        if (checkP > 21) {
            cout << "==============================\n"
                << "|        \033[31mYOU BUST!\033[0m        |\n"
                << "==============================\n";
            this_thread::sleep_for(chrono::seconds(2));
            playerTurn = false;
        }
        if (!playerTurn) {
            int checkD = hand_value(hand, dealerDeckSize);
            while (checkD < 17) {
                string* newHand = new string[dealerDeckSize + 1];
                for (int i = 0; i < dealerDeckSize; i++) newHand[i] = hand[i];
                delete[] hand;
                hand = newHand;
                hand[dealerDeckSize] = deck[rand() % 312];
                dealerDeckSize++;
                checkD = hand_value(hand, dealerDeckSize);
            }
            this_thread::sleep_for(chrono::seconds(1));
            cout << "Dealer's cards: ";
            for (int i = 0; i < dealerDeckSize; i++) cout << hand[i] << " ";
            cout << "\nYour cards: ";
            for (int i = 0; i < playerDeckSize; i++) cout << playerHand[i] << " ";
            cout << "\n==============================\n";
            int checkDTotal = hand_value(hand, dealerDeckSize);
            bool playerBlackjack = (playerDeckSize == 2 && checkP == 21);
            bool dealerBlackjack = (dealerDeckSize == 2 && checkDTotal == 21);

            if (playerBlackjack && !dealerBlackjack) {
                bet = coefficient * 1.5;
                win = true;
                cout << "YOU WIN (Blackjack)!\n";
                cout << "You won " << (coefficient + bet) << " chips!\n";
            }
            else if (!playerBlackjack && checkP <= 21 && (checkDTotal > 21 || checkP > checkDTotal)) {
                bet = coefficient;
                win = true;
                cout << "YOU WIN!\n";
                cout << "You won " << (coefficient * 2) << " chips!\n";
            }
            else if (checkP > 21 || (!playerBlackjack && checkP < checkDTotal && checkDTotal <= 21)) {
                win = false;
                bet = 0;
                cout << "YOU LOSE!\n";
                cout << "You lost " << coefficient << " chips.\n";
            }
            else {
                bet = 0;
                win = false;
                cout << "DRAW!\n";
                cout << "Your bet is returned: " << coefficient << " chips.\n";
            }
            string game = "blackjac";
            EditClientsData(clients, clientCount, game, login, win, bet, coefficient);
            this_thread::sleep_for(chrono::seconds(2));
            break;
        }
    }
    delete[] playerHand;
    delete[] hand;
    string playAgain;
    cout << "\nDo you want to play again? (y/n): ";
    cin >> playAgain;
    if (playAgain == "y" || playAgain == "Y") blackjac(login);
}


void slots(string login) {
    const int reels = 3, size = 5;
    const string symbols[size]{ "@", "$", "&", "*", "7" };

    string result[reels];
    Coefficient coefficient[size]{
    {"@",  9.5, 0.8},
    {"$", 10.5, 1.0},
    {"&", 12.0, 1.2},
    {"*", 10.0, 0.9},
    {"7", 10.5, 1.1}
    };
    int clientCount;
    Clients* clients = TempolaryDatabase(clientCount);
    int bet = 100;
    CheckingMoney(clients, login, clientCount, bet);
    for (int i = 0; i < 7; i++) {
        this_thread::sleep_for(chrono::milliseconds(300));
        system("cls");
        for (int j = 0; j < reels; j++) {
            int r = rand() % size;
            result[j] = symbols[r];
        }
        cout << "=====================" << endl
            << "|   \033[36mSLOT MACHINE\033[0m    |" << endl
            << "=====================" << endl
            << "| |\033[35m " << result[0] << " \033[0m| |\033[35m "
            << result[1] << " \033[0m| |\033[35m " << result[2]
            << " \033[0m| |";
        cout << "\n=====================" << endl;
    }
    if (result[0] == result[1] && result[1] == result[2])
        cout << "|\033[32m     JACKPOT!      \033[0m|" << endl;
    else if (result[0] == result[1] || result[0] == result[2] || result[1] == result[2])
        cout << "|\033[32m     YOU WIN!      \033[0m|" << endl;
    else
        cout << "|\033[31m Better luck next  \033[0m|" << endl;
    cout << "=====================" << endl;
    coefficientCalc(coefficient, result, bet, clients, clientCount, login);
    int player = 0;
    for (int i = 0; i < clientCount; i++) {
        Clients& w = clients[i];
        if (login == w.login) {
            player = i;
            break;
        }
    }
    cout << "Balance updated successfully.\n";
    cout << "Your current balance: \033[33m"
        << clients[player].balance
        << " credits\033[0m\n";
    cout << "---------------------\n";
    cout << "Do you want to play again?\n";
    cout << "[1] Yes, spin again\n";
    cout << "[2] No, return to menu\n";
    cout << "---------------------\n";
    string restart;
    restart = _getch();
    if (restart == "1") {
        slots(login);
    }
}
void displaySimonGrid(int& score, string login) {
    system("cls");
    score = 0;
    int size = 0;
    int* arr = nullptr;
    string* arr3 = nullptr;
    bool playing = true;

    int clientCount, bet = 100;
    Clients* clients = TempolaryDatabase(clientCount);
    CheckingMoney(clients, login, clientCount, bet);

    while (playing) {
        string* newArr3 = new string[size + 1];
        int* arr2 = new int[size + 1];
        for (int i = 0; i < size; i++) {
            arr2[i] = arr[i];
            newArr3[i] = arr3[i];
        }
        arr2[size] = rand() % 9 + 1;
        newArr3[size] = to_string(arr2[size]);
        delete[] arr;
        delete[] arr3;
        arr = arr2;
        arr3 = newArr3;
        size++;
        for (int i = 0; i < size; i++) {
            string nums[9]{ "1","2","3","4","5","6","7","8","9" };
            int r = arr[i] - 1;
            system("cls");
            cout << "=========================\n" << "|   \033[31m"
                << nums[0] << "\033[0m   |   \033[32m"
                << nums[1] << "\033[0m   |   \033[33m"
                << nums[2] << "\033[0m   |\n" << "-------------------------\n" << "|   \033[34m"
                << nums[3] << "\033[0m   |   \033[35m"
                << nums[4] << "\033[0m   |   \033[36m"
                << nums[5] << "\033[0m   |\n" << "-------------------------\n" << "|   \033[31m"
                << nums[6] << "\033[0m   |   \033[32m"
                << nums[7] << "\033[0m   |   \033[33m"
                << nums[8] << "\033[0m   |\n"
                << "=========================\n";
            nums[r] = "*";
            this_thread::sleep_for(chrono::milliseconds(400));
            system("cls");
            cout << "=========================\n" << "|   \033[31m"
                << nums[0] << "\033[0m   |   \033[32m"
                << nums[1] << "\033[0m   |   \033[33m"
                << nums[2] << "\033[0m   |\n" << "-------------------------\n" << "|   \033[34m"
                << nums[3] << "\033[0m   |   \033[35m"
                << nums[4] << "\033[0m   |   \033[36m"
                << nums[5] << "\033[0m   |\n" << "-------------------------\n" << "|   \033[31m"
                << nums[6] << "\033[0m   |   \033[32m"
                << nums[7] << "\033[0m   |   \033[33m"
                << nums[8] << "\033[0m   |\n"
                << "=========================\n";
            this_thread::sleep_for(chrono::milliseconds(400));
            nums[r] = to_string(r + 1);
            system("cls");
            cout << "=========================\n" << "|   \033[31m"
                << nums[0] << "\033[0m   |   \033[32m"
                << nums[1] << "\033[0m   |   \033[33m"
                << nums[2] << "\033[0m   |\n" << "-------------------------\n" << "|   \033[34m"
                << nums[3] << "\033[0m   |   \033[35m"
                << nums[4] << "\033[0m   |   \033[36m"
                << nums[5] << "\033[0m   |\n" << "-------------------------\n" << "|   \033[31m"
                << nums[6] << "\033[0m   |   \033[32m"
                << nums[7] << "\033[0m   |   \033[33m"
                << nums[8] << "\033[0m   |\n"
                << "=========================\n";
        }
        cout << "=========================\n";
        cout << " Now repeat the sequence \n";
        cout << "=========================\n";

        string userInput;
        while (userInput.size() < size) {
            string temp;
            getline(cin, temp);
            temp.erase(remove(temp.begin(), temp.end(), ' '), temp.end());
            userInput += temp;
        }
        bool correct = true;
        if (userInput.size() != size) {
            correct = false;
            break;
        }
        for (int i = 0; i < size; i++) {
            if (userInput[i] != arr3[i][0]) {
                correct = false;
                break;
            }
        }
        if (!correct) playing = false;
        else {
            score++;
            cout << "\nCorrect! Next round...\n";
            this_thread::sleep_for(chrono::milliseconds(1000));
        }
    }
    system("cls");
    cout << "=========================\n";
    cout << "        Game Over        \n";
    cout << "Your final score: " << score << endl;
    cout << "=========================\n";

    int player = 0;
    for (int i = 0; i < clientCount; i++) {
        Clients& w = clients[i];
        if (login == w.login) {
            player = i;
            break;
        }
    }
    string game{ "simon" };
    bool a = true;
    double coefficient = score;
    EditClientsData(clients, clientCount, game, login, a, bet, coefficient);
    delete[] clients;
    delete[] arr;
    delete[] arr3;
    this_thread::sleep_for(chrono::seconds(3));
}
void simon(string login) {
    cin.ignore();
    int user, score = 0;
    do
    {
        system("cls");
        cout << "===============================\n"
            << "|       \033[36mSIMON SAYS GAME\033[0m       |\n"
            << "===============================\n"
            << "Follow the sequence shown on screen!\n"
            << "Press the corresponding keys when prompted.\n"
            << "-------------------------------\n"
            << "Current Score: \033[33m" << score << "\033[0m\n"
            << "-------------------------------\n"
            << "[1] Start Game\n"
            << "[2] Return to Main Menu\n"
            << "===============================\n";
        cin >> user;
        if (user == 1) displaySimonGrid(score, login);
        else if (user == 2) break;
    } while (true);
}
void CasinoGamesMenu(string login) {
    int user;
    cin.ignore();
    do
    {
        system("cls");
        cout << "====================================\n"
            << "|         \033[36mCASINO GAME MENU\033[0m         |\n"
            << "====================================\n"
            << "Which game would you like to play? |\n"
            << "------------------------------------\n"
            << "[1] \033[35mBlackjack\033[0m\n"
            << "[2] \033[33mSlots\033[0m\n"
            << "[3] \033[32mSimon Says\033[0m\n"
            << "[4] \033[34mStatistics\033[0m\n"
            << "[5] Exit\n"
            << "------------------------------------\n"
            << "Enter the number of your choice: ";
        cin >> user;
        int a = 0, clientCount;
        Clients* clients = TempolaryDatabase(clientCount);
        if (user == 1) blackjac(login);
        else if (user == 2) slots(login);
        else if (user == 3) simon(login);
        else if (user == 4) userStatistics(clients, clientCount, login);
        else if (user == 5) break;
        else {
            cout << "\033[31mInvalid input. Please enter a number from 1 to 4.\033[0m\n";
            this_thread::sleep_for(chrono::seconds(2));
        }
    } while (true);
}