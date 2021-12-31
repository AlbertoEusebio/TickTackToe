/*
	A simple version of the Tick Tack Toe game
	@EusebioAlberto
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

#include "my_functions.c"

#define WIDTH 600
#define HEIGHT 600

#define SIZE (WIDTH/3)

typedef struct p{
	SDL_Rect body;
	int status;
}Cell;

//Functions' caller

_Bool isIn(int x, int y, int i, int j, int n, Cell field[n][n]);
_Bool victory(int n, Cell field[n][n], int player);

void Draw(SDL_Renderer *renderer,int n, Cell field[n][n]);
int aiMarkCell(int n, Cell field[n][n], int count);
int playerMarkCell(SDL_Event *event, int n, Cell field[n][n], int count,_Bool *ON);

int main(){
	SDL_Window * window;
	SDL_Renderer * renderer;
	SDL_Event event;	
	SDL_Init(SDL_INIT_VIDEO);
	
	window = SDL_CreateWindow("Tick Tack Toe", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	int n = 3;
	int count = 9;

	_Bool ON = 1;

	Cell field[n][n];
	for(int i = 0; i<n; i++){
		for(int j = 0; j<n; j++){
			field[i][j].body = (SDL_Rect) {j*SIZE, i*SIZE,SIZE,SIZE};
			field[i][j].status = 0;
		}
	}

	while(ON && count>0){
		count = aiMarkCell(n, field, count);

		if(victory(n, field, (-1))){
			printf("\nGREEN WON\n");
			Draw(renderer, n, field);
			break;
		}

		Draw(renderer, n, field);
		
		count = playerMarkCell(&event, n, field, count, &ON);

		if(victory(n, field, 1)){
			printf("\nRED WON\n");
			ON = 0;
		}

		Draw(renderer, n, field);
	}
	if(!victory(n, field, 1) && !victory(n, field, -1)){
		printf("\nTIE\n");
	}
	delay(1000);		
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

void Draw(SDL_Renderer *renderer, int n, Cell field[n][n]){
	//sets the background
	SDL_SetRenderDrawColor(renderer, 86, 148, 247, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	//draws the field
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
	for(int i =0; i<n;i++){
		for(int j = 0; j<n;j++){
			switch(field[i][j].status){
				case 0:
					SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE); //WHITE
				break;
				case 1:
					SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE); //RED
				break;
				case (-1):
					SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE); //GREEN
				break;
			}
			SDL_RenderFillRect(renderer, &field[i][j].body);
			
			//This will draw black lines around the cell
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
			SDL_RenderDrawRect(renderer, &field[i][j].body);
		}
	}

	SDL_RenderPresent(renderer);
	delay(100);
}

_Bool isIn(int x, int y, int i, int j, int n,  Cell field[n][n]){

  if((field[i][j].body.x<=x&&x<=field[i][j].body.x+field[i][j].body.w)&&(field[i][j].body.y<=y&&y<=field[i][j].body.y+field[i][j].body.h)){
	return 1;
  }

  return 0;
}


int aiMarkCell(int n, Cell field[n][n], int count){
	srand((unsigned)time(0));
	int y,x;

	if(count > 0){
		do{
			y = rand()%n;
			x = rand()%n;

		}while(field[y][x].status != 0);
		
		field[y][x].status = -1; // -1 is the status of the blackcell
		return count-1;
	}
	return count;
}


int playerMarkCell(SDL_Event *event, int n, Cell field[n][n], int count,_Bool *ON){

	int x;
	int y;
	_Bool turn = 1;

	if(count > 0){
		while(turn){
			SDL_PollEvent(event);
			if(event->type == SDL_QUIT){
				turn  = 0;
				*ON = 0;
			}
			if(event->type==SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT){
				SDL_GetMouseState(&x, &y);
				for(int i =  0; i<n; i++){
					for(int j = 0; j<n ;j++){
						if(isIn(x,y,i,j, n, field) && field[i][j].status == 0){
							field[i][j].status = 1;
							turn  = 0;
							return count-1;
						}
					}
				}
			}
		}
	}
	return count;
}

_Bool victory(int n, Cell field[n][n], int player){

	int triplet = 0;

	//checking the rows
	for(int i = 0; i<3; i++){
		for(int j = 0; j<3; j++){
			if(field[i][j].status == player){
				triplet++;
			}		
		}
		if(triplet == 3){
			return 1;
		}else{
			triplet = 0;
		}
	}

	//checking the coloumns
	for(int j = 0; j<3; j++){
		for(int i = 0; i<3; i++){
			if(field[i][j].status == player){
				triplet++;
			}		
		}
		if(triplet == 3){
			return 1;
		}else{
			triplet = 0;
		}
	}

	//checking the decrescent diagonal
	for(int i = 0, j = 0; i<3 && j<3; i++, j++){
		if(field[i][j].status == player){
			triplet++;
		}		
	}
	if(triplet == 3){
		return 1;
	}
	triplet = 0;

	//checking the crescent diagonal
	for(int i = 2, j = 0; i>=0 && j<3; i--, j++){
		if(field[i][j].status == player){
			triplet++;
		}		
	}
	if(triplet == 3){
		return 1;
	}
	return 0;
}
