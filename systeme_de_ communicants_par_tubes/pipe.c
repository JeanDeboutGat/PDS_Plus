#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>  


struct commande 
{ 
   char* name; 
   char* argument;  
};



struct command_set
{ 
   struct commande *cmd;  
   int nb_command; 
};


int nb_command(int argc ,  char* argv[]){
	int count  = 1 ;
	for(int i = 1 ; i< argc ; i++){
		if(strcmp(argv[i] , "to") ==  0){
			count++;
		}
	}
	return count;
}

char* strcat_with_space(char * base, char* ajout){
	if(base[0] != 0){
    	strcat( base, " " );
    }
    strcat( base, ajout );
    return base;
}



struct command_set parse_Command(int argc ,  char* argv[]){
	struct command_set cm;
	cm.nb_command = nb_command(argc ,argv);
	cm.cmd = malloc((sizeof(struct commande))*cm.nb_command);
	cm.cmd[0].name = argv[1];
	for(int i = 0 ; i< cm.nb_command ; i++){
		cm.cmd[i].argument = malloc(500);
	}

	int i = 2 ;
	int y = 0;
	while(i < argc){
		if(strcmp(argv[i] , "to") == 0){
			y++;
			cm.cmd[y].name = argv[i+1];
			i += 2 ;
		}else{
			strcat_with_space(cm.cmd[y].argument , argv[i]);
			i++;
		}
	}
	return cm;
}







int main(int argc , char* argv[]){
	assert(argc >= 3);
	int pid ;
	int i;
    int fd[2] ;
	pipe(fd);
	struct command_set cm_s = parse_Command(argc, argv);
	switch(pid=fork())
	{
		case -1 : return -1 ;
		case 0  : {			
			dup2(fd[1],1); 
			close(fd[0]);
			close(fd[1]);
			execlp(cm_s.cmd[0].name, cm_s.cmd[0].name , cm_s.cmd[0].argument , NULL);
			exit(EXIT_FAILURE);
		}
		default : {
			dup2(fd[0],0); 
			close(fd[1]) ;
			close(fd[0]) ;
			execlp(cm_s.cmd[1].name , cm_s.cmd[1].name , cm_s.cmd[1].argument,NULL) ;
			exit(EXIT_FAILURE);
		}
	}
	
	return 0; 
}
