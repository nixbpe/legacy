#include "main.h"







void set_card()
{
	vector<int> t;
	t.push_back(1);
	t.push_back(1);
	t.push_back(2);
	t.push_back(2);
	t.push_back(3);
	t.push_back(3);
	t.push_back(4);
	t.push_back(4);
	for(int i=0;i<8;i++)
	{
		Card[i].isClose = 1;
		int r = rand()%t.size();
		Card[i].type = t[r];
		t.erase(t.begin()+r);
		Card[i].x = 45+i*100;
		Card[i].y = 505;
	}
}
void handleMouse(int button, int state, int x, int y)
{
	if(state == GLUT_LEFT_BUTTON && game_state == state::game_play)
	{
		
		// check click card
		if(wait_time_card==1000) // มีการกดไพ่ไปแล้วสองใบ
		for(int i=0;i<8;i++)
		{ 
			// printf("%d %d : %d %d\n",x,y,Card[i].x,Card[i].y); // debug click card
			if ( (x < Card[i].x+45 && x > Card[i].x-45 ) && (y<Card[i].y+45 && y>Card[i].y-45))
			{
				Card[i].isClose?Card[i].isClose=0:Card[i].isClose=1;
				printf("%d\n",1);
			}
		}
	}
}
void handleKeypress(unsigned char key, int x, int y) {
	//printf("%d\n",key);
	if(key=='v') isEffect4 = 1;
	if(key=='g') isEffect2 = 1;
	if(key=='t') isEffect1 = 1;
	switch(game_state)
	{
	case state::open_game :
		{
			if(key == 13)
			{
				if(state_key%3==0) game_state = state::LoadObject_game;
				if(state_key%3==1) game_state = state::how_to_play;
				if(state_key%3==2) exit(0);
			}
			if(key == '1' ) 
				state_key = (state_key+1);
			if(key == '2' )
			{
				if (state_key == 0) state_key = 3;
				state_key = (state_key-1);
			}
			break;
		}
	case state::game_play :
		{
		if(key=='n')game_state = state::new_game;
		if(key=='m')speed=1;
		switch(key)
		{
			case 'a':if(yond.x<35&&game_state == state::game_play)
					 {
						 yond.x=yond.x+5;
					 }
					 yond.Rz=-30;
					 glutPostRedisplay();
					 break;
			case 'd':if(yond.x>-30&&game_state == state::game_play)
					 {
						 yond.x=yond.x-5;
					 }
					 yond.Rz=30;
					 glutPostRedisplay();
					 break;
			case 's':NUM_MILLISECONDS=NUM_MILLISECONDS+5; glutPostRedisplay();break;
			case 'w':NUM_MILLISECONDS=NUM_MILLISECONDS-5; glutPostRedisplay();break;
			case 'c':camera.state++;break;
		}
		break;
		}
	case state::game_over :
		if(key==13)
		{
			game_state = state::open_game;
			alSourceStop(Source);
			LoadALData("./resource/sound/wind.wav");
			alSourcePlay(Source);
			count_time = 0;
			state_key = 0;
			user.score = 0;
			speed = 1;
			yond.z = 0;
		}
	case state::how_to_play :
		if(key==13)
		{
			game_state = state::open_game;
			alSourceStop(Source);
			LoadALData("./resource/sound/wind.wav");
			alSourcePlay(Source);
			count_time = 0;
			state_key = 0;
			user.score = 0;
			speed = 1;
			yond.z = 0;
		}
	default :
		{
			break;
		}
	}
	

	if( key == 27 ) exit(0);
}
void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(100.0, (float)(w) / (float)h, 1.0, 100.0);
}
void handleKeyup(unsigned char key, int x, int y)
{
	yond.Rz=0;
}


//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
				 0,                            //0 for now
				 GL_RGB,                       //Format OpenGL uses for image
				 image->width, image->height,  //Width and height
				 0,                            //The border of the image
				 GL_RGB, //GL_RGB, because pixels are stored in RGB format
				 GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
				                   //as unsigned numbers
				 image->pixels);               //The actual pixel data
	return textureId; //Returns the id of the texture
}

