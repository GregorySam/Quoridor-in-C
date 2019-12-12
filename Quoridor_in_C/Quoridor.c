#include <stdio.h>
#include <string.h>
#include "Quoridor_Functions.h"


struct shell **board_data;		/*hold board information*/
unsigned char **board_display;	/*contains characters in order to properly display the board*/
int new_x,new_y,size=9,p=0;
int col_count=9-2;			/*variable for strategy*/
int row_count=9/2;			/*variable for strategy*/
struct pawn W, B, C;
struct history *history_list;  /*List to hold the history*/
int main(void){
	char *input;
	char *command,*arg1;
	int walls;
	Begin_Game();
	command = NULL;
	do{
		if(input=Input()){		/*if  input is valid*/
        	if ((command=strtok(input," "))!=NULL){		/*remove whitespaces and take the first word*/
        		if (!strcmp(command,"boardsize")){
        			Boardsize();						/*set boardsize*/
        		}
        		else if (!strcmp(command,"clear_board")){	/*free arrays and restart game*/
        			Free_Arrays(size);
        			p=0;
        			Begin_Game();
        			printf("=\n\n");fflush(stdout);
				}
				else if(!strcmp(command,"name")){			/*display name of game*/
					printf("=Quoridor in C\n\n");fflush(stdout);
				}
				else if (!strcmp(command,"walls")){			/*set number of walls*/
					walls=Walls();
					if(walls!=0){
						B.walls=walls;
    					W.walls=walls;
						history_list[p].B=B;
						history_list[p].W=W;
						printf("=\n\n");fflush(stdout);
					}
					else{
						printf("? syntax error\n\n");fflush(stdout);
					}
				}
				else if(!strcmp(command,"genmove")){    /*generate a move*/
					Genmove();
				}
				else if(!strcmp(command,"playmove")){	/*play a move*/
					Playmove();
				}
				else if(!strcmp(command,"playwall")){	/*play a wall*/
					Playwall();
				}
				else if(!strcmp(command,"list_commands")){ /*list known commands*/
					List_Commands();
				}
				else if(!strcmp(command,"undo")){        /*undo command*/
					if(p>0){
						arg1=strtok(NULL," ");
						Undo(arg1);
					}
					else{
						printf("? cannot undo");fflush(stdout);
					}
				}
				else if(!strcmp(command,"known_command")){	/*check if a command is known*/
					arg1=strtok(NULL," ");
					Is_Known(arg1);
				}
				else if(!strcmp(command,"showboard")){	/*display board*/
					Showboard();
				}
				else if (!strcmp(command,"winner")){ /*declare the winner*/
					Winner();
				}
				else if (!strcmp(command,"quit")){ /*quit game*/
					printf("=\n\n");fflush(stdout);
					Free_Arrays(size);
				}
				else{
					printf("? uknown command\n\n");fflush(stdout);
				}
				if(!strcmp(command,"playmove")|| !strcmp(command,"playwall")||!strcmp(command,"genmove")){ /*in case there is a new move add the new board information to history*/
					p++;
					history_list=History_Management();
				}
			}
		}
		else{														/*if input is invalid*/
			printf("? uknown command\n\n");fflush(stdout);
		}
	}while(command==NULL || strcmp(command,"quit"));
	return 0;
}
