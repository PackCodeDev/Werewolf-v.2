#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <random>
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
    cout << "\nEach player will see their role privately. Press Enter to continue.\n";

    for (auto& player : players) {
        cout << "\n" << player.name << ", it's your turn to see your role." << endl;
        cout << "Press Enter when you're ready to see your role...";
        
        // Wait for Enter
        cin.ignore();  
        cin.get();     

        // Show player's role
        cout << "Your role is: ";
        switch (player.role) {
            case WEREWOLF: cout << "Werewolf"; break;
            case VILLAGER: cout << "Villager"; break;
            case SEER: cout << "Seer"; break;
            case BODYGUARD: cout << "Bodyguard"; break;
            case BEGGAR: cout << "Beggar"; break;
            case SPELLCASTER: cout << "Spellcaster"; break;
            case HUNTER: cout << "Hunter"; break;
        }
        cout << "\nRemember your role! Press Enter to continue to the next player...";
        
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

    cout << "\nAll roles have been assigned. The game begins now!" << endl;
    this_thread::sleep_for(chrono::seconds(2)); // Wait for players to prepare before starting
}
void hunterRevenge(vector<Player>& players) {
    string target;
    cout << "The Hunter has died! Choose someone to take down with you: ";
    cin >> target;

    for (auto& player : players) {
        if (player.name == target && player.alive) {
            player.alive = false;
            cout << player.name << " was shot by the Hunter!" << endl;
            return;
        }
    }
    cout << "Player not found or already dead. No one was shot." << endl;
}


