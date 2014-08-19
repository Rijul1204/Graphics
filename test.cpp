

// BISMILLALLAHIR RAHMANIR  RAHIM>>>>>>>>>>>>>>>>
#include "glut.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include <math.h>
#include <time.h>
#include "bmploader.h"
//#include"image.h"

//COLOR select...........
//for black color......
#define black1     glColor3f(0.0f,0.0f,0.0f)
//for white color..........
#define white1     glColor3f(1.0f,1.0f,1.0f)
//for  green  car...........
#define green1   glColor3f(0.0f, 0.320f, 0.00f)
#define green2   glColor3f(0.0f, 0.511f,0.001f)
#define green3	 glColor3f(0.0f, 0.61f, 0.00f)
#define green4   glColor3f(0.0f,0.42f,0.00f)
//for red  car...................
#define red1     glColor3f(0.80f, 0.00f, 0.00f)
#define red2     glColor3f(0.65f, 0.001f, 0.001f)
#define red3     glColor3f(0.85f, 0.0f, 0.00f)
#define red4     glColor3f(0.75f,0.00f,0.00f)
//for blue car...................
#define blue1     glColor3f(0.2f, 0.1f, 0.81f)
#define blue2     glColor3f(0.0f, 0.2f, 0.65f)
#define blue3     glColor3f(0.2f, 0.1f, 0.85f)
#define blue4     glColor3f(0.0f,0.20f,0.75f)
//for ash car....................
#define ash1      glColor3f(0.33f, 0.33f, 0.5f)
#define ash2      glColor3f(0.23f, 0.33f, 0.55f)
#define ash3      glColor3f(0.23f, 0.43f, 0.5f)
#define ash4      glColor3f(0.23f, 0.33f, 0.45f)
//for  left...........field........
#define road1     glColor3f(.12,.12,.12)
//  for........defining   Key...........

#define RIGHT_ARROW 4
#define LEFT_ARROW 3
#define UP_ARROW 1
#define DOWN_ARROW 2

bool keyStates[256];
bool arrowKeys[5];



// Initial size of graphics window.
int WIDTH  = 900;
int HEIGHT = 550;
int i,countc,count2c;
float n;
float x=.5;
//float Set_time,Load_time=0;

//for viewing high score.............
typedef struct{
	char nam[100];
	int score;
	char times[30];
	char month[30];
	char day[30];
	char date[30];
}player;

player players[100],temp;



// Current size of window.
int width  = WIDTH;
int height = HEIGHT;
//track-er  jonno.matrix......
int a[50000][100];

//all variable........
bool flagtime=true;
int posud=0,flagmode=0,diff=-2,counter=0,flagplayer=1,flagwin=0;
float poslr=0,poslr2=0;
float car_angle=0.0;
int k=2,flag=0;int c=0;
float j=0.0,l=0.0;
float posup=0,posup2=0,posudd=0,distance=0;
float gulie=0;
int count1=0,cursor=0;
int flagm=0,flagbrk=0,flagtype=0;
float red=0,green=0,blue=0;
int flagpause=1;
int score=0,place=3;
char name[30];
//movement key.....
int up=119,down=115,left=97,right=100;
//VAriable.........

int flagp=1,fin=1000;
//opponent car-er position  rakhar jonno............
float c1=400.0,c2=450.0,c3=390,c5=250,c6=200,c4=350,c7=10;
//opponent car-er color choose korar jonno............
int countc1=0,countc2=1,countc3=2,countc4=3,countc5=1,countc6=2,countc7=3;
float trackpace=0;
int flagdrawquad=0;
char scores[20];


/**
* TIMERSEC int
* the amount of time (in milisecond) after which animate function is called
*/
const int TIMERMSECS = 10;

