#include<stdio.h>
#include<string.h>

void addUser(char *fileName,int id, char *userName,int age) {
    FILE *p = fopen(fileName,"a");                            

    if(p == NULL) {
        printf("Internal Server Error");
    }

    fprintf(p,"%d,%s,%d\n",id,userName,age);
    fclose(p);

}

int getIDForNewUser(char *fileName) {                          
    FILE *p = fopen(fileName, "r");
    if(p == NULL) return 2000;
    int id =  2000,tid = 2000,tage = 0;
    char tname[100];                                      

    while(fscanf(p,"%d,%99[^,],%d",&tid,tname,&tage) == 3) {
        if(id < tid) id = tid;
    }

    fclose(p);
    return id+1;
    
}

void readUsers(char *fileName) {
    FILE *p = fopen(fileName,"r");

    if(p == NULL) printf("Internal Server Error");
    

    int id,age;
    char name[100];

    printf("\nID       Name                                Age\n");
    printf("________________________________________________\n\n");

    while(fscanf(p,"%d,%99[^,],%d",&id,name,&age) == 3) {
        printf("%-8d %-30s %8d\n",id,name,age);
    }

    fclose(p);
}

void updateUser(char *fileName,int id,char *userName,int age) {
    FILE *p = fopen(fileName,"r");

    if(p == NULL) printf("Internal Server Error");

    int found = 0;



    int tid,tage;
    char tname[100];

    FILE *t = fopen("temp.txt","w");

    while(fscanf(p,"%d,%99[^,],%d",&tid,tname,&tage) == 3) {

        if(id == tid) {
            fprintf(t,"%d,%s,%d\n",id,userName,age);
            found = 1;
        }else fprintf(t,"%d,%s,%d\n",tid,tname,tage);
    }

    fclose(p);
    fclose(t);
    
    if(found == 1) {
        remove(fileName);
        rename("temp.txt",fileName);

    } else {
        remove("temp.txt");
        printf("id not found ");
    }



}

void deleteUser(char *fileName,int id) {
    FILE *p = fopen(fileName,"r");

    if(p == NULL) printf("Internal Server Error");
    
    int found = 0;

    int tid,tage;
    char tname[100];

    FILE *t = fopen("temp.txt","w");

    while(fscanf(p,"%d,%99[^,],%d",&tid,tname,&tage) == 3) {

        if(id == tid) {
            found = 1;
        }else fprintf(t,"%d,%s,%d\n",tid,tname,tage);
    }

    fclose(p);
    fclose(t);
    
    if(found == 1) {
        remove(fileName);
        rename("temp.txt",fileName);

    } else {
        remove("temp.txt");
        printf("id not found ");
    }

}
void createUser(char *fileName) {
    
    int tage;
    char tname[100];

    printf("Enter user name : ");
    scanf(" %99[^\n]",tname); 
    printf("Enter user age  : ");
    scanf("%d",&tage);

    addUser(fileName,getIDForNewUser(fileName),tname,tage);

}


void editUser(char *fileName) {
    int id,age;
    char name[100];

    printf("Enter user ID   : ");
    scanf("%d",&id);
    printf("Enter user name : ");
    scanf(" %99[^\n]",name);  
    printf("Enter user age  : ");
    scanf("%d",&age);

    updateUser(fileName,id,name,age);

}

void removeUser(char *fileName) {
    int id;

    printf("Enter user ID   : ");
    scanf("%d",&id);

    deleteUser(fileName,id);
}

void meenu(char *fileName) {
    int c = 0;
    do{
        c = 0;
        
        printf("\n1. Create User\n");
        printf("2. Update User\n");
        printf("3. Delete User\n");
        printf("4. Show Users\n");
        printf("0. Exit\n");
        printf("Enter operation : ");
        scanf("%d",&c);
        printf("\n");

        switch(c) {
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
    } while(c);
}



int main() {
    
    char *fileName = "users.txt";

    meenu(fileName);

    return 0;

}