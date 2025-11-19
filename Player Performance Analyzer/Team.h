
#ifndef Team_Structure_H
#define Team_Structure_H

#include "Player.h"

#define NAME_LENGTH 51

typedef struct {
    int id;
    char name[NAME_LENGTH];
    int totalPlayers;
    float averageBattingStrikeRate;

    Player *ballers;
    Player *batsmen;
    Player *allRounders;
} Team;

Team* getTeamById(Team *teamArray,int totalTeams,int id);

Team* getTeamByName(Team *teamArray,char *name,int totalTeams);

Player** getPlayersByRole(Team *team,char *role);

void updateBattingAverageStrikeRate(Team *team);

void addPlayerInTeam(Team *team,Player *player);


void displayTeam(Team *team);

void printTeamTile(Team *team);

void printTeamHeader();



#endif