void update(int value) 
{
	if(game_state == state::wait_over)
	{
		wait_end_time+=NUM_MILLISECONDS;
		if(wait_end_time > WAIT_END_TIME )
		{
			game_state = state::game_over;
			wait_end_time=0;
		}
	}
	else
	{
		if( NUM_MILLISECONDS < 5 ) NUM_MILLISECONDS = 5;
		if(yond.z>1000)
		{
			yond.z=0;
			set_block();
			NUM_MILLISECONDS-=5;
			if(NUM_MILLISECONDS<0)
			{
			NUM_MILLISECONDS=1;
			}
		}
		if(game_state == state::game_play )
		{
			if(speed!=0)
			{
				user.score++;
			//	cout << user.score << endl ;
			}
			yond.z=yond.z+speed;
			count_time+=NUM_MILLISECONDS;
			if(count_time/1000 > 60 )
			{
				game_state = state::wait_over ;
				printf("yout score : %.0f",user.score);
			}
		}
		glutPostRedisplay(); 

		//check for open 2 cards
		int count = 0;
		int tmp1=-1,tmp2=-1;
		for(int i=0;i<8;i++)
			if(!Card[i].isClose)
			{
				tmp1==-1?tmp1=Card[i].type:tmp2=Card[i].type;
				count++;
			}
		if(count==2)
		{
			printf("debug 2 cards\n");
			//clear cards
			if(tmp1==tmp2)
			{
				if(tmp1==1)
				{
					isEffect1= 1;
				}
				if(tmp1==2)
				{
					isEffect2 = 1;
					for(int i=0;i<cube.size();i++)
					{
						if(cube[i].type == 7)
						{
							cube[i].type = 8;
						}
					}
				}
				if(tmp1==3)
				{
					// destroy-box
					for(int i=0;i<cube.size();i++)
					{
						if(cube[i].z < yond.z+50 && cube[i].type == 7)
						{
							cube.erase(cube.begin()+i);
							i--;
						}
					}
				}
				if(tmp1==4)
				{
						isEffect4 = 1;
				}
				wait_time_card-=NUM_MILLISECONDS;
				if(wait_time_card < 0)
				{
					set_card();
					wait_time_card = 1000;
				}
			}
			else
			{
				wait_time_card-=NUM_MILLISECONDS;
				if(wait_time_card < 0)
				{
					for(int i=0;i<8;i++)
					{
						Card[i].isClose = 1;
					}
					wait_time_card = 1000;
				}
				printf("%d\n",wait_time_card);
			}
		}
		if(isEffect1) // stop time
		{
			count_time-=NUM_MILLISECONDS;
			wait_time_effect1-=NUM_MILLISECONDS;
			if(wait_time_effect1 < 0)
			{
				wait_time_effect1 = STOP_TIME_EFFECT;
				isEffect1 = 0;
			}
		}
		if(isEffect2)
		{
			wait_time_effect2-=NUM_MILLISECONDS;
			if(wait_time_effect2 < 0)
			{
				wait_time_effect2 = STOP_TIME_EFFECT2;
				isEffect2 = 0;

				for(int i=0;i<cube.size();i++)
					{
						if(cube[i].type == 8)
						{
							cube[i].type = 7;
						}
					}
			}
		}
		if(isEffect4)
		{
			//printf("test\n");
			wait_time_effect4-=NUM_MILLISECONDS;
			if(wait_time_effect4 < 0)
			{
				wait_time_effect4 = STOP_TIME_EFFECT4;
				isEffect4 = 0;
			}
		
		
		}
	}
    glutTimerFunc(NUM_MILLISECONDS, update, 0);
}
void gameloop()
{
	if(yond.state==0)
	{
		yond.Rz+=0.005;
	}
	else if(yond.state==1)
	{

		yond.Rz-=0.005;
	}
	if(yond.Rz>10) yond.state=1;
	if (yond.Rz<-10) yond.state=0;

	cube_Ry++;
	if(cube_Ry>360)cube_Ry=0;

	if(camera.state>5)
	{
	camera.state=0;
	}
	else 
	{
		switch (camera.state)
		{
		case 0: camera.x=0;camera.y=10;camera.z=-15;break;
		case 1: camera.x=20;camera.y=15;camera.z=-10;break;
		case 2: camera.x=0;camera.y=40;camera.z=-1;break;
		case 3: camera.x=0;camera.y=1;camera.z=3;break;
		case 4: camera.x=0;camera.y=-10;camera.z=-15;break;
		
		}
	}

	//yond.z++;
	for(int i=0;i<cube.size();i++)
	{
		if(game_state == state::game_play )
		{
			if((yond.z+2<=cube[i].z+2&&yond.z+2>=cube[i].z-2)&&(yond.x+1<=cube[i].x+1&&yond.x+1>=cube[i].x-1||yond.x-1<=cube[i].x+1&&yond.x-1>=cube[i].x-1))
			{
			//prinft("cude break");
				if(cube[i].type==7 && !isEffect4)//box
				{
					speed=0;
					game_state = state::wait_over ;
					alSourceStop(Source);
					LoadALData("./resource/sound/bomb.wav");
					alSourcePlay(Source);
					printf("yout score : %.0f\n",user.score);
					break;
					
				}
				if(cube[i].type==3 || cube[i].type==8)//star
				{
					user.score+=10*user.multi;
					user.multi++;
					
					cube.erase(cube.begin()+i--);
				}
			}
		}
	}
	glutPostRedisplay();
}

