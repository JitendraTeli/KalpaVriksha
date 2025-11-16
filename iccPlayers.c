#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "PLAYERS_DATA.H"   

#define NAME_LENGTH 51
#define TRUE 1
#define FALSE 0

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

Team *teamArray = NULL;
int totalTeams = 0;


Team* getTeamById(int id) {
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

Team* getTeamByName(char *name) {
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

int sumOfStrikRate(Player *player,int *count) {
    float sum = 0;
    while(player != NULL) {
        sum += player->strikeRate;
        (*count)++;

        player = player->next;
    }
    return sum;
}

void updateBattingAverageStrikeRate(Team *team) {
    
    int count = 0;
    team->averageBattingStrikeRate += sumOfStrikRate(team->batsmen,&count);
    team->averageBattingStrikeRate += sumOfStrikRate(team->allRounders,&count);

    team->averageBattingStrikeRate /= count;
}

void updateBattingAverageOfAllTeams() {
    for(int i = 0; i<totalTeams; i++) updateBattingAverageStrikeRate(teamArray + i);
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

void addPlayerInTeam(Team *team,Player *player) {
    Player **playerList = getPlayersByRole(team,player->role);

    *playerList = appendPlayer(*playerList,player);

    team->totalPlayers++;
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

        addPlayerInTeam(getTeamByName(newPlayer->team),newPlayer);
    }
}



void printLine() {
    printf("\n");
    for(int i = 0; i < 120; i++) printf("=");
    printf("\n");
}

void printHeader() {
    
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

void displayTeam(Team *team) {
    showList(team->ballers);
    showList(team->batsmen);
    showList(team->allRounders);
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

void printTopK(Player *player,int k) {
    while(player != NULL && k-- > 0) {
        printPlayerTile(player);
        player = player->next;
    }
}

void ShowTopK(Team *team,int K,char *role) {    
    Player **playerList = getPlayersByRole(team,role);
    
    printHeader();
    
    printTopK(*playerList,K);

    printLine();

}

void swapPlayers(Player **greater, Player **lesser) {
    Player *temp = *greater;
    *greater = *lesser;
    *lesser = temp;
}

//recursive
void heapifyDown(Player **heap,int size,int rootIndex) {
    if(rootIndex >= size) return;

    int leftChild = rootIndex * 2 + 1;
    int rightChild = rootIndex * 2 + 2;
    int largest = rootIndex;

    if(leftChild < size) if(heap[largest]->performanceIndex < heap[leftChild]->performanceIndex) largest = leftChild;

    if(rightChild < size) if( heap[largest]->performanceIndex < heap[rightChild]->performanceIndex) largest = rightChild;

    if(largest != rootIndex) {
        swapPlayers(heap+largest,heap+rootIndex);
        heapifyDown(heap,size,largest);
    } 

}

//iterative
void heapifyUp(Player **heap,int index) {
    while(index > 0) {
        int parent = (index - 1)/2;

        if(heap[parent]->performanceIndex >= heap[index]->performanceIndex) break;
        
        swapPlayers(heap+parent,heap+index);
        index = parent;
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

void printTeamTile(Team *team) {
    printf("\n");
    printf("%-5d ",team->id);
    printf(" %-15s",team->name);
    printf(" %-14d",team->totalPlayers);
    printf(" %-10.3f",team->averageBattingStrikeRate);
    printf("\n");
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

    return getTeamById(teamId);
}

void inputPlayer() {
    Player *newPlayer = (Player *) malloc(sizeof(Player));

    Team *team = inputTeam();

    printf("Player ID: ");
    scanf("%d",&newPlayer->id);

    printf("Name: ");
    scanf("%s",newPlayer->name);

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

    printf("Player added Successfully to Team %s",team->name);
}

void displayAllPlayersOfTeam() {
    Team *team  = inputTeam();
    
    printHeader();

    displayTeam(team);
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

    ShowTopK(team,K,role);
}   

void swapIntegers(int *lesser,int *greater) {
    int temp = *lesser;
    *lesser = *greater;
    *greater = temp;
}

int partition(int *array,int low,int high) {

    int index = low-1;

    for(int i = low; i<high; i++) {
        if(teamArray[array[i]].averageBattingStrikeRate >= teamArray[array[high]].averageBattingStrikeRate) {
            index++;
            
            swapIntegers(array+index,array+i);
        }
    }
    index++;
    swapIntegers(array+index,array+high);

    return index;
}

void quickSort(int *array,int low,int high) {
    if(low < high) {
        int pivot = partition(array,low,high);

        quickSort(array,low,pivot-1);
        quickSort(array,pivot+1,high);
    }

    
}

void displayTeamsByAverage() {

    int teamIndices[teamCount];

    for(int i = 0; i<teamCount; i++) teamIndices[i] = i;

    quickSort(teamIndices,0,teamCount-1);

    printTeamHeader();

    for(int i = 0; i<teamCount; i++) printTeamTile(teamArray + teamIndices[i]);
}

void displayEveryPlayer() {
    char role[NAME_LENGTH];

    printf("Role (Batsman, Bowler, All-rounder): ");
    scanf("%s",role);

    printHeader();
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

    initializeTeams();

    initializePlayers();
    
    updateBattingAverageOfAllTeams();
    
    startInput();

    return 0;
}