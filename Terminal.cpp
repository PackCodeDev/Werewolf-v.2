#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <random>

enum Role { WEREWOLF, VILLAGER, SEER };

struct Player {
    std::string name;
    Role role;
    bool alive;
};

void assignRoles(std::vector<Player>& players) {
    std::srand(std::time(0));
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

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(players.begin(), players.end(), g);
}

void displayRoles(const std::vector<Player>& players) {
    for (const auto& player : players) {
        std::cout << player.name << " is a ";
        switch (player.role) {
            case WEREWOLF: std::cout << "Werewolf"; break;
            case VILLAGER: std::cout << "Villager"; break;
            case SEER: std::cout << "Seer"; break;
        }
        std::cout << std::endl;
    }
}

bool gameOver(const std::vector<Player>& players) {
    int werewolves = 0, villagers = 0;
    for (const auto& player : players) {
        if (player.alive) {
            if (player.role == WEREWOLF) ++werewolves;
            else ++villagers;
        }
    }
    return werewolves == 0 || werewolves >= villagers;
}

void nightPhase(std::vector<Player>& players) {
    std::cout << "Night falls. Werewolves, choose your victim." << std::endl;

    for (auto& player : players) {
        if (player.alive && player.role == WEREWOLF) {
            std::string targetName;
            std::cout << player.name << ", choose a player to kill: ";
            std::cin >> targetName;

            bool foundTarget = false;
            for (auto& p : players) {
                if (p.name == targetName && p.alive) {
                    foundTarget = true;
                    p.alive = false;
                    std::cout << p.name << " was killed by the Werewolves." << std::endl;
                    break;
                }
            }

            if (!foundTarget) {
                std::cout << "Player not found or player is already dead." << std::endl;
            }
            break;
        }
    }

    for (auto& player : players) {
        if (player.alive && player.role == SEER) {
            std::cout << player.name << ", you are the Seer. Choose a player to inspect." << std::endl;
            std::string targetName;
            std::cin >> targetName;

            bool foundTarget = false;
            for (const auto& p : players) {
                if (p.name == targetName && p.alive) {
                    foundTarget = true;
                    std::cout << "The role of " << p.name << " is: ";
                    switch (p.role) {
                        case WEREWOLF: std::cout << "Werewolf"; break;
                        case VILLAGER: std::cout << "Villager"; break;
                        case SEER: std::cout << "Seer"; break;
                    }
                    std::cout << std::endl;
                    break;
                }
            }
            if (!foundTarget) {
                std::cout << "Player not found or player is dead." << std::endl;
            }
        }
    }
}

void dayPhase(std::vector<Player>& players) {
    std::cout << "Day breaks. Villagers, choose someone to lynch." << std::endl;

    std::string targetName;
    std::cout << "Enter the name of the player to lynch: ";
    std::cin >> targetName;

    bool foundTarget = false;
    for (auto& player : players) {
        if (player.name == targetName && player.alive) {
            foundTarget = true;
            player.alive = false;
            std::cout << player.name << " was lynched by the Villagers." << std::endl;
            break;
        }
    }

    if (!foundTarget) {
        std::cout << "Player not found or player is already dead." << std::endl;
    }
}

void displayWinner(const std::vector<Player>& players) {
    int werewolves = 0, villagers = 0;
    for (const auto& player : players) {
        if (player.alive) {
            if (player.role == WEREWOLF) ++werewolves;
            else ++villagers;
        }
    }

    if (werewolves == 0) {
        std::cout << "Villagers win!" << std::endl;
    } else if (werewolves >= villagers) {
        std::cout << "Werewolves win!" << std::endl;
    }
}

int main() {
    int numPlayers;
    std::cout << "Enter the number of players: ";
    std::cin >> numPlayers;

    if (numPlayers < 4) {
        std::cout << "At least 4 players are required to play the game." << std::endl;
        return 1;
    }

    std::vector<Player> players;
    for (int i = 0; i < numPlayers; ++i) {
        std::string name;
        std::cout << "Enter name for player " << i + 1 << ": ";
        std::cin >> name;
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

    std::cout << "Game over!" << std::endl;
    return 0;
}
