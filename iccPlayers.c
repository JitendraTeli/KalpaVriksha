#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "PLAYERS_DATA.H"   

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

Player *playerList = NULL;
int totalPlayers = 0;

Team *teamArray = NULL;
int totalTeams = 0;


int getIndexById(int id) {
    int low = 0;
    int high = totalTeams-1;
    int mid = 0;
    
    while(low <= high) {
        mid = low + (high - low)/2;
        if(teamArray[mid].id == id ) return mid;

        if(teamArray[mid].id > id) high = mid-1;
        else low = mid + 1;
    }
    return -1;
}

int getIndexByName(char *name) {
    for(int i = 0; i < totalTeams; i++)
        if(strcmp(teamArray[i].name, name) == 0 ) 
            return i;

    return -1;
}

void initializeTeams() {
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

    totalTeams = teamCount;
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

void addPlayerInTeam(int index,Player *player) {
    if(strcmp(player->role,"Bowler") == 0) 
        teamArray[index].ballers = appendPlayer(teamArray[index].ballers,player);
    
    else if(strcmp(player->role,"Batsman") == 0)
        teamArray[index].batsmen = appendPlayer(teamArray[index].batsmen,player);
    
    else teamArray[index].allRounders = appendPlayer(teamArray[index].allRounders,player);

    teamArray[index].totalPlayers++;
    
}

void initializePlayers() {
    
    for(int i = 0; i<playerCount; i++) {
        Player *newPlayer = (Player *)malloc(sizeof(Player));
        
        newPlayer->id = players[i].id;
        strcpy(newPlayer->name,players[i].name);
        strcpy(newPlayer->team,players[i].team);
        strcpy(newPlayer->role,players[i].role);

        newPlayer->battingAvergae = players[i].battingAverage;
        newPlayer->economyRate = players[i].economyRate;
        newPlayer->wickets = players[i].wickets;
        newPlayer->totalRuns = players[i].totalRuns;
        newPlayer->strikeRate = players[i].strikeRate;

        newPlayer->next = NULL;
        
        updatePerforManceIndex(newPlayer);

        addPlayerInTeam(getIndexByName(newPlayer->team),newPlayer);
    }
}

void showList(Player *head) {
    while(head != NULL) {
        printf("%-25s %-15s %-15s %.3f\n",head->name,head->team,head->role,head->performanceIndex);
        head = head->next;
    }
}

void showEverything() {
    for(int i = 0; i < totalTeams; i++) {
        printf("%s\n\n",teamArray[i].name);

        printf("\nBatsmen\n\n");
        showList(teamArray[i].batsmen);

        printf("\nBowlers\n\n");
        showList(teamArray[i].ballers);

        printf("\nAll Rounders\n\n");
        showList(teamArray[i].allRounders);
    }
}




int main() {

    initializeTeams();

    initializePlayers();
    
    showEverything();

    return 0;
}