#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <random>

using namespace std;

enum Role { WEREWOLF, VILLAGER, SEER, BODYGUARD, BEGGAR, SPELLCASTER };

struct Player {
    string name;
    Role role;
    bool alive;
    bool protectedThisNight;
    bool muted; // ถ้าถูกปิดปาก จะไม่สามารถพูดในรอบถัดไปได้
};

void assignRoles(vector<Player>& players) {
    srand(time(0));
    int numPlayers = players.size();
    int numWerewolves = numPlayers / 3;
    int numSeers = 1;
    int numBodyguards = 1;
    int numBeggars = 1;
    int numSpellcasters = 1;

    for (int i = 0; i < numWerewolves; ++i) players[i].role = WEREWOLF;
    for (int i = numWerewolves; i < numWerewolves + numSeers; ++i) players[i].role = SEER;
    for (int i = numWerewolves + numSeers; i < numWerewolves + numSeers + numBodyguards; ++i) players[i].role = BODYGUARD;
    for (int i = numWerewolves + numSeers + numBodyguards; i < numWerewolves + numSeers + numBodyguards + numBeggars; ++i) players[i].role = BEGGAR;
    for (int i = numWerewolves + numSeers + numBodyguards + numBeggars; i < numWerewolves + numSeers + numBodyguards + numBeggars + numSpellcasters; ++i) players[i].role = SPELLCASTER;
    for (int i = numWerewolves + numSeers + numBodyguards + numBeggars + numSpellcasters; i < numPlayers; ++i) players[i].role = VILLAGER;

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
            case BODYGUARD: cout << "Bodyguard"; break;
            case BEGGAR: cout << "Beggar"; break;
            case SPELLCASTER: cout << "Spellcaster"; break;
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

void beggarWins() {
    cout << "The Beggar has been lynched and wins the game!" << endl;
    exit(0);
}

void nightPhase(vector<Player>& players) {
    cout << "\nNight falls. Werewolves, choose your victim." << endl;

    // Bodyguard action
    string bodyguardProtection;
    for (auto& player : players) {
        if (player.alive && player.role == BODYGUARD) {
            cout << player.name << ", choose a player to protect: ";
            cin >> bodyguardProtection;
            for (auto& p : players) {
                if (p.name == bodyguardProtection && p.alive) {
                    p.protectedThisNight = true;
                    cout << p.name << " is being protected by the Bodyguard." << endl;
                    break;
                }
            }
            break;
        }
    }

    // Werewolf action
    string werewolfTarget;
    for (auto& player : players) {
        if (player.alive && player.role == WEREWOLF) {
            cout << player.name << ", choose a player to kill: ";
            cin >> werewolfTarget;
            for (auto& p : players) {
                if (p.name == werewolfTarget && p.alive) {
                    if (p.protectedThisNight) {
                        cout << p.name << " was protected and survived the attack!" << endl;
                        p.protectedThisNight = false;
                    } else {
                        p.alive = false;
                        cout << p.name << " was killed by the Werewolves." << endl;
                    }
                    break;
                }
            }
            break;
        }
    }

    // Spellcaster action
    string spellTarget;
    for (auto& player : players) {
        if (player.alive && player.role == SPELLCASTER) {
            cout << player.name << ", choose a player to silence for the next round: ";
            cin >> spellTarget;
            for (auto& p : players) {
                if (p.name == spellTarget && p.alive) {
                    p.muted = true;
                    cout << p.name << " has been silenced and cannot speak during the next round!" << endl;
                    break;
                }
            }
            break;
        }
    }

    // Seer action
    for (auto& player : players) {
        if (player.alive && player.role == SEER) {
            cout << player.name << ", you are the Seer. Choose a player to inspect: ";
            string targetName;
            cin >> targetName;
            for (const auto& p : players) {
                if (p.name == targetName && p.alive) {
                    cout << "The role of " << p.name << " is: ";
                    switch (p.role) {
                        case WEREWOLF: cout << "Werewolf"; break;
                        case VILLAGER: cout << "Villager"; break;
                        case SEER: cout << "Seer"; break;
                        case BODYGUARD: cout << "Villager"; break;
                        case BEGGAR: cout << "Villager"; break;
                        case SPELLCASTER: cout << "Villager"; break;
                    }
                    cout << endl;
                    break;
                }
            }
        }
    }
}

void dayPhase(vector<Player>& players) {
    cout << "\nDay breaks. Villagers, choose someone to lynch." << endl;

    string targetName;
    bool someoneMuted = false;

    // เช็คว่ามีใครโดนปิดปากจากรอบก่อนหรือไม่
    for (auto& player : players) {
        if (player.muted) {
            cout << player.name << " has been silenced and cannot speak today!" << endl;
            player.muted = false; // รีเซ็ตให้พูดได้ในรอบถัดไป
            someoneMuted = true;
        }
    }

    if (!someoneMuted) {
        cout << "Enter the name of the player to lynch: ";
        cin >> targetName;
        for (auto& player : players) {
            if (player.name == targetName && player.alive) {
                player.alive = false;
                cout << player.name << " was lynched by the Villagers." << endl;

                if (player.role == BEGGAR) {
                    beggarWins();
                }
                return;
            }
        }
        cout << "Player not found or already dead." << endl;
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
    werewolves == 0 ? cout << "Villagers win!" << endl : cout << "Werewolves win!" << endl;
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
        players.push_back({name, VILLAGER, true, false});
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



