
//Data criação: 19/02/2019
//Gideão Pinheiro dos Santos Silva
//Gabriel Araujo

#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

//Obtem o consumo de memória do processo
//através do pid e retorna o valor obtido para quem a chamou
char *memUsage(int pid){ 
    char bash_cmd[256];
    sprintf(bash_cmd, "ps u %d | awk '{print $5}' | grep -v VSZ", pid); //formando o comando que para obter o consumo de memória
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
    mem_usage[strlen(mem_usage)-1] = '\0'; //adicionando o fim de string no lugar da quebra de linha
    return mem_usage;
}

//Obtem o consumo de cpu do processo
//e retorna o valor obtido para quem a chamou
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

//Função que mata o processo de determinado pid
void killProcess(int pid){ //Kill the process 
    char comand[256];
    sprintf(comand, "kill -9 %d", pid);
    system(comand); //system call para executar um comando no terminal
}

//Função principal onde os processos estão escritos
int main(int argc, char *argv[], char *envp[]){
    pid_t pid;
    pid = fork();  //cria um processo identico ao processo pai
    char *ucp;
    char *mem;
    char *word;

    FILE *file;
    file = fopen("arquivo_teste.csv", "w"); //abrindo ou criando o arquivo que conterá os valores de consumo
    fputs("ucp_usage,mem_usage\n", file); //inicializando o arquivo com os títulos das colunas

    if(pid < 0){
        perror("error "); 
        exit(-1); //interrompe o programa caso haja algum erro no fork()
    }
    else if(pid != 0){ //Este é o processo pai
        int n = 0;
        int len = sizeof(char) * 10;

        if(strcmp(argv[1],"ucp") == 0){ //Quando o argumento passado na execução do programa é ucp entra no loop
            while(n<10){
                //Chama a função que obtem os valores de consumo de ucp
                //guarda os valores com fputs() e quando n==10 sai do loop;
                ucp = ucpUsage(pid);
                sprintf(ucp, "%s,0.0\n", ucp);
                fputs(ucp, file);
                ++n;
                sleep(1);
            }
        }
        else if(strcmp(argv[1], "ucp-mem")==0){ //Quando o argumento passado na execução do programa é ucp-mem
            while(n<10){
                //Chama a função que obtem os valores de consumo de ucp
                //guarda os valores com fputs() e quando n == 10 sai do loop
                ucp = ucpUsage(pid);
                sprintf(ucp, "%s,",ucp);
                fputs(ucp, file);

                //Chama a função que obtem os valores de consumo de memória
                mem = memUsage(pid);
                sprintf(mem, "%s\n", mem);
                fputs(mem, file);

                ++n;
                sleep(1); 
            }
        }
        killProcess(pid);
    }
    else{ //child process
        if(strcmp(argv[1],"ucp") == 0){ // Quando o argumento passado é ucp
            //execução de loop infinito para forçar a cpu
            while(1);
        }
        else if(strcmp(argv[1], "ucp-mem")==0){ // Quando o argumento passado é ucp-mem
            //execução de loop infinito
            while(1){
                usleep(1); // Esperando 1 milissegundo para não sobrecarregar a memória
                malloc(5); // Alocando memória
            }
        }
        _exit(0); //Retornando o controle do processo para o processo pai
    }
    fclose(file); //Fechando o arquivo que salva os valores obtidos 
    printf("END\n"); //Mensagem de encerramento
    exit(0);
}