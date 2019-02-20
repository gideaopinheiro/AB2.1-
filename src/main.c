#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

char *memUsage(int pid) {
    char bash_cmd[256];
    sprintf(bash_cmd, "ps u %d | awk '{print $5}' | grep -v VSZ", pid);
	char buffer[1000];
	FILE *pipe;
	int len;
	pipe = popen(bash_cmd, "r");

	if (NULL == pipe) {
		perror("pipe");
		exit(1);
	} 
	
	char* mem_usage = fgets(buffer, sizeof(buffer), pipe);
	len = strlen(bash_cmd);
	bash_cmd[len-1] = '\0'; 
	pclose(pipe);
	printf("mem_usage == %s", mem_usage);
    mem_usage[strlen(mem_usage)-1] = '\0';
    return mem_usage;
}

char *ucpUsage(int pid) {
    char bash_cmd[256];
    sprintf(bash_cmd, "ps u %d | awk '{print $3}' | grep -v CPU", pid);
	char buffer[1000];
	FILE *pipe;
	int len;
	pipe = popen(bash_cmd, "r");
	if (NULL == pipe) {
		perror("pipe");
		exit(1);
	} 
	
	char* ucp_usage = fgets(buffer, sizeof(buffer), pipe);
    len = strlen(bash_cmd);
	bash_cmd[len-1] = '\0'; 
	pclose(pipe);
	printf("ucp_usage == %s", ucp_usage);
    ucp_usage[strlen(ucp_usage)-1] = '\0';
    return ucp_usage;
}

void killProcess(int pid){ //MATAR PROCESSO
    char comand[256];
    sprintf(comand, "kill -9 %d", pid);
    system(comand);
}

int main(int argc, char *argv[], char *envp[]){
    pid_t pid;
    pid = fork();  
    char *ucp;
    char *mem;
    char *word;

    FILE *file;
    file = fopen("arquivo_teste2.csv", "w");
    fputs("ucp_usage,mem_usage\n", file);
    if(ucp == mem){
        printf("SIM\n");
    }
    if(pid < 0){
        perror("error ");
        exit(-1);
    }
    else if(pid != 0){ //PAI
        int n = 0;
        int len = sizeof(char) * 10;

        if(strcmp(argv[1],"ucp") == 0){
            while(n<10){
                ucp = ucpUsage(pid);
                sprintf(ucp, "%s,0.0\n", ucp);
                fputs(ucp, file);
                ++n;
                sleep(1);
            }
        }
        else if(strcmp(argv[1], "ucp-mem")==0){
            while(n<10){
                ucp = ucpUsage(pid);
                sprintf(ucp, "%s,",ucp);
                fputs(ucp, file);

                mem = memUsage(pid);
                sprintf(mem, "%s\n", mem);
                fputs(mem, file);

                ++n;
                sleep(1); 
            }
        }
        killProcess(pid);
    }
    else{ //FILHO
        if(strcmp(argv[1],"ucp") == 0){
            while(1);
        }
        else if(strcmp(argv[1], "ucp-mem")==0){
            while(1){
                usleep(1);
                malloc(5);
            }
        }
        _exit(0);
    }
    fclose(file);
    printf("END\n");
    exit(0);
}