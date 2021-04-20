//	IFSC - Campus Florianópolis						//
//	Curso Técnico Integrado em Eletrônica 7º Fase	//
//													//
//	Projeto de Programação em C						//
//													//
//	Alunos: Geovane Martins Schmitz 				//
//			João Pedro Schmidt Cordeiro				//
//													//
//					TRON O LEGADO					//
//													//
//////////////////////////////////////////////////////
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <stdio.h>

enum KEYS{ UP, DOWN, LEFT, RIGHT};
enum COLOR{ R, G, B};

int main(void)
{
	int width = 640;
	int height = 480;
	
	bool done = false;
	bool redraw = false;
	const int FPS = 60;

    float thickness = 10;

    int color[3] = {255, 0, 255};

	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;

	if(!al_init())										// initialize Allegro
	{										
		return -1;
	}

	display = al_create_display(width, height);			// create our display object

	if(!display)										// test display object
	{										
		return -1;
	}

	al_init_primitives_addon();                         // Instalou o pacote de primitivas
	al_install_keyboard();                              // Instalou o pacote do teclado


	event_queue = al_create_event_queue();              // Criou a lista de eventos

	timer = al_create_timer(1.0 / FPS);

	al_register_event_source(event_queue, al_get_keyboard_event_source());              // Registrou o get_keyboard na fila de eventos
	al_register_event_source(event_queue, al_get_display_event_source(display));        // Registrou o get_display na fila de eventos
	al_register_event_source(event_queue, al_get_timer_event_source(timer)); 			// Registrar o get_timer na fila de eventos

	al_start_timer(timer); 								// Começar o timer

	while(!done)
	{
		ALLEGRO_EVENT ev;                               // Cria um evento no allegro
		al_wait_for_event(event_queue, &ev);            // Fica a espera do evento

		if(ev.type == ALLEGRO_EVENT_TIMER) 
		{
			redraw = true;
		}
		

		if(ev.type == ALLEGRO_EVENT_KEY_DOWN)           // Se apertar a tecla
		{
			switch(ev.keyboard.keycode)
			{
				case ALLEGRO_KEY_UP:
					keys[UP] = true;
					break;
				case ALLEGRO_KEY_DOWN:
					keys[DOWN] = true;
					break;
				case ALLEGRO_KEY_RIGHT:
					keys[RIGHT] = true;
					break;
				case ALLEGRO_KEY_LEFT:
					keys[LEFT] = true;
					break;
                case ALLEGRO_KEY_R:                     // Controle de cores RGB
                    color[R] = color[R] + 20;
					if (color[R] > 255)
                    {
						color[R]=0;
                    }
                    break;
                case ALLEGRO_KEY_G:
                    color[G] = color[G] + 20;
					if (color[G] > 255)
                    {
						color[G]=0;
                    }
                    break;
                case ALLEGRO_KEY_B:
					color[B] = color[B] + 20;
					if (color[B] > 255)
                    {
						color[B]=0;
                    }
                    break;
                case ALLEGRO_KEY_C:                     // Apagar o traço
                    color[R] = 0;
                    color[G] = 0;
                    color[B] = 0;
                    break;
                case ALLEGRO_KEY_EQUALS:                // Mudar a grossura da linha
                    thickness += 1;
					if (thickness >=25) {
 						thickness=25;
 					}
                    break;
                case ALLEGRO_KEY_MINUS:
					thickness -= 1;
					if (thickness <= 1){
						thickness = 1;
					}
                    break;
                case ALLEGRO_KEY_F1:
                    r = al_show_native_message_box(NULL,tcaixa,titulo,texto,NULL,ALLEGRO_MESSAGEBOX_OK_CANCEL);
    				printf("%i",r);
                    break;
			}
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_UP)        // Se soltar a tecla
		{
			switch(ev.keyboard.keycode)
			{
				case ALLEGRO_KEY_UP:
					keys[UP] = false;
					break;
				case ALLEGRO_KEY_DOWN:
					keys[DOWN] = false;
					break;
				case ALLEGRO_KEY_RIGHT:
					keys[RIGHT] = false;
					break;
				case ALLEGRO_KEY_LEFT:
					keys[LEFT] = false;
					break;
				case ALLEGRO_KEY_ESCAPE:
					done = true;
					break;
			}
		}
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) // Se clicar no X do display 
		{
			done = true;
		}

		pos_y -= keys[UP] * (2);                 	// Movimentação do quadrado
		pos_y += keys[DOWN] * (2);
		pos_x -= keys[LEFT] * (2);
		pos_x += keys[RIGHT] * (2);


        if (redraw && al_is_event_queue_empty(event_queue))
		{
			al_draw_filled_rectangle(pos_x, pos_y, pos_x + thickness, pos_y + thickness, al_map_rgb(color[R],color[G],color[B]));
			al_flip_display();
			redraw = false;
		}
	}


	al_destroy_event_queue(event_queue);
	al_destroy_display(display);						// destroy our display object

	return 0;
}