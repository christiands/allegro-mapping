/*
	alm.c - Allegro Mapping
	Another Allegro demo model, this time covering map drawing
*/

#define REFR_R 60.0f /* Refresh rate in Hz */

#define WIN_L 1280 /* Window length */
#define WIN_H 720 /* Window height */

#define MAP_X 8 /* Map size horizontally */
#define MAP_Y 8 /* Map size vertically */

#define TILE_X 16 /* Tile size horizontally */
#define TILE_Y 16 /* Tile size vertically */

#define SCAL_I 0.2f /* Scaling increment and decrement */
#define MOVE_I 20 /* Movement increment and decrement */

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>

typedef struct
{

	ALLEGRO_BITMAP *bmp; /* The image used for the tile */
	int cln; 			 /* If the player should collide with this tile */

} TILE;

ALLEGRO_TIMER *frame_timer;
ALLEGRO_EVENT_QUEUE *ev_queue;
ALLEGRO_DISPLAY *window;
ALLEGRO_KEYBOARD_STATE keyboard;

ALLEGRO_FONT *bn_font;


TILE c_br;
TILE c_bl;
TILE c_tl;
TILE c_tr;

TILE c_ibl;
TILE c_ibr;
TILE c_itl;
TILE c_itr;

TILE s_b;
TILE s_t;
TILE s_l;
TILE s_r;

TILE flr;
TILE bln;

TILE map[MAP_X * MAP_Y];
ALLEGRO_BITMAP *comp_map;

void init();
void load();
void destroy();
void make_map(TILE m[]);
ALLEGRO_BITMAP *draw_map(TILE m[], int size_x, int size_y, int tile_l, int tile_w, float scale_p);

void init()
{
	al_init();

	al_install_keyboard();
	al_init_image_addon();

	frame_timer = al_create_timer(1.0 / REFR_R);
	ev_queue = al_create_event_queue();
	window = al_create_display(WIN_L, WIN_H);
	bn_font = al_create_builtin_font();

	al_register_event_source(ev_queue, al_get_keyboard_event_source());
	al_register_event_source(ev_queue, al_get_timer_event_source(frame_timer));
	al_register_event_source(ev_queue, al_get_display_event_source(window));
}

void load()
{
	c_bl.bmp = al_load_bitmap("assets/corner_bl.png");
	c_br.bmp = al_load_bitmap("assets/corner_br.png");
	c_tl.bmp = al_load_bitmap("assets/corner_tl.png");
	c_tr.bmp = al_load_bitmap("assets/corner_tr.png");

	c_ibl.bmp = al_load_bitmap("assets/corner_ibl.png");
	c_ibr.bmp = al_load_bitmap("assets/corner_ibr.png");
	c_itl.bmp = al_load_bitmap("assets/corner_itl.png");
	c_itr.bmp = al_load_bitmap("assets/corner_itr.png");

	s_b.bmp = al_load_bitmap("assets/straight_b.png");
	s_t.bmp = al_load_bitmap("assets/straight_t.png");
	s_l.bmp = al_load_bitmap("assets/straight_l.png");
	s_r.bmp = al_load_bitmap("assets/straight_r.png");

	flr.bmp = al_load_bitmap("assets/floor.png");
	bln.bmp = al_load_bitmap("assets/blank.png");


	c_bl.cln = 1;
	c_br.cln = 1;
	c_tl.cln = 1;
	c_tr.cln = 1;

	c_ibl.cln = 1;
	c_ibr.cln = 1;
	c_itl.cln = 1;
	c_itr.cln = 1;

	s_b.cln = 1;
	s_t.cln = 1;
	s_l.cln = 1;
	s_r.cln = 1;

	flr.cln = 0;
	bln.cln = 1;
}

