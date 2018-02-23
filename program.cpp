//#include <stdio.h>
//#include <stdlib.h>

/*
	0: normal
	1: text
*/
void changeMouse(int mode);
void print(const char* n);
void floatprint(float n);
void print(int n);
float rand();
float sin(float x);
float cos(float x);
int ceil(float x);
float abs(float x) { return x < 0 ? -x : x; };

// width: 300, height: 250

unsigned char screen[300 * 250 * 3];

int w = 300;
int h = 250;

bool mouseDown = false;
int mouseX = 0;
int mouseY = 0;

float camera_move_speed = 0.005;
float move_speed = 0.1;

unsigned char map[64 * 64 * 64];
int texmap[16 * 16 * 3 * 26];
int glass_colors[] = {0x000000, 0xFEFEFE, 0xC0F5FE, 0xB3D6DB};
char glass_map[] = {
	1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,2,
	1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,2,
	1,0,0,1,0,0,2,0,0,0,0,0,0,0,0,0,
	1,0,1,0,0,0,0,0,0,0,0,0,0,0,1,2,
	1,1,0,0,0,0,0,0,0,1,0,0,0,2,0,3,
	1,0,0,0,0,0,0,0,1,0,0,0,2,0,0,0,
	0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,3,
	1,0,2,0,0,0,2,0,0,0,0,0,0,0,0,3,
	1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,3,
	1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,
	1,0,0,0,0,0,2,0,0,0,0,0,1,0,0,3,
	0,2,0,0,0,0,0,0,0,0,0,1,0,0,0,3,
	2,0,0,0,0,0,0,0,0,0,2,0,0,0,0,3,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
	1,3,3,3,3,3,0,0,3,3,0,3,3,3,3,2
};

