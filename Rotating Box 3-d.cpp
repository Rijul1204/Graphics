
#include "glut.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define pi  acos(-1.0)
#define glC(a,b,c) glColor3f(a,b,c)
#define glV(a,b)   glVertex2i (a,b);

//  for keyboard...........
#define RIGHT_ARROW 4
#define LEFT_ARROW 3
#define UP_ARROW 1
#define DOWN_ARROW 2

bool keyStates[256];
bool arrowKeys[5];

struct point3d{
	double x,y,z;
	int X,Y;
	point3d(){}
	point3d(double _x,double _y,double _z){
		X=x=_x; Y=y=_y; z=_z;
	}
};

struct cube{
	point3d p[10];
};

struct point{
	int x,y;
	point(){}
	point(int _x,int _y){ x=_x; y=_y; }
};
//using mid-point algorithm.........
void midpoint_line(int x0,int y0,int x1,int y1,int op);
void draw_line(int x0,int y0,int x1,int y1);
void draw_pixel(int x,int y){ 	glBegin(GL_POINTS); glV(x,y); glEnd(); }
void lines();
void change_cen(int x,int y);

//  Variable...
cube cube1;
point3d cop;
double zp,q,dx,dy,dz,tp,xp,yp;

void draw_pixel1(int x,int y,int op){
	glBegin(GL_POINTS);
	if(op==1)glV(x, y);
	if(op==2)glV(y, x);
	if(op==3)glV(-y, x);
	if(op==4)glV(-x, y);
	if(op==5)glV(-x, -y);
	if(op==6)glV(-y, -x);
	if(op==7)glV(y, -x);
	if(op==8)glV(x, -y);
	glEnd();
}

int dirx[]={1,1,1,1,-1,-1,-1,-1};
int diry[]={1,-1,-1,1,1,1,-1,-1};
int dirz[]={1,1,-1,-1,1,-1,-1,1};

void r_up(){

	double rad=pi/180.0;
	rad*=2;
	
	int i;
	double xd,yd,zd;
	for(i=0;i<8;i++){
		yd=cube1.p[i].y; zd=cube1.p[i].z;
		printf("prev: %lf %lf\n",yd,zd);
		cube1.p[i].y= (yd*cos(-rad)) - (zd*sin(-rad));
		cube1.p[i].z= (yd*sin(-rad)) + (zd*cos(-rad));
		printf("now: %lf %lf\n",cube1.p[i].y,cube1.p[i].z);
	}
	printf("HI u\n");
}
void r_down(){
	
	double rad=pi/180.0;
	rad*=2;
	
	int i;
	double xd,yd,zd;
	for(i=0;i<8;i++){
		yd=cube1.p[i].y; zd=cube1.p[i].z;
		printf("prev: %lf %lf\n",yd,zd);
		cube1.p[i].y= (yd*cos(rad)) - (zd*sin(rad));
		cube1.p[i].z= (yd*sin(rad)) + (zd*cos(rad));
		printf("now: %lf %lf\n",cube1.p[i].y,cube1.p[i].z);
	}
	printf("HI u\n");
}
void r_left(){

	double rad=pi/180.0;
	rad*=2;
	
	int i;
	double xd,yd,zd;
	for(i=0;i<8;i++){
		xd=cube1.p[i].x; zd=cube1.p[i].z;
		printf("prev: %lf %lf\n",yd,zd);
		cube1.p[i].x= (xd*cos(-rad)) + (zd*sin(-rad));
		cube1.p[i].z= (-xd*sin(-rad)) + (zd*cos(-rad));
		printf("now: %lf %lf\n",cube1.p[i].y,cube1.p[i].z);
	}

	printf("HI l\n");
}
void r_right(){

	double rad=pi/180.0;
	rad*=2;
	
	int i;
	double xd,yd,zd;
	for(i=0;i<8;i++){
		xd=cube1.p[i].x; zd=cube1.p[i].z;
		printf("prev: %lf %lf\n",yd,zd);
		cube1.p[i].x= (xd*cos(rad)) + (zd*sin(rad));
		cube1.p[i].z= (-xd*sin(rad)) + (zd*cos(rad));
		printf("now: %lf %lf\n",cube1.p[i].y,cube1.p[i].z);
	}
	printf("HI u\n");
	printf("HI r\n");
}