void destroy()
{
	al_destroy_display(window);
	al_destroy_timer(frame_timer);
	al_destroy_event_queue(ev_queue);
	al_destroy_font(bn_font);

	al_destroy_bitmap(c_bl.bmp);
	al_destroy_bitmap(c_br.bmp);
	al_destroy_bitmap(c_tl.bmp);
	al_destroy_bitmap(c_tr.bmp);

	al_destroy_bitmap(c_ibl.bmp);
	al_destroy_bitmap(c_ibr.bmp);
	al_destroy_bitmap(c_itl.bmp);
	al_destroy_bitmap(c_itr.bmp);
	
	al_destroy_bitmap(s_b.bmp);
	al_destroy_bitmap(s_t.bmp);
	al_destroy_bitmap(s_l.bmp);
	al_destroy_bitmap(s_r.bmp);
	
	al_destroy_bitmap(flr.bmp);
	al_destroy_bitmap(bln.bmp);
}

void make_map(TILE m[]) /* Grouped in sections of MAP_X by MAP_Y */
{
	/* (0, 0) to (0, 7) */
	m[0] = bln;
	m[1] = bln;
	m[2] = bln;
	m[3] = c_ibr;
	m[4] = s_b;
	m[5] = s_b;
	m[6] = c_ibl;
	m[7] = bln;

	/* (1, 0) to (1, 7) */
	m[8] = bln;
	m[9] = bln;
	m[10] = c_ibr;
	m[11] = c_br;
	m[12] = flr;
	m[13] = flr;
	m[14] = c_bl;
	m[15] = s_b;

	/* (2, 0) to (2, 7) */
	m[16] = bln;
	m[17] = c_ibr;
	m[18] = c_br;
	m[19] = flr;
	m[20] = flr;
	m[21] = flr;
	m[22] = flr;
	m[23] = flr;

	/* (3, 0) to (3, 7) */
	m[24] = c_ibr;
	m[25] = c_br;
	m[26] = flr;
	m[27] = flr;
	m[28] = flr;
	m[29] = flr;
	m[30] = flr;
	m[31] = flr;
	
	/* (4, 0) to (4, 7) */
	m[32] = s_r;
	m[33] = flr;
	m[34] = flr;
	m[35] = flr;
	m[36] = flr;
	m[37] = flr;
	m[38] = flr;
	m[39] = flr;
	
	/* (5, 0) to (5, 7) */
	m[40] = s_r;
	m[41] = flr;
	m[42] = flr;
	m[43] = flr;
	m[44] = flr;
	m[45] = flr;
	m[46] = flr;
	m[47] = flr;
	
	/* (6, 0) to (6, 7) */
	m[48] = s_r;
	m[49] = flr;
	m[50] = flr;
	m[51] = flr;
	m[52] = c_tl;
	m[53] = s_t;
	m[54] = s_t;
	m[55] = s_t;
	
	/* (7, 0) to (7, 7) */
	m[56] = s_r;
	m[57] = flr;
	m[58] = flr;
	m[59] = c_tl;
	m[60] = c_itl;
	m[61] = bln;
	m[62] = bln;
	m[63] = bln;
}