void nightPhase(vector<Player>& players) {
    cout << "\nNight falls. Werewolves, choose your victim." << endl;

    string werewolfTarget;
    bool victimFound = false;

    // Werewolf selects victim
    for (auto& player : players) {
        if (player.alive && player.role == WEREWOLF) {
            cout << R"(                                          .                .              
            **+=+=:      .  .                                 .                        ..::--
           %==-:  . .         =*###-    ..       .   .     .                            .:--
           *#=:            =@@@%=+#%+  .                             . .         .       :--
           *==+.  .       :%@@%%@%:                               .                       .-
           =.   -       %@@@@@@@#                         .                   .          . :
           -          .%@%@@%%@%.             .         .         ..          .             
           .. . :    %@@@%%@@@@# -%@@*.        .     .*+                 .                  
               :   -@@@@%%@@@@@@@#-.    .           =#%#:.          .                 .     
           --     :%@@%%%%%%#*:          .  .    .=+*++*=                    .              
              .   #@@@@%@%+            .      .#*++*=*%*:   . . .       .           .   .   
                 #@@@@@%##-              .:-=%##*+%%%##:           .                    .   
                #@@@@%%#%*:            :*@@%%%***#@#++=-        . .              .          
               =%@%%%%%##+.            =%@@@%###%%%#*##*                          .  .      
              *@@%#%%%#%*+-         .*%%%%####%%%%%%%%+                 .    .          .   
             +%@%%%%%###*+-      . +%@@%%%###*#%%#%%##               ..     .               
            .#%%%%%%####*+:   .  .#@@%#=--=****#%%#%**.                         .       .   
           .+*%%%%%%%%%#*+=#*==: ..-==---==-:..-=%%#*=.                       .  .     .   .
            :*%%%%%%%%%##*+*#+==--+----+#**+==----***=:                                     
            -#%%%@@@%%%%####%#**++*#+*+*####*=====-+++=-.            .    .                .
           :-%%@@@@@@@@%@@%%%%%###*#%%##%%%%#**##%*+**+++=-=-.                         .    
             =%%%@@@@%@@@@@@@@%%##%%%%%%@@%%%%%@@@%#+#%##*+=+*=               .             
              :=+#@%%%#%@@@@@@%%%%%%@@@@@%%@@%%%#**+==**###+-*+-    .  .        .    .      
                 .-..:=*@@@@%%%%@@@@@@@@@@@@%%###*##*+++=*%#++#*=:             . .          
            . .     .  =-==  .=%%@@@%@@@@@@@%%%%%%%%%###*=*#*=%%#+:     .                   
                .              *#%%@@@@@@@@@@@@@@%@@%%%%##*++*@@%*+..   .         .         
            .                  :=-#%@@@@%@@@@@@@@@@@@%%%%%##*+@@%%#+     .       .          
            .                     *#%@@@@@@@@@%%%%@@%%@@%%%%*+@@@%*===+=-:  ..              
               . .                --##%@@@@@@@%%%%%@%%%%%@%##%@@@@%%%#####*=                
                           .  .    .+**#%@@@@@@%%@@@%%%%@@%##%@@@@%@@%%%%%###+-           . 
           .                       .::  -@@@@@@@@@%%%@@@@%*-=%@@@@@@@@@@@@@%%###=           
                                .      .@@%@%@@@@%%%@@@@@:   =%@@@@@@@@@@@@@@@@%%%#*:       
                     . .              *@@@%%%%*=++=--..:.     %@@@@@@@@@@@@@@@@@@@@%%#*     
           :                        =@@%###*##*###**++==-:.   =+*%%%%%%@@@@@@@@@@@@@%%##*   
           =                      +@@%#%%%%%%%%%%%%##***++=..   .  :-=::::+%#%%@@@@@@@@@%#==
           *=.    .    .       . #@@%%@@@%%%%%%%%%%%%###***-                   :#%%@@@@@@@@+
           %++=  .   :          -@@@@@@@%%##%%%%%%@@%%%%##*.    .          .       %@@@@@@@+
           #+==-.  .-+  ..      %@@@@@%%%%%%@@@@@@%%%%%%%*- .         .          . +@@@@@@@%
           *+====-=#*.          @@@@@%%%@@@@@@@@@%%@@%%#*                          .%@@@@@@@
           *+++=*%%#            #@@@@%@@@@@@%@@%@@@@%%##*..       .  .      .  .  :+%+%@+%@@
           @**%@###*=-. .        #@@%@@@@@@%%%%@@@@%%#%@%                      ..--+*=#%+*@%
           @@@%###*====--:       :#@@@@@@@@@@@@@%%@%#%@@#                    :-====#=+%+=##=
           @%%@@%%#**+======-:    -%@@@@@@@@@@@%@@%%@@@%:       .          -=======*+#*==*--
           @%#%%%%##**+++++======+@@@@@@@@@@@%@@@@@@@@@%.            -========+====**+=+==--
           @@%%%@%%%#***+***#**+%@@@@@@@@@@%@@@@@@@@@@@#  .       :====+=====+=******+======
               )" << endl;
            cout << "Werewolf, choose a player to kill: ";
            cin >> werewolfTarget;

            // Wait for Enter to hide the action
            cout << "Press Enter to hide...";
            cin.ignore();  // Clear buffer
            cin.get();     // Wait for Enter
            
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
                :::::   . .----  %@@@@@@@@@@ ==:                                    
            ----     ::::-   @%%%##@%%%%##%@@@ +-                                   
          :-  @@@@@%%%@@ @@@@%%%%%%@@%%%%###%@@ +:                                  
          :-  @@@%####%%@@%%%%########%%@%###@% *=                                  
        :-- #%%%###############%%###%@%###@% *=                                                                
    -- .#%%%%%%%%%%%%@@%*+=------=+**%@@#*#@@ *-                                    
   -  %%%%#%%%%%%%%%@=:::::::::::::::::::::+@@ +-       .                           
   -= #####%%%%%%%%%@+:::::::::::::::::::::-%@ +=    --  .--.                       
    -= #%@@%%%%@%%%%%%:::-=-----:::-::::::::#@%*=  -- @@@%* ==-:                    
     -- %@%%%%%%%%%%%@#::::::::::-=:-::::=::=@@ +-:::+@=-@%%@   =-:                 
      - %@%%%%%%@@@@%=:::-%%%%%%##%=-:::%##%%@@% +=.#%*::::+%%%@  =-                
      --%@%%%@@%%%%@=-::::-++#@%%*++:::====+*%@#  :%+:::-::::*%%%@ +=               
     :- @@%*+*%%%%%%%#####%%*#++@@%#***#%%@*=#*%%%@-::::-:::::*%%@@ +-              
    -- @@+-=+=-=#%%@####%%=-#::%@##@%#@@=+%#:*=+%@=:::::::::::-%%%% *=              
    -- @%=-===++-*%%*:::%#::-*+**:+@+-#%-*+:.*=*@#:-=-:::::::::%%%%# +:             
    .:= @@*----==*=#%#:::=@+::....=@#---%%-..:##@@=:======-:::::#%%%# +-             
     -= @@#=--=++=+%%-:::-#@#**#@#+=---=##%@@@@%@--=========-::%%%%* *-             
      -=  @@@#**%%==+=::::::----::#+=----+--%@  @--===========+@%%%  +:             
        -+      @@*===::::-:::::::::==+**=:+@   %+-===========#@@%@ *-              
          --==++ @%===::::*:::::::::=***-::%@   @%--=========#@@@@  +               
               -= @@%+-:::+-:::::-=::--:::+@% +- @%:+#*=====#@@@@ *+-               
             ----=   @@*::-=:::::::::--::-%@ *=-- @@*+*==+#%+@@@ *+-                
          --     @@@@@*#=#++:::::::::::::#@  +--- %#=#@@@#+=%   +-                  
         -- @@@%##****-*+--+%*------=+*%@@  +- --@@+*#**==*@@ +-                    
        :- %@*======*=.:*-::#**#+++*#%@@  *=- :- @*==*#+=++@# =                     
        :- %@#*=====*+..-=::+*=-*+===##@% +-  :- @@+=++**+#@  =                     
       :- @@#*%======%...+-::-*+.=*===*%@@ +-  -- @@*=++*@@ *+-                     
      -- @@*++*+++*+==*:+%@%*-%%%=%+#***#@@ +- - @@%**%@@ *+=                       
     :- @@+===+#=-=-=****##%%%%##*%+==%+=*@@ +-  @%=::=@@ +:                        
    -- @%=======*##+===#=#+*%#+##+==+%#+==*@@   @%==+#%%% *-                        
   -- @%=====-======+%*=+%*+#*+#++#+=*#+===*@@ @@=====*@@ +:                        
  -  @#======-=*#==#+=====%+#*+#+==**#*+====*@@@+====+#@ *=                         
 -- @%========**+++=#+====#*#*+#===#+@#++++==*@#=====*%@ +-                         
-  @%=-==--==%@*+++==*#===+##*+#==**+@@#+++=+%%=====+%% ==                          
:- @%=------=%@@%++++==+#===#**+#=+#=#%%@*++++=======*@@ =:                          
:- @%===----+@@ %@#++++===#+=#*#+*=#==%% @@*++=======*%@ +=                           
-  @#==++++*%@   @%++++====*%*##+#%===%%  @%*++=====+%@ *+:                           
-- %#+======*@@@*@#**+++====*#%#+%#===@%   @@%#*+=+*%#:==                             
.-- @%*+*+====+%@@*+++*+====+#%##%%===@@ **   @@@@@%+.+=-                             
-=  @@*+=======##*+++++===+#%##%%==+@% +-                            
  =+  @@%*+===++..=%+++====####%%===@% *-                                 
    -=+  @@@%%-:-*+*%*+====####%%===#@%+=                                                                         
                                                          
)" << endl;
            cout << "Seer, choose a player to inspect: ";
            cin >> seerTarget;
            for (auto& p : players) {
                if (p.name == seerTarget && p.alive) {
                    cout << "The role of " << seerTarget << " is: ";
                    switch (p.role) {
                        case WEREWOLF: cout << "Werewolf"; break;
                        case VILLAGER: cout << "Villager"; break;
                        case SEER: cout << "Seer"; break;
                        case BODYGUARD: cout << "Bodyguard"; break;
                        case BEGGAR: cout << "Beggar"; break;
                        case SPELLCASTER: cout << "Spellcaster"; break;
                        case HUNTER: cout << "Hunter"; break;
                    }
                    cout << endl;
                    break;
                }
            }

            // Wait for Enter to hide the action
            cout << "Press Enter to hide...";
            cin.ignore();  // Clear buffer
            cin.get();     // Wait for Enter
            
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
                ..    .              .:--:.            .                        .    
.                 .    . .          .::::::::.                       .          .     
                       .          .::::::::::        .            .            .   . 
                              ...-*****+*#***+.. .  .                          .     
.                              :::=%#%#:*@%%*:=+.    . .                        .    
                               .:::::::::::::::-.                              .     
           .     .             :::::::::::::::::.                                    
                              .:::::::::::::::::. .     .                   .        
           .       .        -#=:::::::::::::::::-#=                            .     
     .        .      .   -#@@@+.:::::::::::::::.=@@@%-            .                  
.   .                .     +@@@@@@@.   .:::::::..   @@@@@@@*.                            
                     +@@@@@@@@@%..    =#*    ..#@@@@@@@@@*                .      .   
                    #@@@@@@@@@@@*  ..-#%%-..  *@@@@@@@@@@@#.     .                   
           .       #@@@@@@@@@@@@@%.  .*##:   #@@@@@@@@@@@@@%. .  .             .     
.   .                 =@@@@@@@@@@@@@@@%. =*##+ .#@@@@@@@@@@@@@@@+                        
        .         *@@@@@@@@@@@@@@@@@=+*##*-@@@@@@@@@@@@@@@@@#.           .           
.                 #@@@@@@@@@@@@@@@@@@%*##%@@@@@@@@@@@@@@@@@@%.                  .    
.                 .  *@@@@@@@@@@@@@@@@@@@@%@@@@@@@@@@@@@@@@@@@@#.           .           
.      .           -%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@=    .                   
.                       -@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@=           .             
   .                :%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%:                      .   
   .                 .%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@:      . .                  
.                      .#@@@@@@@@@@@@@@+*+@@@@@@@@@@@@@@%.  . .                     . 
.    .                        -%@@@@@@@@@@#:::..*@@@@@@@@@@@+                              
. .            .            .   :%@@@@@@@@%-:::-%@@@@@@@@@=       .                        
                            .=#@@@@@@@***@@@@@@@%=..         .        .              
        . . .                  .@@@@@@*++@@@@@@-                                 .  .
                            .   #@@@@@:  @@@@@%..   .          .                  .  
.           .                    +@@@@@:  @@@@@*                                      
                                -%@@@@:  @@@@@=           .                         .
  .                          .  .%@@@@:  @@@@@:    .               .  .              
                  .     .        *@@@@:  @@@@%.    .              . .      .         
    .                          . -@@@@:..@@@@+                    .       ..         
.                             .........:%@@%...%@@@-..........                  .          
.                      .........=@@@@@%#...*@@@@@@+.........                           
                 .     ........-=:...........:=-........   .   .              .      
                      .        ..................       .               .  .         
)" << endl;
            cout << "Bodyguard, choose a player to protect: ";
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
                -----------------*%=-------------------------------------
                -----------------=@@=------------------------------------
                ------------------*@%------------------------------------
                -------------------#@#-----------------------------------
                -------------------=%@+----------------------------------
                ---------*@#=-*@#---=@@=---------------------------------
                ----------+%@@@*-----+@%---------------------------------
                ----------------------*@#--------------------------------
                -----------------------#@*-------------------------------
                -----------------------=@@-----+@@@#---------------------
                ------------------------+@%----@@=+@%--------------------
                --------------------====*@%---=@%-=@@--------------------
                -------------------+@@@@@*----=@%-=@@--------------------
                --------------------=@@=------=@%-=@@--------------------
                --------------------=#@@=-----=@%-=@@--------------------
                -------------------+@@@@=-----=@%-=@@--------------------
                --------------------=%@#------=@%-=@@@@@***=-------------
                -------------------+@@+-------=@%-=@@==@@%%@@@@%=--------
                -------------------*@*--------=@%-----=@%=-@@==@@--------
                -------------------%@=--------=@%----------%%--@@--------
                ------------------#@#---------=@%--------------@@--------
                --------+**++++*#@@*----------=@%--------------@@--------
                -------=@@%%@@@%#=------------=@%--------------@@--------
                -------=@%---------------------@@=------------*@%--------
                -------=@%---------------------=@@#----------#@%---------
                -------=@%-----------------------%@+--------*@#----------
                -------=@#-----------------------+@*--------#@=----------      
                    )" << endl;
            cout << "Spellcaster, choose a player to silence: ";
            cin >> spellTarget;

            // Wait for Enter to hide the action
            cout << "Press Enter to hide...";
            cin.ignore();  // Clear buffer
            cin.get();     // Wait for Enter
            
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
                    cout << p.name << " was protected and survived the attack!" << endl;
                } else {
                    p.alive = false;
                    cout << p.name << " was killed by the Werewolves." << endl;

                    if (p.role == HUNTER) {
                        hunterRevenge(players);
                    }
                }
                break;
            }
        }
    } else {
        cout << "No valid target was chosen by the Werewolves." << endl;
    }
}