//ALL FUNCTION>>>>>>>>-----------------------
void mycarred();
void mycarred2();
void mycargreen();
void mycargreen2();
void mycarblue();
void mycarblue2();
void mycarash();
void mycarash2();
void trackt();
void menu();
void mouse(int button,int state,int x,int y );
void choose_car();
void scoreprint();
void print_string(char *s, double pos_x, double pos_y);
void print_char(char s, double pos_x, double pos_y);
void tracknew();
void carngreen(int x,int y,float cpoint);
void carnblue(int x,int y,float cpoint);
void carnred(int x,int y,float cpoint);
void carnash(int x,int y,float cpoint);
void shesh();
void viewscore();
void file();
void sort(int n);
void reset();
void reset1();
void gamemode();
void trackselect();
void drawquad(int x1,int x2,int y1,int y2);
void times();
void wait ( int seconds );
void mainmenu();
void menupic();
void playmodern();
void playclassic();
void mycarpic1();
void mycarpic2();
void mycarpic3();
void mycarpic4();
void mycarpic12();
void mycarpic22();
void mycarpic32();
void mycarpic42();
void carnpic1(int x,int y,float cpoint);
void carnpic2(int x,int y,float cpoint);
void carnpic3(int q,int y,float point);
void carn1inv(int q,int y,float point);
void trackpic();
void drawquad(int x1,int x2,int y1,int y2);
void meterpic();
void choose_carpic();
void sheshpic();
void viewscorepic();
void gamemodepic();
void gettime();
void secondplayer();
void winnershow();


//texture  mapping-er  jonno  code.....aei  part  kisu  bujhi   na........

BMPClass::BMPClass(){bytes=0;}
BMPClass::~BMPClass(){delete[] bytes;}
BYTE& BMPClass::pixel(int x,int y,int c){return bytes[(y*width+x)*3+c];}
void BMPClass::allocateMem(){delete[] bytes;bytes=new BYTE[width*height*3];}

std::string TranslateBMPError(BMPError err)
{
	switch(err)
	{
	case(BMPNOTABITMAP):
		return "This file is not a bitmap, specifically it doesn't start 'BM'";
	case(BMPNOOPEN):
		return "Failed to open the file, suspect it doesn't exist";
	case(BMPFILEERROR):
		return "ferror said we had an error. This error seems to not always mean anything, try ignoring it";
	case(BMPBADINT):
		return "sizeof(int)!=4 quite a lot of rewriting probably needs to be done on the code";
	case(BMPNOERROR):
		return "No errors detected";
	case(BMPUNKNOWNFORMAT):
		return "Unknown bmp format, ie not 24bit, 256,16 or 2 colour";
	default:
		return "Not a valid error code";
	}
}

BMPError BMPLoad(std::string fname,BMPClass& bmp)
{
	if(sizeof(int)!=4) return BMPBADINT;

	FILE* f=fopen(fname.c_str(),"rb");		//open for reading in binary mode
	if(!f) return BMPNOOPEN;
	char header[54];
	fread(header,54,1,f);			//read the 54bit main header

	if(header[0]!='B' || header[1]!='M')
	{
		fclose(f);
		return BMPNOTABITMAP;		//all bitmaps should start "BM"
	}

	//it seems gimp sometimes makes its headers small, so we have to do this. hence all the fseeks
	int offset=*(unsigned int*)(header+10);

	bmp.width=*(int*)(header+18);
	bmp.height=*(int*)(header+22);
	//now the bitmap knows how big it is it can allocate its memory
	bmp.allocateMem();

	int bits=int(header[28]);		//colourdepth

	int x,y,c;
	BYTE cols[256*4];				//colourtable
	switch(bits)
	{
	case(24):
		fseek(f,offset,SEEK_SET);
		fread(bmp.bytes,bmp.width*bmp.height*3,1,f);	//24bit is easy
		for(x=0;x<bmp.width*bmp.height*3;x+=3)			//except the format is BGR, grr
		{
			BYTE temp=bmp.bytes[x];
			bmp.bytes[x]=bmp.bytes[x+2];
			bmp.bytes[x+2]=temp;
		}
		break;

	case(8):
		fread(cols,256*4,1,f);							//read colortable
		fseek(f,offset,SEEK_SET);
		for(y=0;y<bmp.height;++y)						//(Notice 4bytes/col for some reason)
			for(x=0;x<bmp.width;++x)
			{
				BYTE byte;
				fread(&byte,1,1,f);						//just read byte
				for(int c=0;c<3;++c)
					bmp.pixel(x,y,c)=cols[byte*4+2-c];	//and look up in the table
			}
			break;

	case(4):
		fread(cols,16*4,1,f);
		fseek(f,offset,SEEK_SET);
		for(y=0;y<256;++y)
			for(x=0;x<256;x+=2)
			{
				BYTE byte;
				fread(&byte,1,1,f);						//as above, but need to exract two
				for(c=0;c<3;++c)						//pixels from each byte
					bmp.pixel(x,y,c)=cols[byte/16*4+2-c];
				for(c=0;c<3;++c)
					bmp.pixel(x+1,y,c)=cols[byte%16*4+2-c];
			}
			break;

	case(1):
		fread(cols,8,1,f);
		fseek(f,offset,SEEK_SET);
		for(y=0;y<bmp.height;++y)
			for(x=0;x<bmp.width;x+=8)
			{
				BYTE byte;
				fread(&byte,1,1,f);
				//Every byte is eight pixels
				//so I'm shifting the byte to the relevant position, then masking out
				//all but the lowest bit in order to get the index into the colourtable.
				for(int x2=0;x2<8;++x2)
					for(int c=0;c<3;++c)
						bmp.pixel(x+x2,y,c)=cols[((byte>>(7-x2))&1)*4+2-c];
			}
			break;

	default:
		fclose(f);
		return BMPUNKNOWNFORMAT;
	}

	if(ferror(f))
	{
		fclose(f);
		return BMPFILEERROR;
	}

	fclose(f);

	return BMPNOERROR;
}