ALLEGRO_BITMAP *draw_map(TILE m[], int size_x, int size_y, int tile_l, int tile_w, float scale_p)
{
	fprintf(stdout, "Started the map compilation process\n");
//	ALLEGRO_BITMAP *tmp;
	ALLEGRO_BITMAP *map;
	ALLEGRO_BITMAP *prv;

//	tmp = al_create_bitmap(tile_w * scale_p, tile_l * scale_p);
	map = al_create_bitmap(((float)(size_x * tile_w)) * scale_p, ((float)(size_y * tile_l)) * scale_p);
	fprintf(stdout, "Compiled map w: %d\nCompiled map h: %d\n", al_get_bitmap_width(map), al_get_bitmap_height(map));
	prv = al_get_target_bitmap();
	al_set_target_bitmap(map);

	float m_sx = 0;
	float m_sy = 0;
	float m_sl;
	float m_sw;

	float m_dx;
	float m_dy;
	float m_dl;
	float m_dw;

	for(int i = 0; i < (size_x * size_y); ++i)
	{
//			m_sx = (float)(pos_x + ((i % size_x) * tile_l));
//			m_sy = (float)(pos_y + ((i / size_y) * tile_w));
			m_sl = (float)tile_l;
			m_sw = (float)tile_w;
//			m_dx = ((float)pos_x * scale_p) + ((float)((i % size_x) * tile_l));
//			m_dy = ((float)pos_y * scale_p) + ((float)((i / size_y) * tile_w));
			m_dx = ((float)tile_l * (i % size_x)) * scale_p;
			m_dy = ((float)tile_w * (i / size_y)) * scale_p;
			m_dl = (float)tile_l * scale_p;
			m_dw = (float)tile_w * scale_p;

		al_draw_scaled_bitmap(m[i].bmp, m_sx, m_sy, m_sl, m_sw, m_dx, m_dy, m_dl, m_dw, 0);
//		fprintf(stdout, "Iteration %d, sx: %f, sy: %f, sl: %f, sw: %f, dx: %f, dy: %f, dl: %f, dw:%f\n", i, m_sx, m_sy, m_sl, m_sw, m_dx, m_dy, m_dl, m_dw);
	}
	fprintf(stdout, "Compiled map\n");
	al_set_target_bitmap(prv);
	return map;

	/*
	for(int i = 0; i < (size_x * size_y); ++i)
	{
//		al_draw_bitmap(m[i].bmp, (float)((i % size_x) * tile_l), (float)((i / size_y) * tile_w), 0);
		al_draw_scaled_bitmap(m[i].bmp,
			(float)(pos_x + ((i % size_x) * tile_l)),
			(float)(pos_y + ((i / size_y) * tile_w)),
			(float)tile_l,
			(float)tile_w,
			((float)pos_x * scale_p) + ((float)((i % size_x) * tile_l)),
			((float)pos_y * scale_p) + ((float)((i / size_y) * tile_w)),
			(float)tile_l * scale_p,
			(float)tile_w * scale_p,
			0
			);
	}
	*/
}

int main(void)
{
	init();
	load();
	make_map(map);

	int done = 0;
	int redraw = 1;

	ALLEGRO_EVENT ev_actual;
	al_start_timer(frame_timer);

	int map_pos_x = 0;
	int map_pos_y = 0;
	float map_scale = 5.0;

	comp_map = draw_map(map, MAP_X, MAP_Y, TILE_X, TILE_Y, map_scale);
	fprintf(stdout, "Comp Map Stats: w: %d, h: %d\n", al_get_bitmap_width(comp_map), al_get_bitmap_height(comp_map));

	while(1)
	{
		al_wait_for_event(ev_queue, &ev_actual);

		switch(ev_actual.type)
		{
			case ALLEGRO_EVENT_TIMER:
				redraw = 1;
				break;
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				done = 1;
				break;
			case ALLEGRO_EVENT_KEY_DOWN:
				if(ev_actual.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
					done = 1;
				if(ev_actual.keyboard.keycode == ALLEGRO_KEY_Z)
				{
					map_scale = map_scale - SCAL_I;
					comp_map = draw_map(map, MAP_X, MAP_Y, TILE_X, TILE_Y, map_scale);
				}
				if(ev_actual.keyboard.keycode == ALLEGRO_KEY_X)
				{
					map_scale = map_scale + SCAL_I;
					comp_map = draw_map(map, MAP_X, MAP_Y, TILE_X, TILE_Y, map_scale);
				}
				if(ev_actual.keyboard.keycode == ALLEGRO_KEY_LEFT)
					map_pos_x = map_pos_x - MOVE_I;
				if(ev_actual.keyboard.keycode == ALLEGRO_KEY_RIGHT)
					map_pos_x = map_pos_x + MOVE_I;
				if(ev_actual.keyboard.keycode == ALLEGRO_KEY_UP)
					map_pos_y = map_pos_y - MOVE_I;
				if(ev_actual.keyboard.keycode == ALLEGRO_KEY_DOWN)
					map_pos_y = map_pos_y + MOVE_I;
				break;
		}

		if(done)
			break;

		if(redraw && al_is_event_queue_empty(ev_queue))
		{
//			fprintf(stdout, "Redrawing!\n");
			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_draw_bitmap(comp_map, map_pos_x, map_pos_y, 0);
//			fprintf(stdout, "Drew compiled map!\n");
			al_draw_textf(bn_font, al_map_rgb(255, 255, 255), 0, 0, 0, "map_pos_x: %d | map_pos_y: %d | map_scale: %f", map_pos_x, map_pos_y, map_scale);
			al_flip_display();

			redraw = 0;
		}
	}

	destroy();
	return 0;
}