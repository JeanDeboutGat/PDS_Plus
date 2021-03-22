#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
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
	assert(argc >= 2);
	int prevfd = 64; 
	struct command_set cm_s = parse_Command(argc, argv);
	dup2(0, prevfd);
	for(int i=0 ; i< cm_s.nb_command ; i++){
		int  pid;
		int  fd[2];
		pipe(fd);
		switch(pid=fork()){
			case -1 : return -1;
			case 0 : {
				if(i+1 <  cm_s.nb_command) {
					dup2(fd[1],1);
				}
				dup2(prevfd, 0);
				close(fd[0]);
				close(fd[1]);
				execlp(cm_s.cmd[i].name, cm_s.cmd[i].name , cm_s.cmd[i].argument , NULL);
				exit(EXIT_FAILURE);
			}
			default : {
				close(prevfd) ;
				dup2(fd[0] , prevfd);
				close(fd[1]) ;
				close(fd[0]);
			}
			wait(NULL);
		}
	}


	
	return 0; 
}


/*




*/