BMPError BMPLoad(std::string fname){BMPClass bmp;return BMPLoad(fname,bmp);}

#ifdef __gl_h
BMPError BMPLoadGL(std::string fname)
{
	BMPClass bmp;
	BMPError e=BMPLoad(fname,bmp);
	if(e!=BMPNOERROR) return e;

	glEnable(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D,0,3,bmp.width,bmp.height,0,GL_RGB,GL_UNSIGNED_BYTE,bmp.bytes);

	return BMPNOERROR;
}
#endif



BMPClass abc,menu1,menu2,menu3,menu4,carpic1,carpic2,carpic3,carpic4,scene1,scene2,meter1,menu5;


//-------function prototype  declare  kora   hoilo-----------\\

const int AST_MAX = 7;

int print_flag=0;

int Set_time,Load_time;

void LoadFPS(double set)
{
	Set_time=glutGet(GLUT_ELAPSED_TIME);
	while(1)
	{
		Set_time=glutGet(GLUT_ELAPSED_TIME);
		if(Set_time-Load_time>1000.00/set)
		{
			Load_time=Set_time;
			break;
		}

	}
}



void drawquad(int x1,int x2,int y1,int y2){


	glBegin(GL_QUADS);
	glColor3f(0.43,0.43,0.64);
	glVertex2f(x1,y1);
    glColor3f(0.30,0.25,0.60);
	glVertex2f(x1,y2);
	glColor3f(0.46,0.47,0.64);
	glVertex2f(x2,y2);
	glColor3f(0.32,0.28,0.63);
	glVertex2f(x2,y1);
	glEnd();

}


void mainmenu(){ /// have to change the color (sajid)

	//puts("MainMenu");
	
	glBegin( GL_POINTS );
	glColor3f(1,0,0);

	for(int i=0;i<=100;i++){
		for(int j=0;j<=100;j++){		
			glVertex2d( i,j);
		}
	}
	
	glEnd();

	

	glColor3f(0,0,0);

	glPushMatrix();
    glLoadIdentity();
	glTranslatef(20,100,0);

	//texture maping of left field
	int r,c,r1;
	double p1,c1;
	
	for(r=0;r<menu5.width;r++)
	{
	  for(c=0;c<menu5.height;c++)
	  {
		  glBegin(GL_POINTS);
		  
		  float rr=(float)(menu5.pixel(r,c,0));
		  float gg=(float)(menu5.pixel(r,c,1));
		  float bb=(float)(menu5.pixel(r,c,2));		  
		  
		  glColor3f(rr/255,gg/255.0,bb/255.0);

		  if(!print_flag && j<30) printf("%3.0f,%3.0f,%3.0f,  ",rr,gg,bb);
		  
		  glVertex2d(r,c);		  
		  
		  glEnd();
	  }

	  if(!print_flag)puts("");
	
	}

	if(!print_flag) printf("%d %d\n",r,c);
	
	print_flag=1;

	

	glPopMatrix();
	
	glPushMatrix();
    glLoadIdentity();
	glTranslatef(250,100,0);

	for(r=0,r1=0;r<=menu5.width;r++,r1+=1)
	{
	  for(c=0,c1=0;c<=menu5.height;c++,c1+=1.55)
	  {
		  glBegin(GL_POINTS);
		  float rr=(float)(menu5.pixel(r,c,0));
		  float gg=(float)(menu5.pixel(r,c,1));
		  float bb=(float)(menu5.pixel(r,c,2));
		  glColor3f(rr/255,gg/255.0,bb/255.0);
		  for(p1=0;p1<=2;p1+=.25){
			  glVertex2d(r1+p1,c1+p1);
		  }
		  glEnd();
	  }

	}

	glPopMatrix();

}


