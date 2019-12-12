#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
struct shell {
	char isblocked_a;									/*   shell:  __a_ each side represents if there is or not a wall*/
	char isblocked_b;									/*			|		|	f describes wether there is or there is not 'W' or'B' in the shell*/
	char isblocked_c;									/*	  d		|	f	|b */
	char isblocked_d;									/*			|_______|		*/
	char pawn;												/*			c           */
};
extern struct shell **board_data;			/*holds board information*/
extern unsigned char **board_display;		/*array with characters to display board*/
extern int new_x, new_y, size , p ;			
extern int col_count ;
extern int row_count ;
struct pawn {								/*current pawn informations*/
	int x;
	int y;
	int walls;
};
extern struct pawn W, B, C;					
struct history {
	int col_count;
	int row_count;
	struct shell **board_data;
	unsigned char **board_display;
	struct pawn B,W,C;
};
extern struct history *history_list;

void List_Commands(void) {			/*commands list*/
	int i;
	char *commands[] = { "-name\n","-known_command 'name of the command'\n","-list_commands\n","-quit\n",
		"-boardsize 'size'\n","-clear board\n","-walls 'number_of_walls'\n",
		"-playmove 'color and vertex of the move'\n","-playwall 'Color , vertex and orientation of the wall'\n",
		"-genmove 'Color for which to generate a move'\n","-undo 'times(optional)'\n","-winner\n","-showboard\n" };
	for (i = 0;i<13;i++) {
		printf("%s", commands[i]);fflush(stdout);
	}
	printf("=\n\n");fflush(stdout);
}
unsigned char **Display_Board(void) {				/*create array with characters to display board*/
	unsigned char col;
	int i, j;
	board_display = (unsigned char**)malloc((2 * size + 3)*sizeof(unsigned char*));
	for (i = 0;i<(2 * size + 3);i++) {
		board_display[i] = (unsigned char*)malloc((4 * size + 2)*sizeof(unsigned char));
	}
	if (board_display == NULL) {
		printf("? Error alocating memory\n\n");fflush(stdout);
		exit(0);
	}
	col = 'A';
	for (i = 0;i<(2 * size + 3);i++) {
		for (j = 0;j<(4 * size + 2);j++) {
			if (j == 4 * size + 1) {
				board_display[i][j] = 0;
			}
			else if (i == 0 || (i == 2 * size + 2)) {
				if (j % 4 == 2 && j <= 4 * size) {
					if (col == ('A' + size)) {
						col = 'A';
					}
					board_display[i][j] = col;
					col++;
				}
				else if (j <= 4 * size) {
					board_display[i][j] = ' ';
				}
			}
			else if (i % 2 == 1) {
				if (j <= 4 * size) {
					if (j % 4 == 0) {
						board_display[i][j] = '+';
					}
					else {
						board_display[i][j] = '-';
					}
				}
			}
			else {
				if (j <= 4 * size) {
					if (j % 4 == 0) {
						board_display[i][j] = '|';
					}
					else {
						board_display[i][j] = ' ';
					}
				}
			}
		}
		if (size % 2 == 1) {
			board_display[2][2 * size] = 'B';
			board_display[2 * size][2 * size] = 'W';
		}
		else {
			board_display[2][2 * size + 2] = 'B';
			board_display[2 * size][2 * size + 2] = 'W';
		}
	}
	return board_display;
}
int Allow_Move(struct pawn C) {		/*check if a pawn move is allowed*/
	if (new_x<size && new_x>-1 && new_y<size && new_y>-1) {
		if (new_x - C.x == 1 && new_y == C.y && board_data[new_x][new_y].isblocked_a == 0 && board_data[new_x][new_y].pawn == 0) {
			return 1;
		}
		else if (C.x - new_x == 1 && new_y == C.y && board_data[new_x][new_y].isblocked_c == 0 && board_data[new_x][new_y].pawn == 0) {
			return 1;
		}
		else if (new_y - C.y == 1 && new_x == C.x && board_data[new_x][new_y].isblocked_d == 0 && board_data[new_x][new_y].pawn == 0) {
			return 1;
		}
		else if (C.y - new_y == 1 && new_x == C.x && board_data[new_x][new_y].isblocked_b == 0 && board_data[new_x][new_y].pawn == 0) {
			return 1;
		}
		else if (new_x - C.x == 2 && new_y == C.y && board_data[new_x][new_y].isblocked_a == 0 && board_data[new_x - 1][new_y].pawn != 0) {
			return 1;
		}
		else if (C.x - new_x == 2 && new_y == C.y && board_data[new_x][new_y].isblocked_c == 0 && board_data[new_x + 1][new_y].pawn != 0) {
			return 1;
		}
		else if (new_y - C.y == 2 && new_x == C.x && board_data[new_x][new_y].isblocked_d == 0 && board_data[new_x][new_y - 1].pawn != 0) {
			return 1;
		}
		else if (C.y - new_y == 2 && new_x == C.x && board_data[new_x][new_y].isblocked_b == 0 && board_data[new_x][new_y + 1].pawn != 0) {
			return 1;
		}
		else if ((C.y - new_y == 1 || new_y - C.y == 1) && (C.x - new_x == 1) && (board_data[C.x - 1][C.y].isblocked_a == 1 || new_x == 0) && board_data[C.x - 1][C.y].pawn != 0) {
			return 1;
		}
		else if ((new_y - C.y == 1) && (C.x - new_x == 1 || new_x - C.x == 1) && (board_data[C.x][C.y + 1].isblocked_b == 1 || new_y == size - 1) && board_data[C.x][C.y + 1].pawn != 0) {
			return 1;
		}
		else if ((new_x - C.x == 1) && (C.y - new_y == 1 || new_y - C.y == 1) && (board_data[C.x + 1][C.y].isblocked_c == 1 || new_x == size - 1) && board_data[C.x + 1][C.y].pawn != 0) {
			return 1;
		}
		else if ((C.y - new_y == 1) && (C.x - new_x == 1 || new_x - C.x == 1) && (board_data[C.x][C.y - 1].isblocked_d == 1 || new_y == 0) && board_data[C.x][C.y - 1].pawn != 0) {
			return 1;
		}
		else {
			return 0;
		}
	}
	else {
		return 0;
	}
}
void Do_Move(char color) {			/*do a pawn move*/
	board_data[C.x][C.y].pawn = 0;
	board_display[2 * C.x + 2][4 * C.y + 2] = ' ';
	C.x = new_x;
	C.y = new_y;
	board_data[new_x][new_y].pawn = color;
	board_display[2 * new_x + 2][4 * new_y + 2] = color;
}
int Convert_Vertex_to_Coordinates(char *arg2) {
	int i = 0;
	if (arg2 != NULL) {
		if ((arg2[0] <= 90 && arg2[0] >= 65) || (arg2[0] >= 97 && arg2[0] <= 122)) {
			do {
				i++;
			} while (isdigit(arg2[i]));
			if (i > 2) {
				sscanf(arg2 + 1, "%d", &new_x);
				new_x = size - new_x;
			}
			else if (i > 1) {
				sscanf(arg2 + 1, "%d", &new_x);
				new_x = size - new_x;
			}
			else {
				return 0;
			}
			if (arg2[0] <= 90) {
				new_y = arg2[0] - 'A';
			}
			else if (arg2[0] >= 97) {
				new_y = arg2[0] - 'a';
			}
			return 1;
		}
		else {
			return 0;
		}
	}
	else {
		return 0;
	}
}
struct shell **Create_Board(void) {		/*Creates array to hold board information*/
	int i, j;
	board_data = (struct shell**)malloc((size)*sizeof(struct shell*));
	for (i = 0;i<(size);i++) {
		board_data[i] = (struct shell*)malloc((size)*sizeof(struct shell));
	}
	if (board_data == NULL) {
		printf("? Error alocating memory\n\n");fflush(stdout);
		exit(0);
	}
	for (i = 0;i<size;i++) {
		for (j = 0;j<size;j++) {
			board_data[i][j].isblocked_a = 0;
			board_data[i][j].isblocked_b = 0;
			board_data[i][j].isblocked_c = 0;
			board_data[i][j].isblocked_d = 0;
			board_data[i][j].pawn = 0;
			W.walls = 10;
			B.walls = 10;
		}
	}
	W.walls = 10;
	B.walls = 10;
	if (size % 2 == 1) {
		board_data[0][(size - 1) / 2 + 1 / 2].pawn = 'B';
		board_data[size - 1][(size - 1) / 2 + 1 / 2].pawn = 'W';
		W.x = size - 1;
		W.y = (size - 1) / 2 + 1 / 2;
		B.x = 0;
		B.y = (size - 1) / 2 + 1 / 2;
	}
	else {
		board_data[0][(size - 1) / 2].pawn = 'B';
		board_data[size - 1][(size - 1) / 2].pawn = 'W';
		W.x = size - 1;
		W.y = (size - 1) / 2;
		B.x = 0;
		B.y = (size - 1) / 2;
	}
	return board_data;
}
int Allow_Wall_Placement(char *arg3, struct pawn C) {		/*Check if a wall is allowed to be placed*/
	int allow = 1;
	int i, j;
	if (arg3 != NULL) {
		if (!strcmp(arg3, "v") || !strcmp(arg3, "vertical") || !strcmp(arg3, "h") || !strcmp(arg3, "horizontal")) {
			if ((!strcmp(arg3, "h") || !strcmp(arg3, "horizontal")) && new_y>-1 && new_y<(size - 1) && new_x>-1 && new_x<(size - 1) && C.walls>0) {
				for (j = 4 * new_y + 1;j <= 4 * new_y + 7;j++) {
					if (board_display[2 * new_x + 3][j] == '=' || board_display[2 * new_x + 3][j] == 186) {
						return 0;
					}
				}
				return 1;
			}
			else if ((!strcmp(arg3, "v") || !strcmp(arg3, "vertical")) && new_y<(size - 1) && new_x<(size - 1) && C.walls>0) {
				for (i = 2 * new_x + 2;i <= 2 * new_x + 4;i++) {
					if (board_display[i][4 * new_y + 4] == 186 || board_display[i][4 * new_y + 4] == '=') {
						return 0;
					}
				}
				return 1;
			}
			else {
				return 0;
			}
		}
		else {
			return 2;
		}
	}
	else {
		return 2;
	}
}
void Wall_Placement(char *arg3) {		/*Place a wall*/
	int i, j;
	if (!strcmp(arg3, "h") || !strcmp(arg3, "horizontal")) {
		board_data[new_x][new_y].isblocked_c = 1;
		board_data[new_x + 1][new_y].isblocked_a = 1;
		board_data[new_x][new_y + 1].isblocked_c = 1;
		board_data[new_x + 1][new_y + 1].isblocked_a = 1;
		for (j = 4 * new_y + 1;j <= 4 * new_y + 7;j++) {
			board_display[2 * new_x + 3][j] = '=';
		}
		C.walls--;
	}
	else if (!strcmp(arg3, "v") || !strcmp(arg3, "vertical")) {
		board_data[new_x][new_y].isblocked_b = 1;
		board_data[new_x][new_y + 1].isblocked_d = 1;
		board_data[new_x + 1][new_y].isblocked_b = 1;
		board_data[new_x + 1][new_y + 1].isblocked_d = 1;
		for (i = 2 * new_x + 2;i <= 2 * new_x + 4;i++) {
			board_display[i][4 * new_y + 4] = 186;
		}
		C.walls--;
	}
}
char *Input(void) {		/*Check if input is valid*/
	unsigned char *str;
	unsigned int i, j;
	str = malloc(101);
	fgets(str, 100, stdin);
	str[strlen(str) - 1] = 0;
	if (str != NULL) {
		for (i = 0;i<strlen(str);i++) {
			if (str[i]<32) {
				str[i] = ' ';
			}
		}
		for (i = 0;i<strlen(str);i++) {
			if (str[i] == '#') {
				j = i;
				break;
			}
		}
		if (str[i] == '#') {
			for (j = i;j<strlen(str);j++) {
				str[j] = 0;
			}
		}
		return str;
	}
	else {
		return 0;
	}
}
void Is_Known(char *arg1) {		/*Check if a command is known*/
	int i, f = 0;
	char *commands[] = { "name","known_command","list_commands","quit","boardsize","clear board_data","walls",
		"playmove","playwall","genmove","undo","winner","showboard" };
	for (i = 0;i<13;i++) {
		if (!strcmp(arg1, commands[i])) {
			f = 1;
			break;
		}
	}
	if (f == 1) {
		printf("=true\n\n");fflush(stdout);
	}
	else {
		printf("=false\n\n");fflush(stdout);
	}
}
int Walls(void) {		/*Wall command*/
	char *arg1;
	int walls;
	arg1 = strtok(NULL, " ");
	if (arg1 != NULL) {
		sscanf(arg1, "%d", &walls);
	}
	if (walls>0) {
		return walls;
	}
	else {
		return 0;
		printf("? syntax error\n\n");fflush(stdout);
	}
}
struct history *History_Management(void) {		/*manage history*/
	int i, j;
	if (history_list == NULL) {
		history_list = (struct history *)malloc((p + 1)*sizeof(struct history));
	}
	else {
		history_list = (struct history *)realloc((void *)history_list, (p + 1)*sizeof(struct history));
	}
	history_list[p].board_data = (struct shell**)malloc((size)*sizeof(struct shell*));
	for (i = 0;i<size;i++) {
		history_list[p].board_data[i] = (struct shell*)malloc((size)*sizeof(struct shell));
	}
	for (i = 0;i<size;i++) {
		for (j = 0;j<size;j++) {
			history_list[p].board_data[i][j] = board_data[i][j];
		}
	}
	history_list[p].board_display = (unsigned char**)malloc((2 * size + 3)*sizeof(unsigned char*));
	for (i = 0;i<2 * size + 3;i++) {
		history_list[p].board_display[i] = (unsigned char*)malloc((4 * size + 6)*sizeof(unsigned char));
	}
	for (i = 0;i<(2 * size + 3);i++) {
		for (j = 0;j<(4 * size + 6);j++) {
			history_list[p].board_display[i][j] = board_display[i][j];
		}
	}
	history_list[p].B = B;
	history_list[p].W = W;
	history_list[p].col_count = col_count;
	history_list[p].row_count = row_count;
	return history_list;
}
void Undo(char *arg1) {		/*undo move*/
	int times, i = -1, j;
	if (arg1 == NULL) {
		times = 1;
	}
	else {
		do {
			i++;
		} while (isdigit(arg1[i]) && arg1[i] != 0);
		if (i == 0 && isdigit(arg1[i])) {
			times = 0;
		}
		else if (isdigit(arg1[i - 1])) {
			sscanf(arg1, "%d", &times);
		}
		else {
			printf("? syntax error\n\n");fflush(stdout);
			return;
		}
	}
	if (times <= p) {
		p = p - times;
		history_list = (struct history *)realloc(history_list, (p + 1)*sizeof(struct history));
		for (i = 0;i<size;i++) {
			for (j = 0;j<size;j++) {
				board_data[i][j] = history_list[p].board_data[i][j];
			}
		}
		for (i = 0;i<2 * size + 3;i++) {
			for (j = 0;j<4 * size + 6;j++) {
				board_display[i][j] = history_list[p].board_display[i][j];
			}
		}
		W = history_list[p].W;
		B = history_list[p].B;
		row_count = history_list[p].row_count;
		col_count = history_list[p].col_count;
		printf("=\n\n");fflush(stdout);
	}
	else {
		printf("? cannot undo\n\n");fflush(stdout);
	}
}
void Winner(void) {	/*declare winner*/
	if (W.x == 0) {
		printf("=true White\n\n");fflush(stdout);
	}
	else if (B.x == size - 1) {
		printf("=true Black\n\n");fflush(stdout);
	}
	else {
		printf("=false\n\n");fflush(stdout);
	}
}
void Free_Arrays(int tmp_size) {		/*function to free arrays*/
	int i, j;
	if (board_data != NULL) {
		for (i = 0;i < tmp_size;i++) {
			free((void *)board_data[i]);
		}
		free(board_data);
	}
	if (history_list != NULL) {
		for (j = 0;j <= p;j++) {
			for (i = 0;i<tmp_size;i++) {
				free((void *)history_list[j].board_data[i]);
			}
		}
		for (j = 0;j <= p;j++) {
			for (i = 0;i<2 * tmp_size + 3;i++) {
				free((void *)history_list[j].board_display[i]);
			}
		}
		free(history_list);
		history_list = NULL;
	}
	if (board_display != NULL) {
		for (i = 0;i<2 * tmp_size + 3;i++) {
			free(board_display[i]);
		}
		free(board_display);
	}
}
void Show_Move(void) {   /*Show the gnerated move*/
	char arg4;
	if (new_y <= 'Z' - 'A') {
		arg4 = new_y + 'A';
	}
	printf("=%c%d\n\n", arg4, size - new_x);fflush(stdout);
}
void Show_Wall_Placement(char *arg3) {	/*Sow generated wall Placement*/
	char arg4;
	if (new_y <= 'Z' - 'A') {
		arg4 = new_y + 'A';
	}
	printf("=%c%d %s\n\n", arg4, size - new_x, arg3);fflush(stdout);

}
int Strategy_Case(char Blocked_side, char color, unsigned int dir) {  /*Strategy selection according to color and side*/
	int i, left_open = 0, right_open = 0;
	if (Blocked_side == 0 && board_data[C.x + dir][C.y].pawn == 0 && C.x != size / 2) {
		new_x = C.x + dir;
		new_y = C.y;
		Do_Move(color);
		Show_Move();
		return 1;
	}
	else if (Blocked_side == 0 && board_data[C.x + dir][C.y].pawn != 0 && C.x != size / 2) {
		if ((board_data[C.x + dir][C.y].isblocked_a == 1 || board_data[C.x + dir][C.y].isblocked_c == 1 || C.x + dir == 0 || C.x + dir == size - 1) && board_data[C.x + dir][C.y].pawn != 0) {
			new_x = C.x + dir;
			new_y = C.y - 1;
		}
		else {
			new_x = C.x + 2 * dir;
			new_y = C.y;
		}
		Do_Move(color);
		Show_Move();
		return 1;
	}
	else if (C.x == size / 2) {
		if (col_count>0) {
			if (history_list[0].C.x==size-1) {
				new_x = C.x;
			}
			else {
				new_x = C.x - 1;
			}
			new_y = col_count;
			if (Allow_Wall_Placement("h", C)) {
				Wall_Placement("h");
				Show_Wall_Placement("h");
				col_count -= 2;
				return 1;
			}
			else {
				return 0;
			}
		}
		else if (row_count > 0 && row_count<size - 1) {
			if(history_list[0].C.x==0){
				new_x = row_count;
			}
			else{
				new_x = row_count + dir;
			}
			new_y = col_count + 2;
			if (Allow_Wall_Placement("v", C)) {
				Wall_Placement("v");
				Show_Wall_Placement("v");
				row_count = row_count + 2 * dir;
				return 1;
			}
			else {
				return 0;
			}
		}
		else {
			new_x = C.x + dir;
			new_y = C.y;
			Do_Move(color);
			Show_Move();
		}
		return 1;
	}
	else if (C.y != size - 1 && C.y != 0) {
		if (Blocked_side == 1 && (board_data[C.x][C.y].isblocked_b == 0 || board_data[C.x][C.y].isblocked_d == 0) && (board_data[C.x][C.y + 1].isblocked_a == 1 || board_data[C.x][C.y - 1].isblocked_a == 1)) {
			for (i = C.y;i <= size - 1;i++) {
				if (Blocked_side==0) {
					right_open++;
				}
			}
			for (i = C.y;i >= 0;i--) {
				if (Blocked_side==0) {
					left_open++;
				}
			}
			if (right_open % 2 == 0 && right_open != 0) {
				new_x = C.x;
				new_y = C.y - 1;
				Do_Move(color);
				Show_Move();
			}
			else if (right_open != 0) {
				new_x = C.x;
				new_y = C.y + 1;
				Do_Move(color);
				Show_Move();
			}
			return 1;
		}
		else {
			return 0;
		}
	}
	else {
		return 0;
	}

}
int Strategy(char color) { /*Strategy*/
	if (history_list[0].C.x==size-1) {
		if (Strategy_Case(board_data[C.x][C.y].isblocked_a,color, -1)) {
			return 1;
		}
		else {
			return 0;
		}

	}
	else if (history_list[0].C.x == 0) {
		if (Strategy_Case(board_data[C.x][C.y].isblocked_c,color, 1)) {
			return 1;
		}
		else {
			return 0;
		}
	}
	else {
		return 0;
	}
}
void Rand_move(char color) { /*generate random move or apply strategy*/
	int i, r_shell[4], move, way;
	char *arg3;
	char arg4;
	i = 0;
	srand((unsigned int)time(NULL));
	if (!Strategy(color)) {
		if (C.walls == 0) {
			move = 1;
		}
		else {
			move = rand() % 2;
		}
		if (move == 1) {
			if (board_data[C.x][C.y].isblocked_a == 0 && C.x != 0) {
				r_shell[i] = 1;
				i++;
			}
			if (board_data[C.x][C.y].isblocked_b == 0 && C.y != (size - 1)) {
				r_shell[i] = 2;
				i++;
			}
			if (board_data[C.x][C.y].isblocked_c == 0 && C.x != (size - 1)) {
				r_shell[i] = 3;
				i++;
			}
			if (board_data[C.x][C.y].isblocked_d == 0 && C.y != 0) {
				r_shell[i] = 4;
				i++;
			}
			i = rand() % i;
			if (r_shell[i] == 1) {
				new_x = C.x - 1;
				new_y = C.y;
				if (board_data[new_x][C.y].pawn != 0 && new_x != 0) {
					new_x = new_x - 1;
				}
			}
			else if (r_shell[i] == 2) {
				new_y = C.y + 1;
				new_x = C.x;
				if (board_data[C.x][new_y].pawn != 0 && new_y != (size - 1)) {
					new_y = new_y + 1;
				}
			}
			else if (r_shell[i] == 3) {
				new_x = C.x + 1;
				new_y = C.y;
				if (board_data[new_x][C.y].pawn != 0 && new_x != (size - 1)) {
					new_x = new_x + 1;
				}
			}
			else if (r_shell[i] == 4) {
				new_y = C.y - 1;
				new_x = C.x;
				if (board_data[C.x][new_y].pawn != 0 && new_y != 0) {
					new_y = new_y - 1;
				}
			}
			if (new_y <= 'Z' - 'A') {
				arg4 = new_y + 'A';
			}
			Do_Move(color);
			printf("=%c%d\n\n", arg4, size - new_x);fflush(stdout);
		}
		else if (move == 0) {
			do {
				new_x = rand() % (size - 1);
				new_y = rand() % (size - 1);
				way = rand() % 2;
				if (way == 1) {
					arg3 = "h";
				}
				else {
					arg3 = "v";
				}
			} while (!Allow_Wall_Placement(arg3, C));
			if (new_y <= 'Z' - 'A') {
				arg4 = new_y + 'A';
			}
			printf("=%c%d %s\n\n", arg4, size - new_x, arg3);fflush(stdout);
			Wall_Placement(arg3);
		}
	}
}
void Playwall(void) {	/*play a wall*/
	char *arg1, *arg2, *arg3;
	arg1 = strtok(NULL, " ");
	if (arg1 != NULL) {
		if (!strcmp(arg1, "white") || !strcmp(arg1, "W")) {
			arg2 = strtok(NULL, " ");
			C = W;
			arg3 = strtok(NULL, " ");
			if (Convert_Vertex_to_Coordinates(arg2) && Allow_Wall_Placement(arg3, C) == 1) {
				Wall_Placement(arg3);
				W = C;
				printf("=\n\n");fflush(stdout);
			}
			else if (!Convert_Vertex_to_Coordinates(arg2) || Allow_Wall_Placement(arg3, C) == 2) {
				printf("? syntax error\n\n");fflush(stdout);
			}
			else {
				printf("? illegal move\n\n");fflush(stdout);
			}
		}
		else if (!strcmp(arg1, "black") || !strcmp(arg1, "B")) {
			arg2 = strtok(NULL, " ");
			C = B;
			arg3 = strtok(NULL, " ");
			if (Convert_Vertex_to_Coordinates(arg2) && Allow_Wall_Placement(arg3, C) == 1) {
				Wall_Placement(arg3);
				B = C;
				printf("=\n\n");fflush(stdout);
			}
			else if (!Convert_Vertex_to_Coordinates(arg2) || Allow_Wall_Placement(arg3, C) == 2) {
				printf("? syntax error\n\n");fflush(stdout);
			}
			else {
				printf("? illegal move\n\n");fflush(stdout);
			}
		}
		else {
			printf("? syntax error\n\n");fflush(stdout);
		}
	}
	else {
		printf("? syntax error\n\n");fflush(stdout);
	}
}
void Genmove(void) {	/*genmove command*/
	char *arg1, color;
	arg1 = strtok(NULL, " ");
	if (!strcmp(arg1, "white") || !strcmp(arg1, "W")) {
		color = 'W';
		history_list[0].C = history_list[0].W;
		C = W;
		Rand_move(color);
		W = C;
	}
	else if (!strcmp(arg1, "black") || !strcmp(arg1, "B")) {
		color = 'B';
		history_list[0].C = history_list[0].B;
		C = B;
		Rand_move(color);
		B = C;
	}
	else {
		printf("? syntax error\n\n");fflush(stdout);
	}
}
void Playmove(void) {		/*Playmove command*/
	char *arg1, *arg2;
	arg1 = strtok(NULL, " ");
	if (arg1 != NULL) {
		if (!strcmp(arg1, "white") || !strcmp(arg1, "W")) {
			arg2 = strtok(NULL, " ");
			C = W;
			if (Convert_Vertex_to_Coordinates(arg2) && Allow_Move(C)) {
				Do_Move('W');
				W = C;
				printf("=\n\n");fflush(stdout);
			}
			else if (!Convert_Vertex_to_Coordinates(arg2)) {
				printf("? syntax error\n\n");fflush(stdout);
			}
			else {
				printf("? illegal move\n\n");fflush(stdout);
			}
		}
		else if (!strcmp(arg1, "black") || !strcmp(arg1, "B")) {
			arg2 = strtok(NULL, " ");
			C = B;
			if (Convert_Vertex_to_Coordinates(arg2) && Allow_Move(C)) {
				Do_Move('B');
				B = C;
				printf("=\n\n");fflush(stdout);
			}
			else if (!Convert_Vertex_to_Coordinates(arg2)) {
				printf("? syntax error\n\n");fflush(stdout);
			}
			else {
				printf("? illegal move\n\n");fflush(stdout);
			}
		}
		else {
			printf("? syntax error\n\n");fflush(stdout);
		}
	}
	else {
		printf("? syntax error\n\n");fflush(stdout);
	}
}
void Begin_Game(void) {	/*Sets the beggining state of game*/
	board_data = Create_Board();
	board_display = Display_Board();
	history_list = History_Management();
}
void Boardsize(void) {		/*boardsize command*/
	char *arg1;
	int tmp_size, i = -1;
	tmp_size = size;
	arg1 = strtok(NULL, " ");
	if (arg1 != NULL) {
		do {
			i++;
		} while (isdigit(arg1[i]) && arg1[i] != 0);
		if (i == 0) {
			printf("? the size should be at least 1\n\n");fflush(stdout);
		}
		else {
			if (isdigit(arg1[i - 1])) {
				sscanf(arg1, "%d", &size);
				if (size <= 26 && size > 0) {
					col_count = size - 2;
					row_count = size / 2 - 1;
					Free_Arrays(tmp_size);
					tmp_size = size;
					p = 0;
					Begin_Game();
					printf("=\n\n");fflush(stdout);
				}
				else {
					printf("? the size should be no more than 26 and at least 1\n\n");fflush(stdout);
					size = tmp_size;
				}
			}
			else {
				printf("? syntax error\n\n");fflush(stdout);
			}
		}
	}
	else {
		printf("? syntax error\n\n");fflush(stdout);
	}
}
void Showboard(void) {	/*showbard command*/
	int i;
	printf("=\n\n");fflush(stdout);
	for (i = 0;i<2 * size + 3;i++) {
		if (i > 0 && i < 2 * size + 2 && ((i % 2) == 0)) {
			printf("%02d ", size - i / 2 + 1);fflush(stdout);
		}
		else {
			printf("   ");fflush(stdout);
		}
		printf("%s", board_display[i]);fflush(stdout);
		if (i > 0 && i < 2 * size + 2 && ((i % 2) == 0)) {
			printf(" %02d\n", size - i / 2 + 1);fflush(stdout);
		}
		else {
			printf("\n");fflush(stdout);
		}
	}
	printf("\nblack walls:%d\nwhite walls:%d\n\n\n", B.walls, W.walls);fflush(stdout);
}
