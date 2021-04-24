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
#include "libs/defs.h"

int main(void)
{
	int width = 640;
	int height = 480;

	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;

	if (!al_init()) // initialize Allegro
	{
		return -1;
	}

	al_set_new_display_flags(ALLEGRO_RESIZABLE);
	display = al_create_display(width, height);

	if (!display) // test display object
	{
		return -1;
	}

	al_init_primitives_addon();
	al_install_keyboard();

	event_queue = al_create_event_queue();

	timer = al_create_timer(1.0 / FPS);

	al_register_event_source(event_queue, al_get_keyboard_event_source());		 // Registrou o get_keyboard na fila de eventos
	al_register_event_source(event_queue, al_get_display_event_source(display)); // Registrou o get_display na fila de eventos
	al_register_event_source(event_queue, al_get_timer_event_source(timer));	 // Registrar o get_timer na fila de eventos

	al_start_timer(timer); // Começar o timer
	while (!0)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (al_is_event_queue_empty(event_queue))
		{
			al_flip_display();
		}
	}

	al_destroy_event_queue(event_queue);
	al_destroy_display(display);
	return 0;
}