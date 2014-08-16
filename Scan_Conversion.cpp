#include "glut.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define  glC(a,b,c) glColor3f(a,b,c)
#define  glV(a,b)	glVertex2i (a,b);
const int TIMERMSECS = 1111114;

struct point{
	int x,y;
	point(){}
	point(int _x,int _y){ x=_x; y=_y; }
};
struct circle{
	point cen;
	int r;
	circle(){}
	circle(point p,int _r){ cen=p; r=_r; }
};

//using mid-point algorithm.........
void draw_line(int x0,int y0,int x1,int y1);
void draw_pixel(int x,int y){ 	glBegin(GL_POINTS); glV(x,y); glEnd(); }
void draw_circle_point(int x,int y); 
void draw_circle(circle c);
void lines();

int ang;

//  Display Function.........
void display(void)
{
	int i;

	glClear(GL_COLOR_BUFFER_BIT);
	glC(1.0, 1.0,.5);
	
	draw_circle(circle(point(0,0),150));
	draw_circle(circle(point(0,0),200+ang));
	//lines();
	
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
void timer(int value) {

	// --------------
	// YOUR CODE ...
	// --------------

    glutTimerFunc(TIMERMSECS, timer, 23123);
	glutPostRedisplay();
}
void spin(void)
{
	ang++;
	ang%=180;
	sleep(2);
	timer(2213232);
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
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);	
	glutIdleFunc(spin);
	glutMainLoop();
	
	return 0;
}

void draw_circle_point(int x,int y){

	draw_pixel(x,y);
	draw_pixel(y,x);
	draw_pixel(y,-x);
	draw_pixel(x,-y);
	draw_pixel(-x,-y);
	draw_pixel(-y,-x);
	draw_pixel(-y,x);
	draw_pixel(-x,y);
}

void draw_circle(circle c){

	int x=0,y=c.r,d=1-c.r,delE=3,delSE=-2*c.r+5;
	
	draw_circle_point(x,y);

	while(y>x){
		if(d<0){
			d+=delE; delE+=2; delSE+=2;
		}
		else{
			d+=delSE; delE+=2; delSE+=4; y--;
		}
		x++;
		draw_circle_point(x,y);
	}
}


void draw_line(int x0,int y0,int x1,int y1){	
	
	int dx=x1-x0,dy=y1-y0;
	int d=2*dy-dx;
	int slope=dy/dx;

	if(slope<0||slope>1) return ;
	if(x1<x0 || y1<y0) return ;

	
	glC(1.0, 1.0,.5);
	int E=2*dy; int NE=2*(dy-dx);
	int x=x0,y=y0;
	draw_pixel(x,y);

	while(x<x1){
		if(d<=0){
			d+=E; x++;
		}
		else{
			d+=NE; x++,y++;
		}
		draw_pixel(x,y);
	}

}

void lines(){

	int x,y;
	int ang1=ang;

	for(i=0;i<8;i++,ang1+=45){
		ang1%=360;
		x=acos(
		
	}
	
	

}