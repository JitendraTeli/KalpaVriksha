#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include "Team.h"

Team* getTeamById(Team *teamArray,int totalTeams,int id) {
    int low = 0;
    int high = totalTeams-1;
    int mid = 0;
    
    while(low <= high) {
        mid = low + (high - low)/2;
        if(teamArray[mid].id == id ) return teamArray + mid;

        if(teamArray[mid].id > id) high = mid-1;
        else low = mid + 1;
    }
    return NULL;
}

Team* getTeamByName(Team *teamArray,char *name,int totalTeams) {
    for(int i = 0; i < totalTeams; i++)
        if(strcmp(teamArray[i].name, name) == 0 ) 
            return teamArray + i;

    return NULL;
}


Player** getPlayersByRole(Team *team,char *role) {
    if(strcmp(role,"Bowler") == 0) return &team->ballers;

    if(strcmp(role,"Batsman") == 0) return &team->batsmen;

    return &team->allRounders;
}

void updateBattingAverageStrikeRate(Team *team) {
    
    int count = 0;
    team->averageBattingStrikeRate += sumOfStrikRate(team->batsmen,&count);
    team->averageBattingStrikeRate += sumOfStrikRate(team->allRounders,&count);

    team->averageBattingStrikeRate /= count;
}


void addPlayerInTeam(Team *team,Player *player) {
    Player **playerList = getPlayersByRole(team,player->role);

    *playerList = appendPlayer(*playerList,player);

    team->totalPlayers++;
}

void displayTeam(Team *team) {
    showList(team->ballers);
    showList(team->batsmen);
    showList(team->allRounders);
}


void printTeamTile(Team *team) {
    printf("\n");
    printf("%-5d ",team->id);
    printf(" %-15s",team->name);
    printf(" %-14d",team->totalPlayers);
    printf(" %-10.3f",team->averageBattingStrikeRate);
    printf("\n");
}

void printTeamHeader() {
 
    printf("\n");
    printf("%-5s ","ID");
    printf(" %-15s","Name");
    printf(" %-14s","Player Count");
    printf(" %-20s","Batting Average");
    printf("\n");
    
}
