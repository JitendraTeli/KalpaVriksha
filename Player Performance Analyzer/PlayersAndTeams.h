#ifndef Player_Structure_H

#define Player_Structure_H

#define NAME_LENGTH 51

typedef struct Player {
    int id;
    char name[NAME_LENGTH];
    char team[NAME_LENGTH];
    char role[NAME_LENGTH];

    int totalRuns;
    float battingAvergae;
    float strikeRate;
    int wickets;
    float economyRate;
    float performanceIndex;

    struct Player *next;
} Player;

typedef struct {
    int id;
    char name[NAME_LENGTH];
    int totalPlayers;
    float averageBattingStrikeRate;

    Player *ballers;
    Player *batsmen;
    Player *allRounders;
} Team;


Player** getPlayersByRole(Team *team,char *role);

int sumOfStrikRate(Player *player,int *count);

void updatePerforManceIndex(Player *player);

Player* appendPlayer(Player *best,Player *newPlayer);

void printPlayerTile(Player *player);


void showList(Player *head);

void printTopK(Player *player,int k);


void updateBattingAverageStrikeRate(Team *team);

void addPlayerInTeam(Team *team,Player *player);

Team* initializeTeams(Team *teamArray,const char **teams,int teamCount);


void updateBattingAverageOfAllTeams(Team *teamArray,int totalTeams);

void displayTeam(Team *team);

void printTeamTile(Team *team);

void printTeamHeader();

void printLine();

void printPlayerHeader();


#endif