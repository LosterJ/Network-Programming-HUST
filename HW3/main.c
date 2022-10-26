#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>

int main(int argc, char *argv[]){
    //kiem tra co phai IP address khong, isIP=0 thi khong phai, isIP=1 thi dung la IP address
    int isIP =1;
    for(int i=0;i<strlen(argv[1]);i++){
        if(((argv[1][i]<'0')||(argv[1][i]>'9'))){
            isIP = 0;
        }
        if (argv[1][i]=='.') isIP=1;
    }
    if (isIP==1){   //thong tin nhap vao la dia chi IP
    //chuyen dot-and-number address sang in_addr
        struct in_addr inAddr;
        if (inet_aton(argv[1],&inAddr))
            printf("The address is valid\n");
        else printf("The address is invalid\n");
        //get hostent by address
        struct hostent *host = gethostbyaddr(&inAddr,4,AF_INET);
        if (host == NULL){
            printf("No Infomation found");
            exit(0);
        }else{
            printf("Official name: %s\n",host->h_name);
            //neu co alias thi in lan luot ra
            if (host->h_aliases[0] == NULL) exit(0);
            else {
                printf("Alias name: \n");
                
                int i = 0;
                while (host->h_aliases[i]!=NULL){
                    printf("%s\n",host->h_aliases[i]);
                    i++;
                    }
                }
            }
    }else{//thong tin nhap vao la ten mien
        //get hostent by domain name
        struct hostent *host = gethostbyname(argv[1]);
        if (host == NULL){
            printf("No Infomation found");
            exit(0);
        }else{
            //lay in_addr tu hostent
            struct in_addr **addrList;
            addrList = (struct in_addr **)host->h_addr_list;
            printf("Official IP: %s\n", inet_ntoa(*addrList[0]));
            if (addrList[1] != NULL) {
                printf("Alias IP:\n");
                for (int i = 1; addrList[i] != NULL; i++) {
                    printf("%s\n", inet_ntoa(*addrList[i]));
                }
            }
        }
    }
}