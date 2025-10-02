#include<stdio.h>
#include<string.h>

void addUser(char *fname,int id, char *uname,int age) {
    FILE *p = fopen(fname,"a");

    if(p == NULL) {
        printf("Internal Server Error");
    }

    fprintf(p,"%d,%s,%d\n",id,uname,age);
    fclose(p);

}

int getID(char *fname) {
    FILE *p = fopen(fname, "r");
    if(p == NULL) return 2000;
    int id =  2000,tid = 2000,temp = 0;
    char tn[100];

    while(fscanf(p,"%d,%99[^,],%d",&tid,tn,&temp) == 3) {
        if(id < tid) id = tid;
    }

    fclose(p);
    return id+1;
    
}

void readUsers(char *fname) {
    FILE *p = fopen(fname,"r");

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

void updateUser(char *fname,int id,char *uname,int age) {
    FILE *p = fopen(fname,"r");

    if(p == NULL) printf("Internal Server Error");

    int found = 0;



    int tid,tage;
    char tname[100];

    FILE *t = fopen("temp.txt","w");

    while(fscanf(p,"%d,%99[^,],%d",&tid,tname,&tage) == 3) {

        if(id == tid) {
            fprintf(t,"%d,%s,%d\n",id,uname,age);
            found = 1;
        }else fprintf(t,"%d,%s,%d\n",tid,tname,tage);
    }

    fclose(p);
    fclose(t);
    
    if(found == 1) {
        remove(fname);
        rename("temp.txt",fname);

    } else {
        remove("temp.txt");
        printf("id not found ");
    }



}

void deleteUser(char *fname,int id) {
    FILE *p = fopen(fname,"r");

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
        remove(fname);
        rename("temp.txt",fname);

    } else {
        remove("temp.txt");
        printf("id not found ");
    }

}
void createUser(char *fname) {
    
    int tage;
    char tname[100];

 

    // printf("provide details of user eg(Vinit Soni:56) name:age - ");
    // scanf("%99[^:]:%d",tname,tage);

    printf("Enter user name : ");
    scanf(" %99[^\n]",tname); //fgets(tname,100,stdin);  //scanf("%99[^\n]\n",tname);
    printf("Enter user age  : ");
    scanf("%d",&tage);

    //printf("\n you entered %s %d",tname,tage);


    addUser(fname,getID(fname),tname,tage);

}


void editUser(char *fname) {
    int id,age;
    char name[100];

    printf("Enter user ID   : ");
    scanf("%d",&id);
    printf("Enter user name : ");
    scanf(" %99[^\n]",name);  //fgets(name,100,stdin);     //scanf("%99[^\n]\n",name);
    printf("Enter user age  : ");
    scanf("%d",&age);

    updateUser(fname,id,name,age);

}

void removeUser(char *fname) {
    int id;

    printf("Enter user ID   : ");
    scanf("%d",&id);

    deleteUser(fname,id);
}

void meenu(char *fname) {
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
            case 1:createUser(fname);
            break;
            case 2:editUser(fname);
            break;
            case 3:removeUser(fname);
            break;
            case 4:readUsers(fname);
            break;
            default: printf("Invalid option");
        }
    } while(c);
}



int main() {
    
    char *fname = "users.txt";

    meenu(fname);


    //--------Testing code commented ---------//

    // createUser();
    // createUser();

    // addUser(fname,getID(fname),"Ramesh Bhattacharya",34);
    // addUser(fname,getID(fname),"Lakshyadeep Singh",29);
    // addUser(fname,getID(fname),"Veena Shriwastav",67);
    // addUser(fname,getID(fname),"Veena Shriwastav",67);
    // addUser(fname,getID(fname),"Rahim Purohit",44);
    // addUser(fname,getID(fname),"Baal Gangadhar Tilak",134);
    // addUser(fname,getID(fname),"Vinit Acharya",37);

    //readUsers(fname);

    // updateUser(fname,2003,"Vidhya shri",23);
    // deleteUser(fname,2005);

    // printf("\n ------ \n\n");

    // readUsers(fname);
    


    return 0;

}