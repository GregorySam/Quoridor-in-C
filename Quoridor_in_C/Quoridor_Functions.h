struct shell {
	char isblocked_a;									/*   shell:  __a_ each side represents if there is or not a wall*/
	char isblocked_b;									/*			|		|	f describes wether there is or there is not 'W' or'B' in the shell*/
	char isblocked_c;									/*	d		|	f	|b */
	char isblocked_d;									/*			|_______|		*/
	char pawn;												/*				c*/
};
struct pawn {
	int x;
	int y;
	int walls;
};
struct history {
	int col_count;
	int row_count;
	struct shell **board_data;
	unsigned char **board_display;
	struct pawn B, W;
};
void List_Commands(void);
unsigned char **Display_Board(void);
int Allow_Move(struct pawn);
void Do_Move(char);
int Convert_Vertex_to_Coordinates(char*);
struct shell **Create_Board(void);
int Allow_Wall_Placement(int, int, char *, struct pawn);
void Wall_Placement(char *);
char *Input(void);
void Is_Known(char *);
int Walls(void);
struct history *History_Management(void);
void Undo(char *);
void Winner(void);
void Free_Arrays(int);
void Show_Move(void);
void Show_Wall_Placement(char *);
int Strategy_Case(char , char , unsigned int );
int Strategy(char );
void Rand_move(char);
void Playwall(void);
void Genmove(void);
void Playmove(void);
void Begin_Game(void);
void Boardsize(void);
void Showboard(void);