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

int sumOfStrikRate(Player *player,int *count) {
    float sum = 0;
    while(player != NULL) {
        sum += player->strikeRate;
        *count++;

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

void heapify(Player **heap,int size,int rootIndex) {
    if(rootIndex >= size) return;

    int leftChild = rootIndex * 2 + 1;
    int rightChild = rootIndex * 2 + 2;
    int largest = rootIndex;

    if(leftChild < size) if(heap[rootIndex]->performanceIndex < heap[leftChild]->performanceIndex) largest = leftChild;

    if(rightChild < size) if( heap[rootIndex]->performanceIndex < heap[rightChild]->performanceIndex) largest = rightChild;

    printf("somewhere %d\n",largest);

    if(largest != rootIndex) {
        swapPlayers(heap+largest,heap+rootIndex);
        heapify(heap,size,largest);
    } 

}

Player** initializePlayerHeap(char *role,int *size) {
    Player **heapOfPlayers = (Player **)calloc(totalTeams+2,sizeof(Player*));

    for(int i = 0; i<totalTeams; i++) {
        Player *player = *getPlayersByRole(teamArray+i,role);

        if(player != NULL) {
            printPlayerTile(player);
            heapOfPlayers[*size] = player;
            *size++;
            heapify(heapOfPlayers,*size,0);
        }
    }

    return heapOfPlayers;
}

void ShowAllPlayers(char *role) {
    int size = 0;
    Player **heap = initializePlayerHeap(role,&size);

    while(size >= 0) {
        Player *player = heap[0];

        printPlayerTile(player);

        if(player->next == NULL) {
            swapPlayers(heap + 0, heap + size-1);
            size--;
        } else heap[0] = player->next;

        heapify(heap,size,0);
    }
}

Team* inputTeam() {
    int teamId = 0;

    printf("Enter Team Id: ");
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

    addPlayerInTeam(team,newPlayer);

    printf("Player added Successfully to Team %s",team->name);
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

void handleChoice(int choice) {
    switch (choice) {
        case 1: inputPlayer();
                break;
        case 2: displayAllPlayersOfTeam();
                break;
        case 3: 
                break;
        case 4: displayTopKPlayers();
                break;
        case 5: showEverything();
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
        printLine();
        showTitle();
        printLine();
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

    showEverything();
    
    updateBattingAverageOfAllTeams();
    
    ShowTopK(teamArray + 5,5,"All-rounder");

    printHeader();

    ShowAllPlayers("Batsman");

    return 0;
}