void display ()
{
	// Function that clears the window. If you don't want to clear the view window, don't call glClear function. make it as a comment statement.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//LoadFPS(10000);
//	printf("flagpalyer=%d\n",flagplayer);

	//mode Select  korar  jonno..........
	if(flagmode==0) mainmenu();



	glutSwapBuffers();


}



void mouseMovement (int x, int y)
{
	//ata  pore  thik korbo.....

	// --------------
	// YOUR CODE ...
	// --------------

	//Main menur .....shomoy........
	if(flagmode==0){
		if(x>=325&&x<=390&&y>=290&&y<=315){
			//	flagm=0;
			flagdrawquad=7; // FOR modern
			printf("flagmode->%d",flagmode);
		}
		else if(x>=425&&x<=500&&y>=285&&y<=320){
			flagdrawquad=8; //  FOR classic
		}

		else flagdrawquad=0;

	}


	///////menur............shomoy.....
	else if(flagm==0){
		//game  statr  korar  jonno...............
		if(x>=335&&x<=650&&y>=130&&y<=175) flagdrawquad=1;
		//car  chooose  korar  jonno........
		else if(x>=370&&x<=650&&y>=200&&y<=225) flagdrawquad=2;
		//high score dekhar jonno................
		else if(x>=330&&x<=540&&y>=350&&y<=395) flagdrawquad=5;
		//Game  mod  select korar  jonno.........
		else if(x>=330&&x<=540&&y>=300&&y<=336) flagdrawquad=4;
		//track  select  korar  jonno.............
		else if(x>=330&&x<=540&&y>=240&&y<=290) flagdrawquad=3;
		//khela  shesh korar  jonno...................
		else if(x>=330&&x<=535&&y>=405&&y<=450) flagdrawquad=6;

		else flagdrawquad=0;

	}
	else if(flagm==2){
		//car....select  button............
		if(x>=335&&x<=540&&y>=130&&y<=175) flagdrawquad=7;

		else flagdrawquad=0;
	}
	else if(flagm==3){
		if(flagtype==1){
			//Yes..
			if(x>=260&&x<=340&&y>=360&&y<=395){
				flagdrawquad=3;
			}
			//No...
			if(x>=435&&x<=510&&y>=360&&y<=395) {
				flagdrawquad=2;
			}
			else flagdrawquad=0;
		}
		if(flagplayer==2){
			//for return .....
			if(flagplayer==2){
				if(x>=710&&x<=800&&y>=475&&y<=510) flagdrawquad=1;
				else flagdrawquad=0;
			}
		}
	}
	//diffIculty.....Level......choose...korar....jonno......
	else if(flagm==5){
		//easy.............
		if(x>=330&&x<=540&&y>=135&&y<=173) {
			flagdrawquad=1;
		}
		//medium.................
		else if(x>=330&&x<=540&&y>=190&&y<=230){
			flagdrawquad=2;
		}
		//Hard.................
		else if(x>=330&&x<=540&&y>=240&&y<=285){
			flagdrawquad=3;
		}
		//back////////
		else if(x>=330&&x<=540&&y>=300&&y<=336) flagdrawquad=4;

		else  flagdrawquad=0;
	}
	// scondplayer function...........
	else if(flagplayer==2){
		if(x>=710&&x<=800&&y>=475&&y<=510) flagdrawquad=1;
		else flagdrawquad=0;
	}

	glutPostRedisplay();
}

