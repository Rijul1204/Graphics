#include "glut.h"
#include <stdlib.h>

#define  glC(a,b,c) glColor3f(a,b,c)
#define  glV(a,b)	glVertex2i (a,b);
#define  cn  50
#define  cn1  20

void drawline(int x0,int y0,int x1,int y1);

void init(void)
{
	glutInitWindowSize (800, 600);
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_FLAT);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);	
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("Testing");
	
}
void drawpixel(int x,int y){
	
	glBegin(GL_POINTS);
	glV(x,y);
	glEnd();
}
void display(void)
{
	int i;
	glClear(GL_COLOR_BUFFER_BIT);
	glC(1.0, 1.0,.5);
	
	for(i=0;i<cn;i++){
		drawline(rand()%800-400,rand()%600-300,rand()%800-400,rand()%600-300);
	}
	
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
	glutMainLoop();
	return 0;
}

void drawline(int x0,int y0,int x1,int y1){	
	
	//return ;		

	int dx=x1-x0,dy=y1-y0;
	int d=2*dy-dx;
	int slope=dy/dx;

	if(slope<0||slope>1) return ;
	if(x1<x0 || y1<y0) return ;

	glC(1.0, .5,.5);
	glBegin(GL_LINES);
	glV(x0+cn1,y0+cn1);
	glV(x1+cn1,y1+cn1);
	glEnd();
	
	glC(1.0, 1.0,.5);
	int E=2*dy; int NE=2*(dy-dx);
	int x=x0,y=y0;
	drawpixel(x,y);

	while(x<x1){
		if(d<=0){
			d+=E; x++;
		}
		else{
			d+=NE; x++,y++;
		}
		drawpixel(x,y);
	}

}