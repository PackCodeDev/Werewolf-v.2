#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <random>

using namespace std;

enum Role { WEREWOLF, VILLAGER, SEER };

struct Player {
    string name;
    Role role;
    bool alive;
};

void assignRoles(vector<Player>& players) {
    srand(time(0));
    int numPlayers = players.size();
    int numWerewolves = numPlayers / 3;
    int numSeers = 1;

    for (int i = 0; i < numWerewolves; ++i) {
        players[i].role = WEREWOLF;
    }
    for (int i = numWerewolves; i < numWerewolves + numSeers; ++i) {
        players[i].role = SEER;
    }
    for (int i = numWerewolves + numSeers; i < numPlayers; ++i) {
        players[i].role = VILLAGER;
    }

    random_device rd;
    mt19937 g(rd());
    shuffle(players.begin(), players.end(), g);
}

void displayRoles(const vector<Player>& players) {
    for (const auto& player : players) {
        cout << player.name << " is a ";
        switch (player.role) {
            case WEREWOLF: cout << "Werewolf"; break;
            case VILLAGER: cout << "Villager"; break;
            case SEER: cout << "Seer"; break;
        }
        cout << endl;
    }
}

bool gameOver(const vector<Player>& players) {
    int werewolves = 0, villagers = 0;
    for (const auto& player : players) {
        if (player.alive) {
            if (player.role == WEREWOLF) ++werewolves;
            else ++villagers;
        }
    }
    return werewolves == 0 || werewolves >= villagers;
}

void nightPhase(vector<Player>& players) {
    cout << "Night falls. Werewolves, choose your victim." << endl;

    for (auto& player : players) {
        if (player.alive && player.role == WEREWOLF) {
            string targetName;
            cout << player.name << ", choose a player to kill: ";
            cin >> targetName;

            bool foundTarget = false;
            for (auto& p : players) {
                if (p.name == targetName && p.alive) {
                    foundTarget = true;
                    p.alive = false;
                    cout << p.name << " was killed by the Werewolves." << endl;
                    break;
                }
            }

            if (!foundTarget) {
                cout << "Player not found or player is already dead." << endl;
            }
            break;
        }
    }

    for (auto& player : players) {
        if (player.alive && player.role == SEER) {
            cout << player.name << ", you are the Seer. Choose a player to inspect." << endl;
            string targetName;
            cin >> targetName;

            bool foundTarget = false;
            for (const auto& p : players) {
                if (p.name == targetName && p.alive) {
                    foundTarget = true;
                    cout << "The role of " << p.name << " is: ";
                    switch (p.role) {
                        case WEREWOLF: cout << "Werewolf"; break;
                        case VILLAGER: cout << "Villager"; break;
                        case SEER: cout << "Seer"; break;
                    }
                    cout << endl;
                    break;
                }
            }
            if (!foundTarget) {
                cout << "Player not found or player is dead." << endl;
            }
        }
    }
}

void dayPhase(vector<Player>& players) {
    cout << "Day breaks. Villagers, choose someone to lynch." << endl;

    string targetName;
    cout << "Enter the name of the player to lynch: ";
    cin >> targetName;

    bool foundTarget = false;
    for (auto& player : players) {
        if (player.name == targetName && player.alive) {
            foundTarget = true;
            player.alive = false;
            cout << player.name << " was lynched by the Villagers." << endl;
            break;
        }
    }

    if (!foundTarget) {
        cout << "Player not found or player is already dead." << endl;
    }
}

void displayWinner(const vector<Player>& players) {
    int werewolves = 0, villagers = 0;
    for (const auto& player : players) {
        if (player.alive) {
            if (player.role == WEREWOLF) ++werewolves;
            else ++villagers;
        }
    }

    if (werewolves == 0) {
        cout << "Villagers win!" << endl;
    } else if (werewolves >= villagers) {
        cout << "Werewolves win!" << endl;
    }
}

int main() {
    int numPlayers;
    cout << "Enter the number of players: ";
    cin >> numPlayers;

    if (numPlayers < 4) {
        cout << "At least 4 players are required to play the game." << endl;
        return 1;
    }

    vector<Player> players;
    for (int i = 0; i < numPlayers; ++i) {
        string name;
        cout << "Enter name for player " << i + 1 << ": ";
        cin >> name;
        players.push_back({name, VILLAGER, true});
    }

    assignRoles(players);
    displayRoles(players);

    while (!gameOver(players)) {
        nightPhase(players);
        if (gameOver(players)) break;
        dayPhase(players);
    }

    displayWinner(players);

    cout << "Game over!" << endl;
    return 0;
}