// Respond to window resizing, preserving proportions.
// Parameters give new window size in pixels.
void reshapeMainWindow (int newWidth, int newHeight)
{
	// ----------------------------
	// DO NOT ALTER THIS FUNCTION ...
	// ----------------------------
    //jani  na  ki kore.....aei  line  gulo die......
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	//ata  jani ...ortho die  amr  screen -er  position thik  korte  hoi.......
	glOrtho( 0, 500, 0, 500, 0, 100);
	glViewport( 0, 0, newWidth, newHeight );
	width = newWidth;
	height = newHeight;
    glMatrixMode(GL_MODELVIEW);

	/*	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)width / (double)height, 1.0, 200.0);
	glutPostRedisplay();*/
}



void timer(int value) {

	// --------------
	// YOUR CODE ...
	// --------------
	//second player-er sathe  collision..-er  jonno......
    if(flagm==1&&flagplayer==2) if(abs(poslr-poslr2)>=0&&abs(poslr-poslr2)<=18&&abs(posup-posup2)>=0&&abs(posup-posup2)<=57){
		flagm=3;
		flagwin=0;
	}
	//printf("flagm=%d\n",flagm);

	//track  samner  dike  jaouar........
	posud+=1;



	if(trackpace<4){

		trackpace+=1;
	}

    //  if game  starts...then  do....
	if(flagm==1){

		posud+=trackpace;
		posudd+=10;
		l+=1.6;
		j+=1.50;
	}
    if(trackpace>8) trackpace=5;

	// first  car-er  counter........
	c1-=2.5;
    //second  car............
	c2-=2.25;
	c3-=3.20;///third   car....
    c4-=3.00;///fourth car
	c5-=4.05;//fifth  car...........
	c6-=3.05;//sixth car.........
	c7-=4.55;///seven car.........

	if(diff>0){
		// first  car-er  counter........
		c1-=1.0;
		//second  car............
		c2-=1.25;
		c3-=1.20;///third   car....
		c4-=1.00;//fourth car......
		c5-=1.05;//fifth  car...........
		c6-=1.05;//sixth car.........
		c7-=1.55;///seven car.........

	}

	if(diff>1){
		// first  car-er  counter........
		c1-=1.0;
		//second  car............
		c2-=1.25;
		c3-=1.20;///third   car....
		c4-=1.00;
		c5-=1.05;//fourth  car...........
		c6-=1.05;//fifth car.........
		c7-=1.55;///sixth car.........

	}

	if(diff>2){
		// first  car-er  counter........
		c1-=1.0;
		//second  car............
		c2-=1.25;
		c3-=1.20;///third   car....
		c4-=1.00;
		c5-=1.05;//fourth  car...........
		c6-=1.05;//fifth car.........
		c7-=1.55;///sixth car.........

	}

	if(diff>3){
		// first  car-er  counter........
		c1-=1.0;
		//second  car............
		c2-=1.25;
		c3-=1.20;///third   car....
		c4-=1.00;
		c5-=1.05;//fourth  car...........
		c6-=1.05;//fifth car.........
		c7-=1.55;///sixth car.........

	}
	if(diff>4){
		// first  car-er  counter........
		c1-=1.0;
		//second  car............
		c2-=1.25;
		c3-=1.20;///third   car....
		c4-=1.00;
		c5-=1.05;//fourth  car...........
		c6-=1.05;//fifth car.........
		c7-=1.55;///sixth car.........

	}


	if(flagm==1){
		if(counter%100==0) diff++;
		distance+=.5;
		counter++;
	}

    glutTimerFunc(TIMERMSECS, timer, 0);
	glutPostRedisplay();

}

void initRendering() {

	//Sob  initialize  korlam...(suru theke suru  korlam...:)).....  )
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL); //Enable color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //Change the background to sky blue

}

///////////MAIN   FUNC>>>>>>>>>>>>>>>>>>

int main (int argc, char **argv)
{
	
	freopen("out.txt","w",stdout);

	//nam lekhar jonno...cursor..........
	name[0]='_';
	//texture  mapping-er  jonno.........	
	
	BMPLoad("menu.bmp",menu5);

	// GLUT initialization.
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutCreateWindow("CAR RACING");
    initRendering();


	// Register call backs.
	glutDisplayFunc(display);
	glutReshapeFunc(reshapeMainWindow);

	//glutMouseFunc(mouse);
    glutTimerFunc(TIMERMSECS, timer, 0);
	//glutIdleFunc(keyCheck);

	// Enter GLUT loop.
	glutMainLoop();

	return 0;
}


