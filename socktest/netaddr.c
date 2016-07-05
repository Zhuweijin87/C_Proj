#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>

/*  struct hostent {
        char *h_name;       //主机的正式名称
        char **h_aliases;   //别名列表
        int h_addrtype;     //主机地址类型
        int h_length;       //地址长度
        char **h_addr_list; //地址列表
    }
*/
int main()
{
    char   *netname = "www.sina.com.cn";
    struct hostent *host = NULL;
    
    host = gethostbyname(netname);
    
    char    str[30];
    if(host){
        int  i = 0;
        fprintf(stderr, "host origin name: %s\n", netname);
        fprintf(stderr, "host name: %s\n", host->h_name);
        fprintf(stderr, "host address type: %s\n", host->h_addrtype == AF_INET? "AF_INET" : "AF_INET6");
        fprintf(stderr, "host address length: %d\n", host->h_length);

        for(i=0; ; i++){
            if(host->h_aliases[i] != NULL){
                fprintf(stderr, "host alias: %s:%s\n", host->h_aliases[i], host->h_addr_list[1]);
            }
            else
                break;
        }
        
        for(i=0; ; i++){
            if(host->h_addr_list[i] != NULL)
                fprintf(stderr, "host alias ip: %s\n",inet_ntop(host->h_addrtype,host->h_addr_list[i], str, 30));
            else
                break;
        }
    }

    return 0;
}

