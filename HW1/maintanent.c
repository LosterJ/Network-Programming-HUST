#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int dolog = 0;
char logUser[30];

typedef struct node{
    char username[40];
    char password[50];
    int status;
    struct node *pNext;
} NODE;

NODE* createNode(char* username,char* password,int status){
    NODE *newNode=(NODE*)malloc(sizeof(NODE));
    strcpy(newNode->username,username);
    strcpy(newNode->password,password);
    newNode->status = status;
    newNode->pNext = NULL;
    return newNode;
}

NODE* addNode(NODE *head,NODE *newNode){
    
}

NODE* loadfile(char *filename){
    NODE *head=NULL;
    NODE *newNode;
    FILE* fptr = fopen(filename,"r");
    if (fptr==NULL){
        printf("Can not open this file"); exit(1);
    }
    char username[20];
    do {
        char username[30], password[30];
        int status;
        fscanf(fptr,"%s %s %d",username,password,&status);
        if(head==NULL){
            head=createNode(username,password,status);
        } else {
            newNode = createNode(username,password,status);
            newNode->pNext = head;
            head=newNode;
        }
    } while(getc(fptr)!=EOF);
    return head;
    fclose(fptr);
}

void changefile(NODE* head, char *filename){
    FILE *fptr = fopen(filename,"w+");
    if (fptr == NULL){
        printf("Change file error!!!"); return;
    }
    while(NULL!=head){
        fprintf(fptr,"%s %s %d\n\n",head->username,head->password,head->status);
        head = head->pNext;
        if (head->pNext == NULL){
            fprintf(fptr,"%s %s %d",head->username,head->password,head->status);
            break;
        }
    }
    fclose(fptr);
}

NODE* func1(NODE *head,char *filename){
    char user_name[30];
    printf("User_name: "); scanf("%s",user_name); fflush(stdin);
    NODE *checkNode = head;
    while(NULL!=checkNode){
        int checkindex = strcmp(user_name,checkNode->username);
        if (checkindex == 0){
            printf("This username already exists! Do you want create by other name?\n Y/N\n");
            char Y_N; scanf("%c",&Y_N);
            if ((Y_N == 'Y')||(Y_N == 'y')) head = func1(head,filename);
            return head;
        }
        checkNode=checkNode->pNext;
    }
    char password[40];
    printf("Password: "); scanf("%s",password);
    checkNode = createNode(user_name,password,1);
    checkNode->pNext = head; head = checkNode;

    FILE* fptr = fopen(filename,"a");
    if (fptr==NULL){
        printf("Something wrong when registering!!!"); return head;
    }
    fprintf(fptr,"\n\n%s %s %d",user_name,password,1);
    fclose(fptr);

    return head;
}

int func2(NODE *head,char *filename){
    char user_name[30];
    printf("User_name: "); scanf("%s",user_name); fflush(stdin);
    NODE *checkNode = head;
    while(NULL!=checkNode){
        int checkindex = strcmp(user_name,checkNode->username);
        if (checkindex == 0){
            //Co ton tai, tiep theo kiem tra password
            for (int i=3;i>0;i--){
                char password[40];
                printf("Password: "); scanf("%s",password);
                int checkindex = strcmp(password,checkNode->password);
                if (checkindex == 0){
                    printf("Logged in successfully\n");
                    dolog = 1;
                    strcpy(logUser,checkNode->username);
                    return 0;
                } else {
                    printf("Wrong password!!!\nYou have %d tries(try) left!\n",i-1);
                }
            }
            printf("Your account has been locked!\n");
            checkNode->status=0;
            changefile(head,filename);
            return 1;
        }
        checkNode=checkNode->pNext;
    }
    printf("Invalid username!!\n");
    return 1;
}

int func3(NODE *head){
    char username[30];
    if (dolog != 1) {
        printf("You do not login!");
        return 1;
    }
    printf("Find the username: "); scanf("%s",username); fflush(stdin);
    NODE *checkNode = head;
    while(NULL!=checkNode){
        int checkindex = strcmp(username,checkNode->username);
        if (checkindex == 0){
            char active[]="Active",blocked[]="Blocked";
            if (checkNode->status){
                printf("user: %s, status: %s\n",username,active);
                return 0;
            } else {
                printf("user: %s, status: %s\n",username,blocked);
                return 0;
            }
        }
        checkNode=checkNode->pNext;
    }
    printf("User does not exit!\n");
    return 0;
}

int func4(){
    char username[30];
    if (dolog != 1) {
        printf("You do not login!\n");
        return 1;
    }
    printf("Your username: "); scanf("%s",username);
    if (strcmp(logUser,username)==0){
        dolog = 0;
        printf("Sign out successful\n");
        exit(0);
    } else {
        printf("It's not your username\n");
    }

}

void menu(NODE *head,char *filename){
    printf("-----------------------\n");
    printf("USER MANAGEMENT PROGRAM\n");
    printf("-----------------------\n");
    printf("1.Register\n2.Sign in\n3.Search\n4.Sign out\n\n");
    printf("Your choice (1-4, other to quit):");
    int index;
    scanf("%d",&index);
    switch (index){
        case 1: 
        printf("-----------------------\nRegister function\n");
        head = func1(head,filename); break;
        case 2: 
        printf("-----------------------\nSign_in function\n");
        func2(head,filename); break;
        case 3: func3(head); break;
        case 4: func4(); break;
        default: exit(1);
    }
    menu(head,filename);
}

int main(){
    NODE *head=NULL,*newNode;
    char filename[]="account.txt";
    head=loadfile(filename);
    menu(head,filename);
}