void render_Object()
{
	iniObject();
	initRendering();
	set_block();


	
	glutSwapBuffers();
	set_card();
	game_state = state::new_game;
}
void render_open()
{
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId);
	
	//Bottom
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glColor3f(1.0f, 0.2f, 0.2f);
	glTranslatef(0.0f, 0.0f, -100.0f);
	glBegin(GL_QUADS);
	
	glTexCoord2f(0.0f, 1.0f); 	glVertex3f(-160, 120, 0);
	glTexCoord2f(1.0f, 1.0f); 	glVertex3f(160, 120, 0);
	glTexCoord2f(1.0f, 0.0f); 	glVertex3f(160, -120, 0);
	glTexCoord2f(0.0f, 0.0f); 	glVertex3f(-160, -120, 0);
	
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _ArrowIcon);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTranslatef(0.0f, 0.0f, 10.0f);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); 	glVertex3f(-10, 10 -menu_pos_y[state_key%3], 0);
	glTexCoord2f(0.0f, 1.0f); 	glVertex3f(10, 10 -menu_pos_y[state_key%3], 0);
	glTexCoord2f(1.0f, 1.0f); 	glVertex3f(10, -10 -menu_pos_y[state_key%3], 0);
	glTexCoord2f(1.0f, 0.0f); 	glVertex3f(-10, -10 -menu_pos_y[state_key%3], 0);
	
	glEnd();

	glDisable(GL_TEXTURE_2D);


}
void render_play()
{
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	
	gluLookAt(yond.x+camera.x,yond.y+camera.y,yond.z+camera.z,yond.x,yond.y,yond.z+4,0,5,0);
	//gluLookAt(0,5,-10,0,5,10,0,5,0);
	Draw_Grid();
	
	Draw_Character_Yondz();
	
	Draw_Character();

	//glDisable(GL_FOG);

	// draw score
	printw(yond.x + 16.5,15.5,yond.z,"SCORE : %.0f\n",user.score);
	printw(yond.x - 11,15.5,yond.z,"TIME : %.2f\n",60-count_time/1000);

	
	if(isEffect4) { glColor3f(0,1.0f,0); printw(yond.x + 18,13.5,yond.z,"IVISIBLE : %.2f\n",wait_time_effect4*1.0f/1000); }
	if(isEffect2) { glColor3f(0,1.0f,1.0f); printw(yond.x + 18,13.5,yond.z,"ALL STAR : %.2f\n",wait_time_effect2*1.0f/1000); }
	if(isEffect1) { glColor3f(0,0,1.0f); printw(yond.x + 18,13.5,yond.z,"TIME STOP : %.2f\n",wait_time_effect1*1.0f/1000); }
}
void render_newgame()
{   
	Initcharacter();
	game_state = state::game_play ;
}
void render_gameover()
{
	
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _GameOver);
	
	//Bottom
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glColor3f(1.0f, 0.2f, 0.2f);
	glTranslatef(0.0f, 0.0f, -100.0f);
	glBegin(GL_QUADS);
	
	glTexCoord2f(0.0f, 1.0f); 	glVertex3f(-160, 120, 0);
	glTexCoord2f(1.0f, 1.0f); 	glVertex3f(160, 120, 0);
	glTexCoord2f(1.0f, 0.0f); 	glVertex3f(160, -120, 0);
	glTexCoord2f(0.0f, 0.0f); 	glVertex3f(-160, -120, 0);
	
	glEnd();
	glDisable(GL_TEXTURE_2D);
	
	if( user.score >= MAX_SCORE )
	{
		printw(yond.x + 13.5,13.5,yond.z-4.5,"YOU ARE MAX SCORE : %.0f\n",user.score);
		MAX_SCORE = user.score;
	}
	else
	{
		printw(yond.x + 13.5,13.5,yond.z,"YOUR SCORE : %.0f\n",user.score);
		printw(yond.x + 13.5,10.5,yond.z,"MAX SCORE : %d\n",MAX_SCORE);
	}
}
void draw_background()
{	
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _background_space);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTranslatef(0.0f, 0.0f, -100.0f);


	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); 	glVertex3f(-160, 155, 0);
	glTexCoord2f(1.0f, 1.0f); 	glVertex3f(160, 155, 0);
	glTexCoord2f(1.0f, 0.0f); 	glVertex3f(160, -75, 0);
	glTexCoord2f(0.0f, 0.0f); 	glVertex3f(-160, -75, 0);
	glEnd();


	glDisable(GL_TEXTURE_2D);
}
void draw_howToPlay()
{	
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _Howto);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTranslatef(0.0f, 0.0f, -100.0f);


	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); 	glVertex3f(-160, 120, 0);
	glTexCoord2f(1.0f, 1.0f); 	glVertex3f(160, 120, 0);
	glTexCoord2f(1.0f, 0.0f); 	glVertex3f(160, -120, 0);
	glTexCoord2f(0.0f, 0.0f); 	glVertex3f(-160, -120, 0);
	glEnd();


	glDisable(GL_TEXTURE_2D);
}
void draw_lower()
{	
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _lower);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTranslatef(0.0f, 0.0f, -5.0f);


	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); 	glVertex3f(-8, -3, 0);
	glTexCoord2f(1.0f, 1.0f); 	glVertex3f(8, -3, 0);
	glTexCoord2f(1.0f, 0.0f); 	glVertex3f(8, -6, 0);
	glTexCoord2f(0.0f, 0.0f); 	glVertex3f(-8, -6, 0);
	glEnd();	
	
	glTranslatef(0.0f, 0.0f, 3);
	glColor3f(1,1,1.0f);
	float tmp=0.8f;
	// draw_card
	for(int i = 0 ; i<8 ; i++)
	{
		if(Card[i].isClose)
			glBindTexture(GL_TEXTURE_2D, _Card[0]);
		else
			glBindTexture(GL_TEXTURE_2D, _Card[Card[i].type]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); 	glVertex3f(-3.15 + i*tmp, -1.4, 0);
			glTexCoord2f(1.0f, 1.0f); 	glVertex3f(-2.45 + i*tmp, -1.4, 0);
			glTexCoord2f(1.0f, 0.0f); 	glVertex3f(-2.45 + i*tmp, -2.1, 0);
			glTexCoord2f(0.0f, 0.0f); 	glVertex3f(-3.15 + i*tmp, -2.1, 0);
		glEnd();
		
	}
	glDisable(GL_TEXTURE_2D);
	//glTranslatef(0.0f, 0.0f, 10.0f);
}
void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(1.0f,1.0f,1.0f);
	if(game_state == state::open_game ) 
	{
		render_open();
	}
	if(game_state == state::game_over )  render_gameover();
	if(game_state == state::new_game ) render_newgame();
	if(game_state == state::LoadObject_game )  render_Object();
	if(game_state == state::how_to_play) draw_howToPlay();
	if(game_state == state::game_play || game_state == state::wait_over) 
	{
		draw_background();
		draw_lower();
		render_play();
	}
	
	glutSwapBuffers();
}

