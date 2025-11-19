#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include "Player.h"

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

