#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include "PlayersAndTeams.h"


Player** getPlayersByRole(Team *team,char *role) {
    if(strcmp(role,"Bowler") == 0) return &team->ballers;

    if(strcmp(role,"Batsman") == 0) return &team->batsmen;

    return &team->allRounders;
}

int sumOfStrikRate(Player *player,int *count) {
    float sum = 0;
    while(player != NULL) {
        sum += player->strikeRate;
        (*count)++;

        player = player->next;
    }
    return sum;
}

void updatePerforManceIndex(Player *player) {
    if(strcmp(player->role,"Bowler") == 0)  
    player->performanceIndex = (float) player->wickets * 2.0 + 100 - player->economyRate;
    
    else if(strcmp(player->role,"Batsman") == 0) 
    player->performanceIndex = (float) (player->battingAvergae * player->strikeRate) / 100.0;
    
    else player->performanceIndex = (float) (player->battingAvergae * player->strikeRate / 100.0) + player->wickets * 2.0; 
    
}


Player* appendPlayer(Player *best,Player *newPlayer) {
    if(best == NULL)  return newPlayer;

    if(best->performanceIndex < newPlayer->performanceIndex) {
        newPlayer->next = best;
        return newPlayer;
    }

    Player *player = best;

    while(player->next != NULL) {
        if(player->next->performanceIndex < newPlayer->performanceIndex) break;
        player = player->next;
    }

    newPlayer->next = player->next;
    player->next = newPlayer;

    return best;
}

void printPlayerTile(Player *player) {
    printf("\n");
    
    printf("%-5d ",player->id);
    printf(" %-25s ",player->name);
    printf(" %-15s ",player->team);
    printf(" %-15s ",player->role);
    printf(" %-5d ",player->totalRuns);
    printf(" %-5.2f ",player->battingAvergae);
    printf(" %-5.2f ",player->strikeRate);
    printf(" %-5d ",player->wickets);
    printf(" %-5.2f ",player->economyRate);
    printf(" %-11.2f ",player->performanceIndex);
    
    printf("\n");
}

void showList(Player *head) {
    while(head != NULL) {
        printPlayerTile(head);
        head = head->next;
    }
}

void printTopK(Player *player,int k) {
    while(player != NULL && k-- > 0) {
        printPlayerTile(player);
        player = player->next;
    }
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

Team* initializeTeams(Team *teamArray,const char **teams,int teamCount) {
    teamArray = (Team *) calloc(teamCount,sizeof(Team));
    
    for(int i = 0; i<teamCount; i++) {
        teamArray[i].id = i + 1000;
        strcpy(teamArray[i].name,teams[i]);
        teamArray[i].allRounders = NULL;
        teamArray[i].batsmen = NULL;
        teamArray[i].ballers  = NULL;
        teamArray[i].totalPlayers = 0;
        teamArray[i].averageBattingStrikeRate = 0;
    }

    return teamArray;
}

void updateBattingAverageOfAllTeams(Team *teamArray,int totalTeams) {
    for(int i = 0; i<totalTeams; i++) updateBattingAverageStrikeRate(teamArray + i);
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

void printLine() {
    printf("\n");
    for(int i = 0; i < 120; i++) printf("=");
    printf("\n");
}

void printTeamHeader() {
    printLine();
    printf("\n");

    printf("%-5s ","ID");
    printf(" %-15s","Name");
    printf(" %-14s","Player Count");
    printf(" %-20s","Batting Average");

    printf("\n");
    printLine();
}
    

void printPlayerHeader() {
    
    printf("\n");
    printLine();
    printf("\n");
    
    printf("%-5s ","ID");
    printf(" %-25s ","Name");
    printf(" %-15s ","Team");
    printf(" %-15s ","Role");
    printf(" %-5s ","Runs");
    printf(" %-5s ","Avg");
    printf(" %-5s ","SR");
    printf(" %-5s ","Wkts");
    printf(" %-5s ","ER");
    printf(" %-11s ","Perf.Index");
    
    printf("\n");
    printLine();
    printf("\n");
}