void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientMenu);
	//glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	//glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	//glEnable (GL_FOG);
	
	
	
	Image* image = loadBMP("./resource/MainMenu.bmp");
	Image* imgSpace = loadBMP("./resource/space.BMP");
	Image* imgArrowIcon = loadBMP("./resource/Nuclear-symbol.bmp");
	Image* imgLower = loadBMP("./resource/lower.bmp");
	Image* imgHowTo = loadBMP("./resource/Howto.bmp");
	Image* imgCard[5];
	imgCard[0] = loadBMP("./resource/Card0.bmp");
	imgCard[1] = loadBMP("./resource/Card1.bmp");
	imgCard[2] = loadBMP("./resource/Card2.bmp");
	imgCard[3] = loadBMP("./resource/Card3.bmp");
	imgCard[4] = loadBMP("./resource/Card4.bmp");
	Image* imgGameOver = loadBMP("./resource/game_over.bmp");
	for(int i=0;i<5;i++) 
		_Card[i] = loadTexture(imgCard[i]);
	_textureId = loadTexture(image);
	_background_space = loadTexture(imgSpace);
	_ArrowIcon = loadTexture(imgArrowIcon);
	_lower = loadTexture(imgLower);
	_GameOver = loadTexture(imgGameOver);
	_Howto = loadTexture(imgHowTo);
	//delete imgCard;
	delete image;
	delete imgSpace;
	delete imgArrowIcon;
	delete imgLower;
	delete imgGameOver;
	delete imgHowTo;
	alSourceStop(Source);
	LoadALData("./resource/sound/space_battle.wav");
	alSourcePlay(Source);
}
void iniObject()
{
	drawmodel1("./resource/Objects/spaceship1/spaceship01.obj",0);
	drawmodel1("./resource/Objects/box-0/box1.obj",1);
	drawmodel1("./resource/Objects/box1/box1.obj",2);
	drawmodel1("./resource/Objects/star/star.obj",3);
	drawmodel1("./resource/Objects/box-2/box-2.obj",4);
	drawmodel1("./resource/Objects/rocket/rocket.obj",5);
	drawmodel1("./resource/Objects/meteo/meteo.obj",6);
	drawmodel1("./resource/Objects/boxmetal/boxmetal.obj",7);
	

}
void Initcharacter()
{
	yond.x=0 ;
	yond.y=0 ;
	yond.z=-5;
	yond.Rx=0;
	yond.Ry=0;
	yond.Rz=0;
	yond.state=0;
	yond.Sx=4.0f;
	yond.Sy=4.0f;
	yond.Sz=4.0f;
	speed = 1;
	NUM_MILLISECONDS = 50;

	user.name_user="Yondz";
	user.score=0;
	user.state_multi=0;
	user.multi=1;

	camera.x=0;
	camera.y=10;
	camera.z=-15;
	camera.state='0';
}

