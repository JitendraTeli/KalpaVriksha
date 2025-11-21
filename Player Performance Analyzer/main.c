#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "PLAYERS_DATA.H"   
#include "PlayersAndTeams.h"
#include "RequiredSortingAndSearching.h"

#define NAME_LENGTH 51
#define TRUE 1
#define FALSE 0

Team *teamArray = NULL;
int totalTeams = 0;

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

        addPlayerInTeam(getTeamByName(teamArray, newPlayer->team, totalTeams),newPlayer);
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

Player** initializePlayerHeap(char *role,int *size) {
    Player **heapOfPlayers = (Player **)calloc(totalTeams+2,sizeof(Player*));

    for(int i = 0; i<totalTeams; i++) {
        Player *player = *(getPlayersByRole(teamArray+i,role));

        if(player != NULL) {
            heapOfPlayers[*size] = player;
            (*size)++;
            heapifyUp(heapOfPlayers,*size - 1);
        }
    }

    return heapOfPlayers;
}

void ShowAllPlayers(char *role) {
    int size = 0;
    Player **heap = initializePlayerHeap(role,&size);

    while(size > 0) {
        Player *player = heap[0];

        printPlayerTile(player);

        if(player->next == NULL) {
            heap[0] = heap[size-1];
            size--;
        } else heap[0] = player->next;

        heapifyDown(heap,size,0);
    }
}

void clearInput() {
    int something;
    while((something = getchar()) != '\n' && something != EOF);
}

char* getName() {
    char *name = (char *) malloc(NAME_LENGTH + 10 * sizeof(char));

    scanf("%60s",name);
    
    if(strlen(name) > 50 ) {
        printf("error : name length cannot exceed 50");
        clearInput();
        free(name);
        return NULL;
    }

    return name;
}

void printTeamIds() {
    printf("Team IDs\n");
    for(int i = 0; i<teamCount; i++) {
        printf("%d\t%s\n",teamArray[i].id,teamArray[i].name);
    }
}

Team* inputTeam() {
    int teamId = 0;

    printTeamIds();

    printf("\nEnter Team Id: ");
    scanf("%d",&teamId);

    return getTeamById(teamArray,totalTeams,teamId);
}

void inputPlayer() {
    Player *newPlayer = (Player *) malloc(sizeof(Player));
    char *name;
    Team *team = inputTeam();

    printf("Player ID: ");
    scanf("%d",&newPlayer->id);

    printf("Name: ");
    name = getName();
    if(name) strcpy(newPlayer->name,name);

    free(name);

    printf("Role (Batsman,Bowler,All-rounder): ");
    scanf("%s",newPlayer->role);

    printf("Total Runs: ");
    scanf("%d",&newPlayer->totalRuns);

    printf("Batting Avergae: ");
    scanf("%f",&newPlayer->battingAvergae);

    printf("Strike Rate: ");
    scanf("%f",&newPlayer->strikeRate);

    printf("Wickets: ");
    scanf("%d",&newPlayer->wickets);

    printf("Economy Rate: ");
    scanf("%f",&newPlayer->economyRate);

    strcpy(newPlayer->team,team->name);

    newPlayer->next = NULL;
    updatePerforManceIndex(newPlayer);

    printf("%s\n",newPlayer->role);

    addPlayerInTeam(team,newPlayer);

    updateBattingAverageStrikeRate(team);

    printf("%s added Successfully to Team %s",newPlayer->name,team->name);
}

void displayAllPlayersOfTeam() {
    Team *team  = inputTeam();
    
    printPlayerHeader();

    displayTeam(team);
}


int greaterThan(int index1,int index2) {
    return (teamArray[index1].averageBattingStrikeRate >= teamArray[index2].averageBattingStrikeRate);
}

void displayTeamsByAverage() {
    
    int teamIndices[teamCount];
    
    for(int i = 0; i<teamCount; i++) teamIndices[i] = i;
    
    quickSort(teamIndices,0,teamCount-1,&greaterThan);

    printLine();
    printTeamHeader();
    printLine();
    
    for(int i = 0; i<teamCount; i++) printTeamTile(teamArray + teamIndices[i]);
}

void displayTopKPlayers() {
    int K = 0;
    char role[NAME_LENGTH];
    Team *team = inputTeam();
    
    printf("Role (Batsman,Bowler,All-rounder): ");
    scanf("%s",role);

    printf("Enter Number of Players: ");
    scanf("%d",&K);

    printf("Top %d Players of Team %s\n",K,team->name);

    printPlayerHeader();

    printTopK(*getPlayersByRole(team,role),K);

    printLine();
}   

void displayEveryPlayer() {
    char role[NAME_LENGTH];
    
    printf("Role (Batsman, Bowler, All-rounder): ");
    scanf("%s",role);
    
    printPlayerHeader();
    ShowAllPlayers(role);
}

void handleChoice(int choice) {
    switch (choice) {
        case 1: inputPlayer();
                break;
        case 2: displayAllPlayersOfTeam();
                break;
        case 3: displayTeamsByAverage();
                break;
        case 4: displayTopKPlayers();
                break;
        case 5: displayEveryPlayer();
                break;
        default: printf("\n Invalid Choice, Try Again");
    }
}

void showTitle() {
    printf("\n");
    printLine();
    printf("\n");

    printf(" ICC ODI Player Performance Analyzer ");

    printf("\n");
    printLine();
    printf("\n");

}

void showMenu() {
    printf("\n1. Add Player to Team\n");
    printf("2. Display Players of a Specific Team\n");
    printf("3. Display Teams by Average Batting Strike Rate\n");
    printf("4. Display Top K Players of a Specific Team by Role\n");
    printf("5. Display all Players of a Specific Role Across All Teams by Performance Index\n");
    printf("6. Exit \n");
}

void startInput() {
    int choice = 0;
    do {
        showTitle();
        showMenu();
        printLine();

        printf("\nEnter Your Choice : ");
        scanf("%d",&choice);

        handleChoice(choice);

    } while(choice != 6);
}

int main() {

    teamArray = initializeTeams(teamArray,teams,teamCount);
    if(teamArray) totalTeams = teamCount;

    initializePlayers();
    
    updateBattingAverageOfAllTeams(teamArray,totalTeams);
    
    startInput();

    return 0;
}