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

int sumOfStrikRate(Player *player,int *count);

void updatePerforManceIndex(Player *player);

Player* appendPlayer(Player *best,Player *newPlayer);

void printPlayerTile(Player *player);


void showList(Player *head);

void printTopK(Player *player,int k);

#endif