void cal_cube(int size){

	int i;

	for(i=0;i<8;i++){
		cube1.p[i]=point3d(size*dirx[i],size*diry[i],size*dirz[i]);
	}
}

void draw_cube(cube cube1){

	point3d p1,p2;
	
	p1=cube1.p[0]; p2=cube1.p[1];  	draw_line(p1.X,p1.Y,p2.X,p2.Y);

	p1=cube1.p[0]; p2=cube1.p[3]; 	draw_line(p1.X,p1.Y,p2.X,p2.Y);

	p1=cube1.p[0]; p2=cube1.p[4]; 	draw_line(p1.X,p1.Y,p2.X,p2.Y);

	p1=cube1.p[1]; p2=cube1.p[2];   draw_line(p1.X,p1.Y,p2.X,p2.Y);

	p1=cube1.p[7]; p2=cube1.p[1]; 	draw_line(p1.X,p1.Y,p2.X,p2.Y);

	p1=cube1.p[2]; p2=cube1.p[3]; 	draw_line(p1.X,p1.Y,p2.X,p2.Y);

	p1=cube1.p[2]; p2=cube1.p[6]; 	draw_line(p1.X,p1.Y,p2.X,p2.Y);

	p1=cube1.p[3]; p2=cube1.p[5]; 	draw_line(p1.X,p1.Y,p2.X,p2.Y);

	p1=cube1.p[4]; p2=cube1.p[5]; 	draw_line(p1.X,p1.Y,p2.X,p2.Y);

	p1=cube1.p[4]; p2=cube1.p[7]; 	draw_line(p1.X,p1.Y,p2.X,p2.Y);

	p1=cube1.p[5]; p2=cube1.p[6]; 	draw_line(p1.X,p1.Y,p2.X,p2.Y);

	p1=cube1.p[6]; p2=cube1.p[7]; 	draw_line(p1.X,p1.Y,p2.X,p2.Y);

}

void cal_projection(cube cube1){

	q=sqrt(cop.x*cop.x + cop.y*cop.y + (cop.z-zp)*(cop.z-zp));
	dx=cop.x/q; dy=cop.y/q ;dz=(cop.z-zp)/q;

	double x,y,z,xp,yp; int i;

	for(i=0;i<8;i++){
		x=cube1.p[i].x; y=cube1.p[i].y; z=cube1.p[i].z;
		//printf("i= %d %lf %lf %lf\n",i,x,y,z);
		xp=x-(z*(dx/dz))+(zp*(dx/dz)); xp/=(((zp-z)/(q*dz))+1);
		yp=y-(z*(dy/dz))+(zp*(dy/dz)); yp/=(((zp-z)/(q*dz))+1);
		cube1.p[i].X=xp; cube1.p[i].Y=yp;
	}
	draw_cube(cube1);

}

//  Display Function.........
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glC(1.0, 1.0,.5);

	cal_projection(cube1);
	

	glFlush();
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w/2, w/2, -h/2, h/2, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void init(void)
{
	glutInitWindowSize (800, 600);
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_FLAT);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("Testing");

}
void mouse(int button, int state, int x, int y)
{

	switch (button) {
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_DOWN)
			{
					break;
			}
		case GLUT_MIDDLE_BUTTON:
			if (state == GLUT_DOWN)
				glutIdleFunc(NULL);
		break;
	default:
		break;
	}
}

void specialDown(int key, int x, int y)
{
     printf("Got it\n");
     switch(key)
     {
                case(GLUT_KEY_UP):
                                  arrowKeys[UP_ARROW] = true;
                                  break;
                case(GLUT_KEY_DOWN):
                                  arrowKeys[DOWN_ARROW] =true;
                                  break;
                case(GLUT_KEY_LEFT):
                                  arrowKeys[LEFT_ARROW] = true;
                                  break;
                case(GLUT_KEY_RIGHT):
                                  arrowKeys[RIGHT_ARROW] = true;
                                  break;
     }
//	 glutPostRedisplay();
}

