#include <iostream>
#include <stdlib.h>
#include <GL/glut.h>
#include "imageloader.h"
#include "glm.h"
#include <time.h>
#include <vector>
#include <string.h>
#include <stdarg.h>
#include <AL\al.h>
#include <AL\alc.h>
#include <AL\alext.h>
#include <AL\alut.h>
#include <AL\efx-creative.h>
#include <AL\efx.h>

#define STOP_TIME_EFFECT 5000;
#define STOP_TIME_EFFECT2 3000;
#define STOP_TIME_EFFECT4 3000;
#define WAIT_END_TIME 500

 int NUM_MILLISECONDS = 50;
 int speed = 1;
 float cube_Ry=0;
 int score = 0;
 int menu_pos_y[3] = {0,27,54};
 int state_key = 0;
 float count_time = 0;
 int wait_end_time = 0;

 GLMmodel* pmodel1[10] = {NULL};
 GLMmodel* pmodel2[20] = {NULL};
 GLuint _textureId; //The id of the texture
 GLuint _background_space;
 GLuint _lower;
 GLuint _ArrowIcon;
 GLuint _Card[5];
 GLuint _GameOver;
 GLuint _Howto;

 GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_24;


 GLfloat ambientMenu[] = { 1,1, 1, 1.0f };
 GLfloat ambientLight[] = { 1.0, 1.0, 1.0, 1.0f };
 GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
 GLfloat specularLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
 GLfloat position[] = { 0.0f, 0.0f, 40.0f, 1.0f };
 GLfloat positionMenu[] = { 0.0f, 0.0f, -50.0f, 1.0f };
using namespace std;

struct object{
	float x;
	float y;
	float z;
	int type;
};
struct new_user{
	string name_user;
	float  score;
	float multi;
	int state_multi;
}user;



vector<object> cube;

void new_block(float x,float y,float z,int type);

enum state{
	open_game  = 0,
	new_game ,
	main_menu  ,
	game_play,
	LoadObject_game,
	game_over ,
	how_to_play ,
	wait_over
};
state game_state;
struct Character_yondz //โครงสร้าง ข้อมูล  Material
{
    int z;
	int x;
	int y;
	float Rx;
	float Ry;
	float Rz;
	int state;
	float Sx;
	float Sy;
	float Sz;
	//char state;

}yond,camera;

void drawmodel1(char* filename,int n);
void drawmodel2(char* filename,int n);
void drawScene();

void Draw_Grid();
void Draw_Character() ;
void Draw_Character_Yondz();
void draw_lower();

void handleKeypress(unsigned char key, int x, int y) ;
void handleResize(int w, int h);
void handleKeyup(unsigned char key, int x, int y);
void handleMouse(int button, int state, int x, int y);

GLuint loadTexture(Image* image);


void render_Object();
void render_open();
void render_play();
void draw_howToPlay();

void update(int value);
void gameloop();

void Initcharacter();
void iniObject();
void initRendering();

void set_block();
void new_block(float x,float y,float z,int type);

void printw (float x, float y, float z, char* format, ...);

void set_card();
//int step_card = 100;
int size ;
struct card{
	int type ;
	int isClose;
	int x;
	int y;
}Card[8];

int wait_time_card = 1000; // 1sec
int wait_time_effect1 = STOP_TIME_EFFECT; 
int wait_time_effect2 = STOP_TIME_EFFECT2; 
int wait_time_effect4 = STOP_TIME_EFFECT4; 
int isEffect1 = 0;
int isEffect2 = 0;
int isEffect4 = 0;
						//See explanation 2



int MAX_SCORE = 0;

/*
* These are OpenAL "names" (or "objects"). They store and id of a buffer
* or a source object. Generally you would expect to see the implementation
* use values that scale up from '1', but don't count on it. The spec does
* not make this mandatory (as it is OpenGL). The id's can easily be memory
* pointers as well. It will depend on the implementation.
*/

// Buffers to hold sound data.
ALuint Buffer;

// Sources are points of emitting sound.
ALuint Source;
ALuint Coin;


/*
* These are 3D cartesian vector coordinates. A structure or class would be
* a more flexible of handling these, but for the sake of simplicity we will
* just leave it as is.
*/

// Position of the source sound.
ALfloat SourcePos[] = { 0.0, 0.0, 0.0 };

// Velocity of the source sound.
ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };


// Position of the Listener.
ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };

// Velocity of the Listener.
ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };

// Orientation of the Listener. (first 3 elements are "at", second 3 are "up")
// Also note that these should be units of '1'.
ALfloat ListenerOri[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };


ALboolean LoadALData(char* filename);
/*
* void SetListenerValues()
*
*	We already defined certain values for the Listener, but we need
*	to tell OpenAL to use that data. This function does just that.
*/
void SetListenerValues();

/*
* void KillALData()
*
*	We have allocated memory for our buffers and sources which needs
*	to be returned to the system. This function frees that memory.
*/
void KillALData();