void dayPhase(vector<Player>& players) {
    cout << "\nDay breaks. Villagers, choose someone to lynch." << endl;
    string targetName;
    cout << "Enter the name of the player to lynch: ";
    cin >> targetName;

    for (auto& player : players) {
        if (player.name == targetName && player.alive) {
            player.alive = false;
            cout << player.name << " was lynched by the Villagers." << endl;
            return;
        }
    }
    cout << "Player not found or already dead." << endl;
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
    werewolves == 0 ? cout << "Villagers win!" << endl : cout << "Werewolves win!" << endl;
}

int main() {
    int numPlayers;
    
    cout << R"(                                          .                .              
    ..                                              .                .              
                 -@@@@%           .     .                                           
             .  .@@@@@@@@#              .               .             .#     .     .
                #@@@@@@@@@@%                                        .#@@@#          
            . . %@@@@@@@@@@@@%  %@+       :#-                .      #@@@@@@.  . .   
         .  .   %@@@@=..+@@@@@@@=@@@@@#=   =@@@@@@@%##*+:       .  #@@@@@@@@+       
       ..     . #@@@@:....-@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*   *@@@@@@@@@@-      
           .    #@@@@:...:..:*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#@@@@@@.    .
                =@@@@-..@@@+...:%@@@@@@%*@@@@@@@@@%.....::=#@@@@@@@@@@%.-@@@@@%     
             .   @@@@%.-@@@@@+....-@@@@@@@:....................*@@@@@@%..+@@@@@:    
     .           +@@@@++@@@*:........-#@@@@@#:....................*@@@@@==@@@@@+    
                  @@@@@%@@@@@@%*:.......:=%@@@@*....................+@@@@@@@@@@*    
              .   -@@@@@@@@@@@@@@@@#.........:*%@@%:..................*@@@@@@@@*    
         .     -#@@@@@@@@@@@@@#=:.....:................................:%@@@@@@+    
          *%@@@@@@@@@@@@@@%%%@@@@@@*...................................#@@@@@@@@-.  
        :@@@@@@@@@=.....................................................#@@@@@@@@   
            =%@@@@@%=..................................... .................:%@@@@  
                %@@@@@#:..........................:  .       ......... .......#@@@  
   .        .-@@@@@@@@@@@@@@@@+....................+@@@%     #=......-     ...-@@@= 
     . .. #@@@@@@@@@@@@@%#####*###*-.......... +@@@@@@@@@@   .:......-   -@#@:-@@@: 
        %@@@@@@@@@#=:.........................  +@..#@@@@@@: .:.......:  @@%.%+@@@. 
   .  %@@@@@@@@#+=:...........................    @@....%@@@@*.........%@@..%@@@@@  
        :+#@@@@@@@@@@@@#=................#*....     .+*+*@@%.............%@@@@@@@+ .
   .     .     -%@@@@@@-.....             @%=..      .     :% ...........%@@@@@@%   
    .   .         @@@%....      --   .     %@#.       .  .      .........+@@@@@@    
                %@@@#..  :%@@@=            +# .             . .  .........#@@@@%.   
              +@@@@%.:#@@@@@-       .. . . :   .   +@@*           ........:@@@@@    
        ..   %@@@@@#@@@@@@@ .*%%@@@%%#          . @@@@@@#          ........=@@@@#   
            %@@@@@@@@@@@@@@@@@@@@@@@=.  .        %%%%###%#          ..::....#####   
           %@@@@@@@@@@@@@@@@@@@@@%               **.:+#**.+         .::::::::+***+  
          #@@@@@#    @@@@@@@@@@@@#  .   .@%+    :**.  .  . ++        +-++*********. 
          %+        %@@@@@%@@@@@@@@@@@     @@%   .*:.    =   .       +**+:  .=****+ 
   .        .       @@@+  %@@#**%@@@@@@@@   @@@    *####***+*+*+    .+**###*******- 
   .                @%  .     :@@@@@@@@@@@@  @@@    @@@%#***%@@@@#   *@@@@@@@@@@@@  
                    : .               =%@@@@#@@@=   -@@@@***#+#@@@@@@*:@@@@@@@@@@.  
                  .   .       .           %@@@@@@    @%%@#****#@= .@@@@@@@@@@@@@.   
    .                .                      %@@@@=   +@ @#****#@  @@@*    +% +@     
    .      .    .    .                        @@@@   .@ %@****%@@#@@@*    @@#       
    .       .      ..         .               *@@@%   @#.@@@*****%@@%@+            .
                                               *@@@   :@   #%#%%@%   @              
              .                                 %@@@   %@.  *@@@%  #%    .        ..
      . .       .                ..   .          %@@@    @%+-. .=%+@         .    . 
   .   .                                   .      %@@@=     +@%-  @.                
       .                              .             %@@@@=.  . =@@        .     .   
                     .                                *@@@@@@@@@-       ..     .  . 
      .        ..             .       .                                .           .
       )" << endl;

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
    cout << "Game over!" << endl;
    return 0;
}