void new_block(float x,float y,float z,int type)
{
	object tmp;
	tmp.x = x;
	tmp.y = y;
	tmp.z = z;
	tmp.type = type;
	cube.push_back(tmp);
}
void set_block()
{
	cube.clear();
	int z = 0;
	for(int d=100;d<1000;d=d+20)
	{
		for(int i=0;i<3;i++)
		{
			new_block((rand()%70 - 30)/5*5 , 0 , z=rand()%15+d , 7);
		}
		for(int i=0;i<1;i++)
		{
			new_block((rand()%60 - 30)/5*5 , 0 , z=rand()%15+d+20 , 3);
		}
	}
	for(int i=0;i<cube.size();i++)
	{
		for(int j=i+1;j<cube.size();j++)
		{
			if(cube[i].x == cube[j].x && cube[i].z == cube[j].z)
			{
				cube.erase(cube.begin()+j--);
			}
		}
	}
}

void Draw_Grid()
{																	
	glLineWidth(1.0);
	glColor3ub(10, 100, 10);
	for(float i = -20; i <= 10000; i += 5)
	{
		glBegin(GL_LINES);
		//glColor3ub(50, 190, 50);							
			glVertex3f(-30, 0, i);									
			glVertex3f(35, 0, i);
		
		   glEnd();
		
	}
	for(float j = -30; j <= 35; j += 5)
		{
	     glBegin(GL_LINES);
			glVertex3f(j, 0, -20);								
			glVertex3f(j, 0, 10000);
		glEnd();
		}
	glColor3ub(255, 255, 255);
}
void Draw_Character() // This object will symbolize our character
{

	for(int i=0;i<cube.size();i++)
	{
		glPushMatrix();	
		glTranslatef(cube[i].x,cube[i].y,cube[i].z);
		// I added these to be able to rotate the whole scene so you can see the box and textures./resource/Objects/
		glScalef(2.5f,2.5f,2.5f);
		if(cube[i].type==3)
		{
			glRotatef(cube_Ry,0,1,0) ;
		}
		if(cube[i].type!=8)
			drawmodel1("./resource/Objects/box1/box1.obj",cube[i].type);
		else
			drawmodel1("./resource/Objects/box1/box1.obj",3);
		glPopMatrix();
	}
	
}
void Draw_Character_Yondz() // This object will symbolize our character
{   
	glPushMatrix();	
	//glColor3ub(255, 255, 255);
	glTranslatef(yond.x,yond.y+2,yond.z);
	// I added these to be able to rotate the whole scene so you can see the box and textures./resource/Objects/
	glScalef(yond.Sx,yond.Sy,yond.Sz);
	glRotatef(yond.Rz,0,0,1) ;
	glRotatef(180,0,1,0) ;
	
	
	drawmodel1("./resource/Objects/spaceship1/spaceship01.obj",0);
	glPopMatrix();
	
}
void drawmodel1(char* filename,int n)
{
	// Load the model only if it hasn't been loaded before
	// If it's been loaded then pmodel1 should be a pointer to the model geometry data...otherwise it's null
    if (!pmodel1[n]) 
	{
		// this is the call that actualy reads the OBJ and creates the model object
        pmodel1[n] = glmReadOBJ(filename);	
        if (!pmodel1[n]) exit(0);
		// This will rescale the object to fit into the unity matrix
		// Depending on your project you might want to keep the original size and positions you had in 3DS Max or GMAX so you may have to comment this.
        glmUnitize(pmodel1[n]);
		// These 2 functions calculate triangle and vertex normals from the geometry data.
		// To be honest I had some problem with very complex models that didn't look to good because of how vertex normals were calculated
		// So if you can export these directly from you modeling tool do it and comment these line
		// 3DS Max can calculate these for you and GLM is perfectly capable of loading them
        glmFacetNormals(pmodel1[n]);        
		glmVertexNormals(pmodel1[n], 90.0);
    }
    // This is the call that will actualy draw the model
	// Don't forget to tell it if you want textures or not :))| GLM_TEXTURE
    glmDraw(pmodel1[n], GLM_SMOOTH| GLM_TEXTURE);
	
}
void drawmodel2(char* filename,int n)
{
	// Load the model only if it hasn't been loaded before
	// If it's been loaded then pmodel1 should be a pointer to the model geometry data...otherwise it's null
    if (!pmodel2[n]) 
	{
		// this is the call that actualy reads the OBJ and creates the model object
        pmodel2[n] = glmReadOBJ(filename);	
        if (!pmodel2) exit(0);
		// This will rescale the object to fit into the unity matrix
		// Depending on your project you might want to keep the original size and positions you had in 3DS Max or GMAX so you may have to comment this.
        glmUnitize(pmodel2[n]);
		// These 2 functions calculate triangle and vertex normals from the geometry data.
		// To be honest I had some problem with very complex models that didn't look to good because of how vertex normals were calculated
		// So if you can export these directly from you modeling tool do it and comment these line
		// 3DS Max can calculate these for you and GLM is perfectly capable of loading them
        glmFacetNormals(pmodel2[n]);        
		glmVertexNormals(pmodel2[n], 90.0);
    }
    // This is the call that will actualy draw the model
	// Don't forget to tell it if you want textures or not :))| GLM_TEXTURE
    glmDraw(pmodel2[n], GLM_SMOOTH);
	
}