char font[] = {
	0,0,1,1,1,0,0,0,0,1,1,0,1,1,0,0,1,1,0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,1,1,1,1,1,0,1,1,0,0,0,
	1,1,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,1,1,0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,
	1,1,1,1,0,0,1,1,0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,
	0,0,1,1,0,0,1,1,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,1,1,
	1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,
	1,0,0,0,1,1,0,1,1,0,0,1,1,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,1,1,0,0,0,0,
	0,0,1,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,
	0,0,0,0,0,1,1,1,1,1,1,1,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,
	1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,1,0,0,0,0,0,1,1,0,0,0,
	0,0,0,1,1,0,0,1,1,1,0,1,1,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,
	0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,1,1,1,1,1,0,1,1,0,0,0,1,1,0,1,1,0,0,0,1,1,
	0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,
	0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,0,0,1,1,0,0,1,1,1,1,1,
	0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,1,1,0,0,1,1,0,0,1,1,0,1,1,0,0,0,1,1,1,1,0,0,0,0,1,1,1,
	1,1,0,0,0,1,1,0,1,1,1,0,0,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,
	0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,
	0,0,0,1,1,0,0,0,1,1,0,1,1,1,0,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0,1,1,0,1,0,1,1,0,1,1,
	0,0,0,1,1,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,1,1,1,0,0,1,1,0,1,1,1,1,0,1,1,
	0,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,0,1,1,0,0,1,1,1,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,
	1,1,0,0,1,1,0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,0,0,1,1,0,0,
	1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,1,1,0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,0,0,1,
	1,0,1,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,1,0,
	0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,1,1,1,1,0,1,1,0,0,1,1,0,0,0,1,1,1,1,0,1,0,
	0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,1,1,0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,0,1,1,1,0,1,1,1,1,1,
	0,0,0,1,1,0,1,1,1,0,0,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0,1,1,
	0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,1,1,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,
	0,0,1,1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,
	1,0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,0,0,1,1,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,
	1,0,1,1,0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,1,0,1,1,1,0,0,1,1,1,1,1,0,0,0,0,1,1,1,0,0,0,0,0,0,
	1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,1,0,1,1,0,1,1,1,1,1,1,1,0,
	1,1,1,1,1,1,1,0,1,1,1,0,1,1,1,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,1,1,1,0,1,
	1,1,0,0,1,1,1,1,1,0,0,0,0,1,1,1,0,0,0,0,1,1,1,1,1,0,0,1,1,1,0,1,1,1,0,1,1,0,0,0,1,1,0,0,0,
	0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,1,1,0,0,0,0,0,1,1,0,0,
	0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,1,
	1,1,0,0,0,0,1,1,1,0,0,0,0,1,1,1,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,
	0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,
	1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,
	0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,
	0,0,0,0,0,0,1,1,1,1,0,0,0,1,0,0,0,0,1,0,1,0,0,1,1,0,0,1,1,0,1,0,0,0,0,1,1,0,1,0,0,0,0,1,1,
	0,0,1,1,0,0,1,0,1,0,0,0,0,1,0,0,0,1,1,1,1,0,0,0,0,1,1,1,0,0,0,0,1,0,0,1,1,0,0,1,1,0,0,0,1,
	1,0,1,1,0,0,0,1,1,0,1,1,0,0,0,1,1,0,0,1,1,0,0,1,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,
	0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,1,1,1,0,0,0,1,1,1,
	1,0,0,0,1,1,1,1,0,0,0,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,
	0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,1,1,0,0,1,1,1,1,1,0,
	0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1,1,1,1,0,0,0,1,1,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,1,1,
	1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,1,
	1,1,1,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,1,1,0,0,0,1,1,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,
	0,0,0,0,1,1,1,1,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,1,1,0,0,0,1,1,0,1,1,0,
	0,0,1,1,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,1,1,0,0,0,1,1,0,0,0,0,0,1,1,0,0,
	0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,
	1,0,0,1,1,0,0,0,1,1,0,1,1,0,0,0,1,1,0,0,1,1,1,1,1,0,0,1,1,0,0,0,1,1,0,1,1,0,0,0,1,1,0,0,1,
	1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,1,0,0,0,1,1,0,1,1,0,0,0,1,1,0,0,1,1,1,1,1,1,
	0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,
	1,1,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0,
	0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,
	1,0,0,0,0,0,1,1,0,0,0,0,1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,0,0,0,0,1,
	1,1,1,0,0,0,1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,1,1,0,0,
	0,0,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,0,1,1,1,
	1,0,0,0,0,1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,0,0,0,0,1,1,1,1,0,0,0,1,
	1,1,1,1,1,1,0,1,1,1,1,1,1,1,0,0,1,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,
	0,0,0,1,1,1,0,0,0,0,0,1,1,1,1,0,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0,0,0,1,1,1,1,0,0,0,0,1,1,
	1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,
	0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,0,
	0,0,1,1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,
	1,0,0,0,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,
	0,1,1,0,0,0,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,0,0,0,1,1,1,1,0,
	0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,
	0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1,0,
	0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,1,1,1,
	1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,
	0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,1,1,1,0,0,1,1,1,1,0,1,1,1,1,0,1,0,
	0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

int FNT_LEN = 3712 / 64;

void init() {
	for (int i = 1; i <= 25; i++) { // for each texture
		float br = 255 - ((int)(rand() * 96) | 0); // number between 160 - 255 (gives texture a random noise)
		for (int y = 0; y < 16 * 3; y++) {
			for (int x = 0; x < 16; x++) {
				int color = 0x966C4A; // default color (brown-like)
				if (i == 3) { // stone
					color = 0x7F7F7F;
				}
				
				if (i != 3 || ((int)(rand() * 3) | 0) == 0) {
					br = 255 - ((int)(rand() * 96) | 0); // reset the random noise
				}
				
				if ((i == 1 && y < (((x * x * 3 + x * 81) >> 2) & 3) + 18)) { // grass
					color = 0x6AAA40;
				} else if ((i == 1 && y < (((x * x * 3 + x * 81) >> 2) & 3) + 19)) {
					br = br * 2 / 3;
				}
				
				if (i == 4) { // brick
					color = 0xB53A15;
					if ((x + (y >> 2) * 4) % 8 == 0 || y % 4 == 0) {
						color = 0xBCAFA5;
					}
				}
				
				if (i == 5) { // wood
					color = 0x675231;
					if (x > 0 && x < 15
							&& ((y > 0 && y < 15) || (y > 32 && y < 47))) {
						color = 0xBC9862;
						int xd = (x - 7);
						int yd = ((y & 15) - 7);
						if (xd < 0)
							xd = 1 - xd;
						if (yd < 0)
							yd = 1 - yd;
						if (yd > xd)
							xd = yd;

						br = 196 - ((int)(rand() * 32) | 0) + xd % 3 * 32;
					} else if (((int)(rand() * 2) | 0) == 0) {
						br = br * (150 - (x & 1) * 100) / 100;
					}
				}
				float brr = br;
				if (y >= 32)
					brr /= 2;

				if (i == 6) { // leaves
					color = 0x50D937;
					if (((int)(rand() * 2) | 0) == 0) {
						color = 0;
						brr = 255;
					}
				}
				
				if (i == 7) { // water
					color = 0x4040ff;
				}
				
				if(i == 8) { // glass
					color = glass_colors[glass_map[(y%16)*16+x]];
					brr = 255;
				}
				if(i == 9) { // green crystal
					int y_ = y%16;
					color = ((x+y_)*4352);
					if(color < 1) color = 1;
					brr = 255;
				}
				if(i == 10) { // WHITE
					color = 0xDDDDDD;
					brr = 255;
				}
				if(i == 11) { // BLACK
					color = 0x191616;
					brr = 255;
				}
				if(i == 12) { // RED
					color = 0x963430;
					brr = 255;
				}
				if(i == 13) { // GREEN
					color = 0x35461B;
					brr = 255;
				}
				if(i == 14) { // BLUE
					color = 0x2E388D;
					brr = 255;
				}
				if(i == 15) { // ORANGE
					color = 0xDB7D3E;
					brr = 255;
				}
				if(i == 16) { // MAGENTA
					color = 0xB350BC;
					brr = 255;
				}
				if(i == 17) { // LIGHT BLUE
					color = 0x6B8AC9;
					brr = 255;
				}
				if(i == 18) { // YELLOW
					color = 0xB1A627;
					brr = 255;
				}
				if(i == 19) { // LIME
					color = 0x41AE38;
					brr = 255;
				}
				if(i == 20) { // PINK
					color = 0xD08499;
					brr = 255;
				}
				if(i == 21) { // GRAY
					color = 0x404040;
					brr = 255;
				}
				if(i == 22) { // LIGHT GRAY
					color = 0x9AA1A1;
					brr = 255;
				}
				if(i == 23) { // CYAN
					color = 0x2E6E89;
					brr = 255;
				}
				if(i == 24) { // PURPLE
					color = 0x7E3DB5;
					brr = 255;
				}
				if(i == 25) { // BROWN
					color = 0x4F321F;
					brr = 255;
				}
				int col = (int)(((color >> 16) & 0xff) * brr / 255) << 16
						| (int)(((color >> 8) & 0xff) * brr / 255) << 8
						| (int)(((color) & 0xff) * brr / 255); // adjust the noise (brr) to each rgb in the color. the color is a number between 0 and 16777215
				
				texmap[x + y * 16 + i * 256 * 3] = col; // add to texture map
			}
		}
	}
}

void initMap() {
	for (int x = 0; x < 64; x++) { // default map (grass and dirt flat-land)
		for (int y = 0; y < 64; y++) {
			for (int z = 0; z < 64; z++) {
				int i = z << 12 | y << 6 | x; // convert XYZ into index
				int block = 0;
				if(y == 45) block = (int)(rand() * 26);
				if(y > 45) block = 2;
				map[i] = block;
			}
		}
	}
}

float pi = 3.141592653589793;

float xRot = 4.235;
float yRot = -0.25;

float ox = 32.5;
float oy = 44.539;
float oz = 32.5;

bool Night = false;

void render() {
	float yCos = cos(yRot);
	float ySin = sin(yRot);
	float xCos = cos(xRot);
	float xSin = sin(xRot);
	
	int hw = ceil((float)w/2); // half of width/height
	int hh = ceil((float)h/2);
	
	int FOV = 1;
	
	bool NEG__ = false;
	bool updated_selection_box = false;
	
	for (int x = 0; x < w; x++) { // all x pixels
		float xScale = ((float)x - (float)w / 2) / (float)h; // scaling of width
		for (int y = 0; y < h; y++) { // all y pixels
			float yScale = ((float)y - (float)h / 2) / (float)h; // scaling of height
			
			bool CE = (x == hw && y == hh); // Is the pixel in the middle of the crosshair?

			float zDepthScale = (FOV * yCos + yScale * ySin);
			
			float _yd = (yScale * yCos - FOV * ySin);
			float _xd = (xScale * xCos + zDepthScale * xSin);
			float _zd = (zDepthScale * xCos - xScale * xSin);

			int col = 0; // Color of pixel
			bool colUpdated = false; // did color get updated?
			int br = 255; // Brightness
			int ddist = 255; // Distance fading

			float closest = 11; // Distance to render everything
			float var36 = closest; //max distance for selection box
			for (int d = 0; d < 3; d++) { // One loop for each visible face
				float dimLength = _xd;
				if (d == 1)
					dimLength = _yd;
				if (d == 2)
					dimLength = _zd;

				float ll = abs(dimLength);
				float xd = _xd / ll;
				float yd = _yd / ll;
				float zd = _zd / ll;

				//blocks are glitchy when the camera is behind XYZ 0.5 because of decimal precision errors
				float initial = ox - ((int)ox);
				if (d == 1)
					initial = oy - ((int)oy);
				if (d == 2)
					initial = oz - ((int)oz);
					
				NEG__ = initial < 0;
					
				initial = abs(initial);
					
				if (dimLength > 0 && !NEG__)
					initial = 1 - initial; // invert the initial number
					
				if(dimLength < 0 && NEG__) {
					initial = 1 - initial;
				}

				float dist = initial / ll; // total distance ray has travelled?

				float xp = (ox + xd * initial);
				float yp = (oy + yd * initial);
				float zp = (oz + zd * initial);

				if (dimLength < 0) {
					if (d == 0)
						xp--;
					if (d == 1)
						yp--;
					if (d == 2)
						zp--;
				}

				while (dist < closest) { // if the blocks are within distance
					float ux = (xp - 1); // adjusted XYZ positions
					float uy = (yp - 1);
					float uz = (zp - 1);
					if(uz < 64 && uy < 64 && ux < 64 && uz >= 0 && uy >= 0 && ux >= 0) { //render only blocks inside the map
						int ptr = (int)uz << 12 | (int)uy << 6 | (int)ux; // index of the block
						int tex = map[ptr]; // block from the map
						if ((tex > 0/* || (ptr == var14)*/)) { // if the block is not air or if the block is selected
							int u = (int)((ux + uz) * 16) & 15; // x position of the texture
							//floatprint(uy);
							int v = ((int)(uy * 16) & 15) + 16; // y position of the texture. any number greater than 16 is a different side of the block
							if (d == 1) { // if top/botttom face
								u = (int)(ux * 16) & 15;
								v = ((int)(uz * 16) & 15);
								if (yd < 0)
									v += 32;
							}
							
							/*float cc = 16777215;
							if(ptr != var14 || u > 0 && v % 16 > 0 && u < 15 && v % 16 < 15) { // if block is selection box
								cc = texmap[u + v * 16 + tex * 256 * 3];
							}*/
							int cc = texmap[u + v * 16 + tex * 256 * 3];//16777215;
							
							/*if(dist < var36 && CE && tex > 0) { // Calculate selection box location
								updated_selection_box = true
								var66 = ptr;
								var36 = dist
								
								float NEG = 1;
								if(dimLength > 0) {
								   NEG = -1;
								}
								var15 = NEG << 6 * d;
							}*/
							if (cc > 0) {
								col = cc;
								colUpdated = true;
								ddist = 255 - ((int)dist << 3); // No "if true" statement, because it somehow slows the engine down.
								if(!Night) ddist = ((int)(dist * 25.6f)) - 600;
								br = (255 - ((d + 2) % 3) * 50);
								closest = dist; // make this the final loop
							}
						}
					}/* else {
						closest = dist;
						if(dist < var36 && CE) {
							updated_selection_box = true;
							int _x = ux;
							int _y = uy;
							int _z = uz;
							if(_x > 63) _x = 63
							if(_y > 63) _y = 63
							if(_z > 63) _z = 63
							if(_x < 0) _x = 0
							if(_y < 0) _y = 0
							if(_z < 0) _z = 0
							
							var66 = _z << 12 | _y << 6 | _x;
							var36 = dist;
							
							int NEG = 1;
							if(dimLength > 0) {
							   NEG = -1;
							}
							var15 = 0;
						}
					}*/
					//move the ray forward to the next block's wall
					xp += xd;
					yp += yd;
					zp += zd;
					dist += 1/ll;
				}
			}
			
			if(ddist < 0) ddist = 0; // if fade distance is negative, the color will get distorted.
			
			/*int TMP = 0; // the skybox pixel color as 24-bit integer
			int skyboxR = 0; // skybox pixel colors as RGB integer
			int skyboxG = 0;
			int skyboxB = 0;
			
			if(!colUpdated && !Night) { // render skybox (same rendering engine, with modified code)
				closest = 1;
				for (int d = 0; d < 3; d++) { // One loop for each visible face
					float dimLength = _xd; // how far the camera is from a particular face?
					if (d == 1)
						dimLength = _yd;
					if (d == 2)
						dimLength = _zd;

					float ll = 1 / abs(dimLength);
					float xd = (_xd) * ll;
					float yd = (_yd) * ll;
					float zd = (_zd) * ll;
					float dist = ll / 2;
					float xp = (0.5 + xd / 2);
					float yp = (0.5 + yd / 2);
					float zp = (0.5 + zd / 2);
					
					if (dist <= closest) {
						int tex = 7;
						int u = (int)((xp + zp) * 16) & 15;
						int v = ((int)(yp * 16) & 15);
						if (d == 1) {
							u = (int)(xp * 16) & 15;
							v = (int)(zp * 16) & 15;
						}
						TMP = texmap[u + v * 16 + tex * 256 * 3];
						closest = dist;
						xp += xd;
						yp += yd;
						zp += zd;
						dist += ll;
					}
				}
			}
			skyboxR = ((TMP >> 16) & 0xff);
			skyboxG = ((TMP >> 8) & 0xff);
			skyboxB = ((TMP) & 0xff);*/
			
			unsigned char r = 0;
			unsigned char g = 0;
			unsigned char b = 0;
			
			if(Night) {
				r = ((col >> 16) & 0xff) * br * ddist / (255 * 255);
				g = ((col >> 8) & 0xff) * br * ddist / (255 * 255);
				b = ((col) & 0xff) * br * ddist / (255 * 255);
			} else {
				r = (((col >> 16) & 0xff) * br / (255));// + skyboxR;
				g = (((col >> 8) & 0xff) * br / (255));// + skyboxG;
				b = (((col) & 0xff) * br / (255));// + skyboxB;
			}
			
			if(((x >= hw - 1 && x < hw) || (y >= hh - 1 && y < hh + 1)) && (((x < hw + 5) && (x >= hw - 5)) && ((y < hh + 5) && (y >= hh - 5)))) { //crosshair
				r = 255-r; //INVERT RGB
				g = 255-g;
				b = 255-b;
			}
			
			screen[(x + y * w) * 3 + 0] = r;
			screen[(x + y * w) * 3 + 1] = g;
			screen[(x + y * w) * 3 + 2] = b;
		}
	}
}

int main() {
	init();
	initMap();
	render();
	
	print("main function completed.");
	return 0;
}

bool changes = false;

void timer() {
	if(changes) {
		render();
		changes = false;
	}
}

float mod(float a, float b) {
	return a - ((int)(a / b) * b);
}

void cameraDelta(int dx, int dy) {
	if (dx >= 0) { //turn left
		xRot -= dx * camera_move_speed;
		if(xRot < 0) {
			xRot = pi * 2 - mod(abs(xRot), pi * 2);
		}
	}
	if (dx < 0) { //turn right
		xRot -= dx * camera_move_speed;
		xRot = mod(xRot, pi * 2);
	}
	
	if (dy >= 0) {
		if (yRot < pi / 2) {
			yRot += dy * camera_move_speed;
			if(!(yRot < pi / 2)) yRot = pi / 2;
		} else {
			yRot = pi / 2;
		}
	}
	if (dy < 0) {
		if (yRot > -pi / 2) {
			yRot += dy * camera_move_speed;
			if(!(yRot > -pi / 2)) yRot = -pi / 2;
		} else {
			yRot = -pi / 2;
		}
	}
	changes = true;
}

float n9 = 0;
float n10 = 0;
float n11 = 0;
bool Jump = 0;
bool Shift = 0;

void movePlayer(int W, int A, int S, int D) {
	//gravity and hit detecion
	float xCos = cos(xRot);
	float xSin = sin(xRot);
	  
	float n24 = (W - S) * move_speed;
	float n25 = (D - A) * move_speed;
	
	float n26 = n9 * 0.5f;
	float n27 = n10 * 0.99f;
	float n28 = n11 * 0.5f;
	n9 = n26 + (xSin * n24 + xCos * n25);
	n11 = n28 + (xCos * n24 - xSin * n25);
	
	n10 = n27 + 0.016f;
	int n29 = 0;

	while ((n29 < 3)) { // Each loop is an axis (X, Y, Z)
		float n30 = ox + n9 * ((int)((n29 + 0) % 3 / (float)2));
		float n31 = oy + n10 * ((int)((n29 + 1) % 3 / (float)2));
		float n32 = oz + n11 * ((int)((n29 + 2) % 3 / (float)2));
		bool continueL = false;
		for (int n33 = 0; n33 < 12; ++n33) {
			float n34 = ((int)(n30 + (n33 >> 0 & 1) * 0.6f - 0.3f)) - (float)1;
			float n35 = ((int)(n31 + ((n33 >> 2) - 1) * 0.8f + 0.65f)) - (float)1;
			float n36 = ((int)(n32 + (n33 >> 1 & 1) * 0.6f - 0.3f)) - (float)1;
			if ((int)n34 < 0 || (int)n35 < 0 || (int)n36 < 0 || (int)n34 >= 64 || (int)n35 >= 64 || (int)n36 >= 64 || map[(int)n36 << 12 | (int)n35 << 6 | (int)n34] > 0 || (Shift == 1 && n29 == 1)) { // Is there a collision?
				if (n29 == 1) {
					if (Jump > 0 && n10 > 0) { // if player is not in air, make player jump
						n10 = -0.23f;
					} else {
						n10 = (float)0;
					}
				}
				++n29;
				//goto MovPlayerContinue; // Immediately stop and go back to top of the loop (if n29 < 3)
				continueL = true;
				break;
			}
		}
		if(continueL) continue;
		ox = n30;
		oy = n31;
		oz = n32;
		++n29;
		//MovPlayerContinue:;
	}
}