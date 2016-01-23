#include <iostream>
#include <allegro5\allegro5.h>
#include <allegro5\allegro_image.h>
#include <time.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>

using namespace std;



int main(){
	int lenght = 800;
	int width = 600;
	ALLEGRO_DISPLAY *display;
	float FPS = 20.0;

	//makes sure the program initalizes correctly. if it doesnt the program wont open
	if (!al_init()) return -1;
	display = al_create_display(lenght, width);
	if (!display) return -1;
	al_set_window_position(display, 200, 200);

	//initalizes addons
	al_install_keyboard();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();

	//loads the images that will be used in the program
	ALLEGRO_BITMAP *head = al_load_bitmap("head.png");
	ALLEGRO_BITMAP *tail = al_load_bitmap("tail.png");
	//loads the font that is to be used in the menu
	ALLEGRO_FONT *menuFont = al_load_font("myFont.ttf", 60, 0);
	ALLEGRO_BITMAP *token = al_load_bitmap("token.png");

	ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);//decided the "FPS" of the game or how fast the code will update
	ALLEGRO_KEYBOARD_STATE keyState; //used to keep the snake going in whichever direction was last input by the keyboard
	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue(); //allocates an empty event queue in memory
	/*Registers events with the program. What this does is tells the program we're taking information from
	specific locations and we're putting them into our event queue and from there the program will execute
	based on the order things are in queue. For example we're registering and event with the source being the keyboard
	so when the events are called for in the program the user is going to be able to use the keyboard to put events into the
	event queue*/
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_start_timer(timer);
	srand(time(NULL));

	enum Direction { DOWN, LEFT, RIGHT, UP };
	//varriables that will decided if the game loop is running along with
	bool done = false;
	int x1 = 0;
	int y1 = 0;
	int moveSpeed = 5; //sets x and y cords and how fast the snake moves in the game
	int dir = DOWN; //starts the snake going down
	int score;
	int x2;
	int y2;
	int xCordtoken = 20 * (rand() % 20); //gives the token random x value so it can be placed in the game randomly
	int yCordtoken = 20 * (rand() % 15); //gives the token random y value so it can be placed in the game randomly
	int xCord[100]; //sets up the arrays to be used to determine x and y cords for the snake
	int yCord[100];
	bool menu = true;
	bool dead = false;

	while (!done){ //game loop

		x2 = x1;
		y2 = y1;

		ALLEGRO_EVENT events; //delcares an event variable
		al_wait_for_event(event_queue, &events); //tells the program to be ready for events to be placed in queue 

		if (events.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (events.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE: //if user presses escape
				done = true; //exit game loop
				break;
			case ALLEGRO_KEY_ENTER: //if user presses enter
				if (menu) menu = false, score = 0, x1 = 0, y1 = 0; /*if user enters enter and they're at the game menu
																   set the menu to false and set
																   the x1 and y1 cords to 0*/
				break;
			} //end of switch            
		} //end of if(events.type == ALLEGRO_EVENT_KEY_UP)

		else if (events.type == ALLEGRO_EVENT_DISPLAY_CLOSE) //if the display is closed
			done = true; //exit game loop

		if (events.type == ALLEGRO_EVENT_TIMER)//if there is an event timer registered
		{
			if (events.timer.source == timer)/*if the timer registered to the event queue is running. the body of the if statement runs seperatly for each time the timer changes
											 and is unique for each second (regulated by FPS that we set above)*/
			{
				al_get_keyboard_state(&keyState); /*saves the what each key does. the keystate that is passed in this function is whichever key was pressed
												  at the whatever time the event queue timer is at. and the following functions keep the keystate in the same
												  postion until another key is passed at a diffrent time*/
				if (al_key_down(&keyState, ALLEGRO_KEY_RIGHT) && dir != LEFT)
					dir = RIGHT;
				else if (al_key_down(&keyState, ALLEGRO_KEY_LEFT) && dir != RIGHT)
					dir = LEFT;
				else if (al_key_down(&keyState, ALLEGRO_KEY_UP) && dir != DOWN)
					dir = UP;
				else if (al_key_down(&keyState, ALLEGRO_KEY_DOWN) && dir != UP)
					dir = DOWN;
				else if (al_key_down(&keyState, ALLEGRO_KEY_ENTER) && menu == true)
					menu = false, score = 0, x1 = 0, y1 = 0;
				if (menu == false)
				{
					if (score != 0)
					{
						for (int i = score; i>0; i--)/*builds the size of the snake based on the score and the values of the positions
													 for each part of the snake is assigned below*/
						{
							xCord[i] = xCord[i - 1];
							yCord[i] = yCord[i - 1];
						} //end of for(int i=score;i>0;i--)

						xCord[0] = x2; //updatest he postion of the snake based on the switch below
						yCord[0] = y2; /*we use x2 and y2 to keep it one step behind the head of the snake because the snake uses X1 and Y1
									   and x2 and y2 are used before the vaules of x1 and y1 change again*/

					} //end of if(score!=0)
				} // end of if(menu==false)



				/*BEFORE THE GAME GETS TO THIS POINT KEEP IN MIND THAT THE HEAD OF THE SNAKE AND THE
				FIRST PART OF THE BODY OF THE SNAKE ARE ON TOP OF EACH OTHER*/


				/*This switch updates the position of the head of the snake and like said above this happens after x2 and y2
				are assigned to keep the head of the snake in front of the body*/
				switch (dir)
				{
				case RIGHT: x1 = x1 + 20;
					break;
				case LEFT: x1 = x1 - 20;
					break;
				case UP: y1 = y1 - 20;
					break;
				case DOWN: y1 = y1 + 20;
					break;
				} //end of switch

				if (x1 == xCordtoken && y1 == yCordtoken)/*(x and y correspond to the head of the snake) if the x cord of the snake touches the x cord of the token
														 and the y cord of the snake touches the y cord of the token*/
				{
					score++; //add to the score
					xCordtoken = 20 * (rand() % 40); //give the token another set of random x and y cords
					yCordtoken = 20 * (rand() % 30);
				} //end of if(x1==xCordtoken && y1==yCordtoken)
				if (menu == false)
				{
					for (int i = 0; i<score; i++)//score will always be equivilent to the size of the snake
					{
						if (x1 == xCord[i] && y1 == yCord[i] && menu == false) dead = true; //if in game mode and the snake collides then set dead to true
					}
					if (x1<0 || x1 >= 800 || y1<0 || y1 >= 600 && menu == false) dead = true; //if the snake hits the walls                    
				} //end of if(menu==false)
			} //end of if(events.timer.source == timer)
		} //end of if(events.type == ALLEGRO_EVENT_TIMER)

		if (dead && menu == false)//if user dies and game is not at the menu then reset the game
		{
			menu = true;
			x1 = 0;//puts the snake back at the top of the screen
			y1 = 0;
			xCordtoken = 20 * (rand() % 40);//shows token in a diffrent place than before you died
			yCordtoken = 20 * (rand() % 30);
			dead = false;
			dir = DOWN; //sets the direction of the snake to down so when the game starts the snake begins going down
		}
		if (menu)/*if at the menu. reset the x1 and y1 and print to the screen the menu*/
		{
			x1 = 0;
			y1 = 0;
			al_draw_text(menuFont, al_map_rgb(100, 50, 250), 80, 0, 0, "Enter to Start");
			al_draw_text(menuFont, al_map_rgb(100, 50, 250), 80, 150, 0, "Esc to Exit");
		}
		else//if not at the menu show the gamestate (begin drawing)
		{
			al_draw_bitmap(token, xCordtoken, yCordtoken, NULL); //draws the token on the screen based on the rand location we gave above
			for (int i = 0; i<score; i++)/*draws the body of the snake (updates above the dir switch) works by accessing each cell of xCord and yCord
										 and draws part of the tail based on the value storted in xCord[i] and yCord[i] and it does this for how ever big
										 the snake is which is determined by the score*/
			{
				al_draw_bitmap(tail, xCord[i], yCord[i], NULL);
			}
			al_draw_bitmap(head, x1, y1, NULL); //draws the head of the snake based on the x1 and y1 cords (which update above the dir switch)
		}
		al_flip_display(); //flips the buffer to be seen by the user (does this every time through the while loop; based off FPS set earlier in program)
		al_clear_to_color(al_map_rgb(0, 0, 0));
	}//end of while                     

	al_destroy_display(display); //destroy allocated variables to avoid memory leak
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);
	return 0;
}

