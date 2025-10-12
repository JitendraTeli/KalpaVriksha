#include<stdio.h>
#include<string.h>

void addUser(char *fileName,int userId, char *userName,int userAge) {
    FILE *filePointer = fopen(fileName,"a");                            

    if(filePointer == NULL) {
        printf("Internal Server Error");
    }

    fprintf(filePointer,"%d,%s,%d\n",userId,userName,userAge);
    fclose(filePointer);

}

int getIDForNewUser(char *fileName) {                          
    FILE *filePointer = fopen(fileName, "r");
    if(filePointer == NULL) return 2000;
    int userId =  2000,tid = 2000,tage = 0;
    char tname[100];                                      

    while(fscanf(filePointer,"%d,%99[^,],%d",&tid,tname,&tage) == 3) {
        if(userId < tid) userId = tid;                                      // to get the max available user id for incremental sequence 
    }

    fclose(filePointer);
    return userId+1;
    
}

void readUsers(char *fileName) {
    FILE *filePointer = fopen(fileName,"r");

    if(filePointer == NULL) printf("Internal Server Error");
    

    int userId,userAge;
    char userName[100];

    printf("\nID       Name                                Age\n");
    printf("________________________________________________\n\n");

    while(fscanf(filePointer,"%d,%99[^,],%d",&userId,userName,&userAge) == 3) {
        printf("%-8d %-30s %8d\n",userId,userName,userAge);
    }

    fclose(filePointer);
}

void updateUser(char *fileName,int userId,char *userName,int userAge) {
    FILE *filePointer = fopen(fileName,"r");

    if(filePointer == NULL) printf("Internal Server Error");

    int userFound = 0;                                 // 0 -> false , 1 -> True



    int tid,tage;
    char tname[100];

    FILE *temporaryFile = fopen("temp.txt","w");

    while(fscanf(filePointer,"%d,%99[^,],%d",&tid,tname,&tage) == 3) {

        if(userId == tid) {
            fprintf(temporaryFile,"%d,%s,%d\n",userId,userName,userAge);
            userFound = 1;
        }else fprintf(temporaryFile,"%d,%s,%d\n",tid,tname,tage);
    }

    fclose(filePointer);
    fclose(temporaryFile);
    
    if(userFound == 1) {
        remove(fileName);
        rename("temp.txt",fileName);

    } else {
        remove("temp.txt");
        printf("given user Id not Found ");
    }



}

void deleteUser(char *fileName,int userId) {
    FILE *filePointer = fopen(fileName,"r");

    if(filePointer == NULL) printf("Internal Server Error");
    
    int userFound = 0;

    int tid,tage;
    char tname[100];

    FILE *temporaryFile = fopen("temp.txt","w");

    while(fscanf(filePointer,"%d,%99[^,],%d",&tid,tname,&tage) == 3) {

        if(userId == tid) {
            userFound = 1;
        }else fprintf(temporaryFile,"%d,%s,%d\n",tid,tname,tage);
    }

    fclose(filePointer);
    fclose(temporaryFile);
    
    if(userFound == 1) {
        remove(fileName);
        rename("temp.txt",fileName);

    } else {
        remove("temp.txt");
        printf("given user Id not Found ");
    }

}
void createUser(char *fileName) {
    
    int userAge;
    char userName[100];

    printf("Enter user userName : ");
    scanf(" %99[^\n]",userName); 
    printf("Enter user userAge  : ");
    scanf("%d",&userAge);

    addUser(fileName,getIDForNewUser(fileName),userName,userAge);

}


void editUser(char *fileName) {
    int userId,userAge;
    char userName[100];

    printf("Enter user userId   : ");
    scanf("%d",&userId);
    printf("Enter user userName : ");
    scanf(" %99[^\n]",userName);  
    printf("Enter user userAge  : ");
    scanf("%d",&userAge);

    updateUser(fileName,userId,userName,userAge);

}

void removeUser(char *fileName) {
    int userId;

    printf("Enter user userId   : ");
    scanf("%d",&userId);

    deleteUser(fileName,userId);
}

void showMenu(char *fileName) {
    int userChoice = 0;
    do{
        userChoice = 0;
        
        printf("\n1. Create User\n");
        printf("2. Update User\n");
        printf("3. Delete User\n");
        printf("4. Show Users\n");
        printf("0. Exit\n");
        printf("Enter operation : ");
        scanf("%d",&userChoice);
        printf("\n");

        switch(userChoice) {
            case 0: printf("\nClosing the app, adios amigo");
            break;
            case 1:createUser(fileName);
            break;
            case 2:editUser(fileName);
            break;
            case 3:removeUser(fileName);
            break;
            case 4:readUsers(fileName);
            break;
            default: printf("Invalid option");
        }
    } while(userChoice);
}



int main() {
    
    char *fileName = "users.txt";

    showMenu(fileName);

    return 0;

}