void specialUp(int key, int x, int y)
{
     switch(key)
     {          
			    //printf("abar paisi\n");
                case(GLUT_KEY_UP):
                                  arrowKeys[UP_ARROW] = false;
                                  break;
                case(GLUT_KEY_DOWN):
                                  arrowKeys[DOWN_ARROW] = false;
                                  break;
                case(GLUT_KEY_LEFT):
                                  arrowKeys[LEFT_ARROW] = false;
                                  break;
                case(GLUT_KEY_RIGHT):
                                  arrowKeys[RIGHT_ARROW] = false;
                                  break;
     }
//	 glutPostRedisplay();
}

void keyCheck()
{

     if (keyStates['i']||arrowKeys[UP_ARROW]){
		 r_up();    
	 }
     if (keyStates['j']||arrowKeys[LEFT_ARROW]) {
          r_left();                                                                                      
     }
     if (keyStates['k']||arrowKeys[DOWN_ARROW]){
          r_down();                                                                                                                                          
	 }   
     if (keyStates['l']||arrowKeys[RIGHT_ARROW]) {
		  r_right();		       
	 }

	 glutPostRedisplay();

}


/*
* Request double buffer display mode.
* Register mouse input callback functions
*/
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	init ();

	cal_cube(100); zp=100;
	//draw_cube(cube1);
	cop=point3d(0,0,600);


	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutMouseFunc(mouse);
	glutSpecialFunc(specialDown);
    glutSpecialUpFunc(specialUp);
    glutIdleFunc(keyCheck);


	glutMainLoop();

	return 0;
}

double conv(int ang){
	double ret=(double) ang * pi;
	ret/=180.0;
	return ret;
}



int calquad(int x0,int y0,int x1,int y1){

	int dx=x1-x0; if(dx<0) dx*=-1;
	int dy=y1-y0; if(dy<0) dy*=-1;

	if(x1>=x0 && y1>=y0){
		if(dx>=dy) return 1;
		return 2;
	}
	if(x1<=x0 && y1>=y0){
		if(dy>=dx) return 3;
		return 4;
	}
	if(x1<=x0&&y1<=y0){
		if(dx>=dy) return 5;
		return 6;
	}
	if(x1>=x0&&y1<=y0){
		if(dy>=dx) return 7;
		return 8;
	}
	return 1;
}



void draw_line(int x0,int y0,int x1,int y1){

	/*
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex2i(x0,y0);
	glVertex2i(x1,y1);
	glEnd();
	*/

	int op=calquad(x0,y0,x1,y1);
	//printf("op = %d\n",op);


	//if(op!=2) return ;
	if(op==1) midpoint_line(x0,y0,x1,y1,op);
	if(op==2) midpoint_line(y0,x0,y1,x1,op);
	if(op==3) midpoint_line(y0, -x0, y1, -x1, op);
	//if(op==2) midpoint_line(y0,x0,y1,x1,op);
	if(op==4) midpoint_line(-x0,y0,-x1,y1,op);
	if(op==5) midpoint_line(-x0,-y0,-x1,-y1,op);
	if(op==6) midpoint_line(-y0,-x0,-y1,-x1,op);
	if(op==7) midpoint_line(-y0,x0,-y1,x1,op);
	if(op==8) midpoint_line(x0,-y0,x1,-y1,op);


}


void midpoint_line(int x0,int y0,int x1,int y1,int op){

	int dx=x1-x0,dy=y1-y0;
	int d=2*dy-dx;

	glC(1.0, 1.0,.5);
	int E=2*dy; int NE=2*(dy-dx);
	int x=x0,y=y0;
	draw_pixel1(x,y,op);

	while(x<x1){
		if(d<=0){
			d+=E; x++;
		}
		else{
			d+=NE; x++,y++;
		}
		draw_pixel1(x,y,op);
	}
}
