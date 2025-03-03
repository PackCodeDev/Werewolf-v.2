#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <random>
#include <windows.h>
#include <thread>  // For sleep function
#include <chrono>  // For time duration

using namespace std;

enum Role { WEREWOLF, VILLAGER, SEER, BODYGUARD, BEGGAR, SPELLCASTER, HUNTER };

struct Player {
    string name;
    Role role;
    bool alive;
    bool protectedThisNight;
    bool muted;
};

void assignRoles(vector<Player>& players) {
    srand(time(0));
    int numPlayers = players.size();
    int numWerewolves = numPlayers / 3;
    int numSeers = 1;
    int numBodyguards = 1;
    int numBeggars = 1;
    int numSpellcasters = 1;
    int numHunters = 1;

    for (int i = 0; i < numWerewolves; ++i) players[i].role = WEREWOLF;
    for (int i = numWerewolves; i < numWerewolves + numSeers; ++i) players[i].role = SEER;
    for (int i = numWerewolves + numSeers; i < numWerewolves + numSeers + numBodyguards; ++i) players[i].role = BODYGUARD;
    for (int i = numWerewolves + numSeers + numBodyguards; i < numWerewolves + numSeers + numBodyguards + numBeggars; ++i) players[i].role = BEGGAR;
    for (int i = numWerewolves + numSeers + numBodyguards + numBeggars; i < numWerewolves + numSeers + numBodyguards + numBeggars + numSpellcasters; ++i) players[i].role = SPELLCASTER;
    for (int i = numWerewolves + numSeers + numBodyguards + numBeggars + numSpellcasters; i < numWerewolves + numSeers + numBodyguards + numBeggars + numSpellcasters + numHunters; ++i) players[i].role = HUNTER;
    for (int i = numWerewolves + numSeers + numBodyguards + numBeggars + numSpellcasters + numHunters; i < numPlayers; ++i) players[i].role = VILLAGER;

    random_device rd;
    mt19937 g(rd());
    shuffle(players.begin(), players.end(), g);
}

void displayRoles(vector<Player>& players) {
    cout << "\n\033[35m Each player will see their role privately. Press Enter to continue.\033[0m\n";

    for (auto& player : players) {
        cout << "\n" << player.name << "\033[35m , it's your turn to see your role.\033[0m" << endl;
        cout << "\033[35m Press Enter when you're ready to see your role...\033[0m";
        
        // Wait for Enter
        cin.ignore();  
        cin.get();     

        // Show player's role
        cout << "\n\033[35m Your role is: \033[0m";
        switch (player.role) {
            case WEREWOLF: cout << "Werewolf"; break;
            case VILLAGER: cout << "Villager"; break;
            case SEER: cout << "Seer"; break;
            case BODYGUARD: cout << "Bodyguard"; break;
            case BEGGAR: cout << "Beggar"; break;
            case SPELLCASTER: cout << "Spellcaster"; break;
            case HUNTER: cout << "Hunter"; break;
        }
        cout << "\n\033[35m Remember your role! Press Enter to continue to the next player...\033[0m";
        
        // Wait for Enter
        cin.ignore();  
        cin.get();     

        // Clear screen
        #ifdef _WIN32
            system("CLS");
        #else
            system("clear");
        #endif
    }

    cout << "\n\033[35m All roles have been assigned. The game begins now!\033[0m" << endl;
    this_thread::sleep_for(chrono::seconds(2)); // Wait for players to prepare before starting
}
void hunterRevenge(vector<Player>& players) {
    string target;
    cout << R"( 
        ...
      ;::::;
    ;::::; :;
  ;:::::'   :;
 ;:::::;     ;.
,:::::'       ;           OOO\
::::::;       ;          OOOOO\
;:::::;       ;         OOOOOOOO
,;::::::;     ;'         / OOOOOOO
;:::::::::`. ,,,;.        /  / DOOOOOO
.';:::::::::::::::::;,     /  /     DOOOO
,::::::;::::::;;;;::::;,   /  /        DOOO
;`::::::`'::::::;;;::::: ,#/  /          DOOO
:`:::::::`;::::::;;::: ;::#  /            DOOO
::`:::::::`;:::::::: ;::::# /              DOO
`:`:::::::`;:::::: ;::::::#/               DOO
:::`:::::::`;; ;:::::::::##                OO
::::`:::::::`;::::::::;:::#                OO
`:::::`::::::::::::;'`:;::#                O
`:::::`::::::::;' /  / `:#
::::::`:::::;'  /  /   `#
)" << endl;
    cout << "\033[34m The Hunter has died! Choose someone to take down with you: \033[0m";
    cin >> target;

    for (auto& player : players) {
        if (player.name == target && player.alive) {
            player.alive = false;
            cout << player.name << " \033[31m was shot by the Hunter!\033[0m" << endl;
            return;
        }
    }
    cout << "\033[31m Player not found or already dead. No one was shot.\033[0m" << endl;
}