int main(int argc, char** argv) {
	srand(time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	
	glutCreateWindow("Beyondz.com");
	initRendering();

	glutDisplayFunc(drawScene);
	glutIdleFunc(drawScene);
	glutIdleFunc(gameloop);
	glutTimerFunc(NUM_MILLISECONDS, update, 0);
	glutKeyboardFunc(handleKeypress);
	glutKeyboardUpFunc(handleKeyup);
	glutReshapeFunc(handleResize);
	glutMouseFunc(handleMouse);
	game_state = state::open_game;
	new_block(25,0,20,1);
	new_block(30,0,30,1);
	new_block(0,0,20,1);
	new_block(-20,0,15,1);

	alutInit(NULL, 0);
	alGetError();
	// Load the wav data.
	if(LoadALData("./resource/sound/wind.wav") == AL_FALSE)
	{
	    printf("Error loading data.");
		return 0;
	}
	SetListenerValues();
	// Setup an exit procedure.
	atexit(KillALData);
		// play sound
	alSourceRewind(Source);
	glutMainLoop();
	return 0;
}

void printw (float x, float y, float z, char* format, ...)
{
va_list args; // Variable argument list
int len; // String length
int i; // Iterator
char * text; // Text

// Initialize a variable argument list
va_start(args, format);

// Return the number of characters in the string referenced the list of arguments.
// _vscprintf doesn't count terminating '\0' (that's why +1)
len = _vscprintf(format, args) + 1;

// Allocate memory for a string of the specified size
text = (char*)malloc(len * sizeof(char));

// Write formatted output using a pointer to the list of arguments
vsprintf_s(text, len, format, args);

// End using variable argument list
va_end(args);

// Specify the raster position for pixel operations.
glRasterPos3f (x, y, z);

// Draw the characters one by one
for (i = 0; text[i] != '\0'; i++)
glutBitmapCharacter(font_style, text[i]);

// Free the allocated memory for the string
free(text);
}

/*
* ALboolean LoadALData()
*
*	This function will load our sample data from the disk using the Alut
*	utility and send the data into OpenAL as a buffer. A source is then
*	also created to play that buffer.
*/
ALboolean LoadALData(char* filename)
{
	// Variables to load into.

	ALenum format;
	ALsizei size;
	ALvoid* data;
	ALsizei freq;
	ALboolean loop;

	// Load wav data into a buffer.

	alGenBuffers(1, &Buffer);

	if(alGetError() != AL_NO_ERROR)
		return AL_FALSE;

	alutLoadWAVFile((ALbyte*)filename, &format, &data, &size, &freq, &loop);
	alBufferData(Buffer, format, data, size, freq);
	alutUnloadWAV(format, data, size, freq);

	// Bind the buffer with the source.

	alGenSources(1, &Source);

	if(alGetError() != AL_NO_ERROR)
		return AL_FALSE;

	alSourcei (Source, AL_BUFFER,   Buffer   );
	alSourcef (Source, AL_PITCH,    1.0      );
	alSourcef (Source, AL_GAIN,     1.0      );
	alSourcefv(Source, AL_POSITION, SourcePos);
	alSourcefv(Source, AL_VELOCITY, SourceVel);
	alSourcei (Source, AL_LOOPING,  loop     );

	// Do another error check and return.

	if(alGetError() == AL_NO_ERROR)
		return AL_TRUE;

	return AL_FALSE;
}
/*
* void SetListenerValues()
*
*	We already defined certain values for the Listener, but we need
*	to tell OpenAL to use that data. This function does just that.
*/
void SetListenerValues()
{
	alListenerfv(AL_POSITION,    ListenerPos);
	alListenerfv(AL_VELOCITY,    ListenerVel);
	alListenerfv(AL_ORIENTATION, ListenerOri);
}



/*
* void KillALData()
*
*	We have allocated memory for our buffers and sources which needs
*	to be returned to the system. This function frees that memory.
*/
void KillALData()
{
	alDeleteBuffers(1, &Buffer);
	alDeleteSources(1, &Source);
	alutExit();
}