void nightPhase(vector<Player>& players) {

    Beep(500, 300);  // เสียง Beep หลังจากหน่วงเวลา
     cout << R"( 
                                                    
                      ::---====                
                        ::----=====            
                         :::------===          
                          :::-------===        
                           ::--------====      
                            ::--------====     
                             ::--------====    
                             :::---------===   
                              ::----=*=---===  
                              ::--=-.:@@--===  
                              ::--+. -@@+-===  
                              ::---%@@@#--===  
                              :-------=---===  
                              -----======--==  
                             :-----===----===  
                            ::-----------===   
                           +---*%+------===    
                         :::-----------===     
                      ::::-----------====      
      =---:::::::::::::------------=====       
        ==----------------------=====          
          ========----------=======            
              =================                
                                               
   )" << endl;
    cout << "\nNight falls. Werewolves, choose your victim." << endl;

    string werewolfTarget;
    bool victimFound = false;

    // Werewolf selects victim
    for (auto& player : players) {
        if (player.alive && player.role == WEREWOLF) {
            cout << R"(             
                __
                                       .d$$b
                                     .' TO$;\
                                    /  : TP._;
                                   / _.;  :Tb|
                                  /   /   ;j$j
                              _.-"       d$$$$
                            .' ..       d$$$$;
                           /  /P'      d$$$$P. |\
                          /   "      .d$$$P' |\^"l
                        .'           `T$P^"""""  :
                    ._.'      _.'                ;
                 `-.-".-'-' ._.       _.-"    .-"
               `.-" _____  ._              .-"
              -(.g$$$$$$$b.              .'
                ""^^T$$$P^)            .(:
                  _/  -"  /.'         /:/;
               ._.'-'`-'  ")/         /;/;
            `-.-"..--""   " /         /  ;
           .-" ..--""        -'          :
           ..--""--.-"         (\      .-(\
             ..--""              `-\(\/;`
               _.                      :
                                       ;`-
                                      :\
                                      ;                      
                                                                                       
              )" << endl;
            cout << "\033[31mWerewolf, choose a player to kill: \033[0m";
            cin >> werewolfTarget;

            // Wait for Enter to hide the action
            cout << "Press Enter to hide...";
            cin.ignore();  // Clear buffer
            cin.get();     // Wait for Enter
            
            this_thread::sleep_for(chrono::seconds(20)); // หน่วงเวลา 20วินาที
            Beep(500, 300);  // เสียง Beep หลังจากหน่วงเวลา

            // Clear the screen after selection
            #ifdef _WIN32
                system("CLS");  // For Windows
            #else
                system("clear");  // For Linux/macOS
            #endif

            // Find the target
            for (auto& p : players) {
                if (p.name == werewolfTarget && p.alive) {
                    victimFound = true;
                    break;
                }
            }
            if (victimFound) break;
        }
    }

    // Seer checks a player
    for (auto& player : players) {
        if (player.alive && player.role == SEER) {
            string seerTarget;
            cout << R"(   
              
            _____-------_____
         ___-----      00000      -----___
      ---           00000 00000           ---
   ---            000000   000000            ---
_---              000000     000000              ---_
-___              000000     000000              ___-
   ---            000000   000000            ---
      ---           00000 00000           ---
         ---_____      00000      _____---
                 -----_______-----      
                                                        
)" << endl;
            cout << "\033[32mSeer, choose a player to inspect: \033[0m";
            cin >> seerTarget;
            for (auto& p : players) {
                if (p.name == seerTarget && p.alive) {
                    cout << "The role of " << seerTarget << " is: ";
                    // Check if the player is a Werewolf and respond accordingly
                if (p.role == WEREWOLF) {
                    cout << "Yes" << endl;
                } else {
                    cout << "No" << endl;
                }
                    cout << endl;
                    break;
                }
            }

            // Wait for Enter to hide the action
            cout << "Press Enter to hide...";
            cin.ignore();  // Clear buffer
            cin.get();     // Wait for Enter
            
            this_thread::sleep_for(chrono::seconds(20)); // หน่วงเวลา 20วินาที
            Beep(500, 300);  // เสียง Beep หลังจากหน่วงเวลา
            // Clear the screen after selection
            #ifdef _WIN32
                system("CLS");  // For Windows
            #else
                system("clear");  // For Linux/macOS
            #endif

            // Check player's role
            
            break;
        }
    }

    // Bodyguard protection
    for (auto& player : players) {
        if (player.alive && player.role == BODYGUARD) {
            string bodyguardProtection;
            cout << R"(   


            ________________
            \      __      /         __
             \_____()_____/         /  )
             '============`        /  /
              #---\  /---#        /  /
             (# @\| |/@  #)      /  /
              \   (_)   /       /  /
              |\ '---` /|      /  /
        _______/ \\_____// \____/ o_|
        /       \  /     \  /   / o_|
        / |           o|        / o_| \
        /  |  _____     |       / /   \ \
        /   |  |===|    o|      / /\    \ \
        |    |   \@/      |     / /  \    \ \
        |    |___________o|__/----)   \    \/
        |    '              ||  --)    \     |
        |___________________||  --)     \    /
        |           o|   ''''   |   \__/
        |            |          |
        
        
            )" << endl;
            cout << "\033[34mBodyguard, choose a player to protect: \033[0m";
            cin >> bodyguardProtection;
            for (auto& p : players) {
                if (p.name == bodyguardProtection && p.alive) {
                    p.protectedThisNight = true;
                    cout << p.name << " is being protected by the Bodyguard." << endl;
                    break;
                }
            }

            // Wait for Enter to hide the action
            cout << "Press Enter to hide...";
            cin.ignore();  // Clear buffer
            cin.get();     // Wait for Enter
            
            this_thread::sleep_for(chrono::seconds(20)); // หน่วงเวลา 20วินาที
            Beep(500, 300);  // เสียง Beep หลังจากหน่วงเวลา
            // Clear the screen after selection
            #ifdef _WIN32
                system("CLS");  // For Windows
            #else
                system("clear");  // For Linux/macOS
            #endif

            // Protect the player
           
            break;
        }
    }

    // Spellcaster silences
    for (auto& player : players) {
        if (player.alive && player.role == SPELLCASTER) {
            string spellTarget;
            cout << R"(   
                       
                
            uuuuuuuuuuuuuuuuuuuu
          u" uuuuuuuuuuuuuuuuuu "u
        u" u$$$$$$$$$$$$$$$$$$$$u "u
      u" u$$$$$$$$$$$$$$$$$$$$$$$$u "u
    u" u$$$$$$$$$$$$$$$$$$$$$$$$$$$$u "u
  u" u$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$u "u
u" u$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$u "u
$ $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ $
$ $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ $
$ $$$" ... "$...  ...$" ... "$$$  ... "$$$ $
$ $$$u `"$$$$$$$  $$$  $$$$$  $$  $$$  $$$ $
$ $$$$$$uu "$$$$  $$$  $$$$$  $$  """ u$$$ $
$ $$$""$$$  $$$$  $$$u "$$$" u$$  $$$$$$$$ $
$ $$$$....,$$$$$..$$$$$....,$$$$..$$$$$$$$ $
$ $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ $
"u "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" u"
  "u "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" u"
    "u "$$$$$$$$$$$$$$$$$$$$$$$$$$$$" u"
      "u "$$$$$$$$$$$$$$$$$$$$$$$$" u"
        "u "$$$$$$$$$$$$$$$$$$$$" u"
          "u """""""""""""""""" u"
            """"""""""""""""""""

    )" << endl;
            cout << "\033[34mSpellcaster, choose a player to silence: \033[0m";
            cin >> spellTarget;

            // Wait for Enter to hide the action
            cout << "Press Enter to hide...";
            cin.ignore();  // Clear buffer
            cin.get();     // Wait for Enter
            
            this_thread::sleep_for(chrono::seconds(20)); // หน่วงเวลา 20วินาที
            Beep(500, 300);  // เสียง Beep หลังจากหน่วงเวลา
            // Clear the screen after selection
            #ifdef _WIN32
                system("CLS");  // For Windows
            #else
                system("clear");  // For Linux/macOS
            #endif

            // Silence the player
            for (auto& p : players) {
                if (p.name == spellTarget && p.alive) {
                    p.muted = true;
                    cout << p.name << " has been silenced and cannot speak next round!" << endl;
                    break;
                }
            }
            break;
        }
    }

    // Process Werewolf's attack
    if (victimFound) {
        for (auto& p : players) {
            if (p.name == werewolfTarget && p.alive) {
                if (p.protectedThisNight) {
                    cout << p.name << "\033[32m was protected and survived the attack!\033[0m " << endl;
                } else {
                    p.alive = false;
                    cout << p.name << "\033[31m was killed by the Werewolves.\033[0m" << endl;

                    if (p.role == HUNTER) {
                        hunterRevenge(players);
                    }
                }
                break;
            }
        }
    } else {
        cout << "\033[33mNo valid target was chosen by the Werewolves.\033[0m" << endl;
    }
}


void dayPhase(vector<Player>& players) {
    Beep(500, 1000);  // เสียง Beep
      cout << R"( 
           .     :     .
            .  :    |    :  .
             .  |   |   |  ,
              \  |     |  /
          .     ,-'"""`-.     .
            "- /  __ __  \ -"
              |==|  I  |==|
        - --- | _`--^--'_ | --- -
              |'`.     ,'`|
            _- \  "---"  / -_
          .     `-.___,-'     .
              /  |     |  \
            .'  |   |   |  `.
               :    |    :
              .     :     .

   )" << endl;
    cout << "\n\033[36mDay breaks. Villagers, choose someone to lynch.\033[0m" << endl;
    string targetName;
    cout << "\033[31mEnter the name of the player to lynch: \033[0m";
    cin >> targetName;

    for (auto& player : players) {
        if (player.name == targetName && player.alive) {
            player.alive = false;
            cout << player.name << "\033[31m was lynched by the Villagers.\033[0m" << endl;
             // Check if the lynched player is a Beggar
             if (player.role == BEGGAR) {
                cout << R"( 
                    _.,-----/=\-----,._
                     (__ ~~~"""""""~~~ __)
                      | ~~~"""""""""~~~ |
                      | |  ; ,   , ;  | |
                      | |  | |   | |  | |
                      | |  | |   | |  | |
                      | |  | |   | |  | |
                      | |  | |   | |  | |
                      | |  | |   | |  | |
                      | |  | |   | |  | |
                      | |  | |   | |  | |
                      |. \_| |   | |_/ .|
                       `-,.__ ~~~ __.,-'
                             ~~~~~
                       )" << endl;
                cout << "The Beggar has been lynched... but the Beggar wins!" << endl;
                exit(0);  // End the game as Beggar wins
            }
        
            return;
        }
    }
    cout << "\033[31mPlayer not found or already dead.\033[0m" << endl;
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

void displayWinner(const vector<Player>& players) {
    int werewolves = 0, villagers = 0;
    for (const auto& player : players) {
        if (player.alive) {
            if (player.role == WEREWOLF) ++werewolves;
            else ++villagers;
        }
    }
    werewolves == 0 ? cout << "\033[32mVillagers win!\033[0m" << endl : cout << "\033[31mWerewolves win!\033[0m" << endl;
}

int main() {
    int numPlayers;
    cout << R"(   
              
    /^\      /^\
   |  \    /  |
   ||\ \../ /||
   )'        `(
  ,;`w,    ,w';,
  ;,  ) __ (  ,;
   ;  \(\/)/  ;;
  ;|  |vwwv|    ``-...
   ;  `lwwl'   ;      ```''-.
  ;| ; `""' ; ;              `.
   ;         ,   ,          , |
   '  ;      ;   l    .     | |
   ;    ,  ,    |,-,._|      \;
    ;  ; `' ;   '    \ `\     \;
    |  |    |  |     |   |    |;
    |  ;    ;  |      \   \   (;
    | |      | l       | | \  |
    | |      | |  pb   | |  ) |
    | |      | ;       | |  | |
    ; ,      : ,      ,_.'  | |
   :__'      | |           ,_.'
            `--'                                            
 )" << endl;
    cout << "\033[36mEnter the number of players: \033[0m";
    cin >> numPlayers;

    if (numPlayers < 4) {
        cout << "\033[31mAt least 4 players are required to play the game.\033[0m" << endl;
        return 1;
    }

    vector<Player> players;
    for (int i = 0; i < numPlayers; ++i) {
        string name;
        cout << "\033[36mEnter name for player \033[0m" << i + 1 << ": ";
        cin >> name;
        players.push_back({name, VILLAGER, true, false, false});
    }

    assignRoles(players);
    displayRoles(players);

    while (!gameOver(players)) {
        nightPhase(players);
        if (gameOver(players)) break;
        dayPhase(players);
    }

    displayWinner(players);
    cout << R"( 
        ,____.
                     ,-^      ^-.___
                    /            \  ^-.
                  _|          /~\ |    \
               _-^ |          (#) |0    |
              /    |          \_/ |_____|____
             /      \            /       _-^~~~\
            / \    _-`-_      _-'     _-^  \  \|
           /   \  /      ~~~~~~|   __-^ \   \   \
          |     \/            \  /   \   \  _-^^
          |     |              \/---______-^
           \     \              |########/
            \     \             |##\||///
             ^-_   \            |###|//'             '
                \_-^\           |###|/|          '
                 \   |          |###|/|       o
              _   | /           |###//|            '
             < \_/ /            |-----~~~~~~~~---____
              \___/             |\   _-^\^^--__      ^.
                               /  \_/##\\|     ^^--__-'
                              /    _=--^'      '
                    ,_____.  /  _-^
                   / ,--- _-^_-^
                   ||  _-^_-^
                   |\  _-^/|
                    \`---'/
                     `^^^'
   
      )" << endl;
    cout << "\033[36mGame over!\033[0m" << endl;
    return 0;
}
