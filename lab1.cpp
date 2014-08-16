#include<stdio.h>
#include<math.h>
#include <windows.h>
#include "glut.h"
#define round(a) ((int)(a+0.5))


int width=640, height=480;
void myInit (void);
void display(void);
void reshape (int w, int h);
void drawPixel(int x,int y);
int n;

void myInit (void)
{
	glClearColor(0.0,0.0,0.0,0.0) ;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-320,319,-240,239);
}



void drawAxis () {



	glBegin(GL_LINES);
		glVertex2f(-width/2,0);
		glVertex2f(width/2,0);
	glEnd();

	glBegin(GL_LINES);
		glVertex2f(0,-height/2);
		glVertex2f(0,height/2);
	glEnd();

}



void display(void)
{
	double a=1.3;
	glColor3f(1.0,1.0,1.0) ;
	glClear(GL_COLOR_BUFFER_BIT);
	int i,k;
	drawAxis () ;

	if(n==1)
	{
		glPointSize(5);
		glColor3f( 1,0,0 );
		for(i=-10;i<10;i++)
		{

			if(i==0)
			{

				glColor3f( 1,0,0 );
				glBegin(GL_LINES);
					glVertex2f(0,0);
					glVertex2f(0,100);
				glEnd();

			}
			else
			{
				glColor3f( 1,0,0 );
				glBegin(GL_POINTS);
				glVertex2i (i*10,0);
				glEnd();

			}
		}
	}
	else if(n==2)
	{
		glPointSize(5);
		glColor3f( 1,0,0 );
		for(i=-10;i<10;i++)
		{

			if(i>=0)
			{

				glColor3f( 1,0,0 );
				glBegin(GL_LINES);
					glVertex2f((0+i*10),0);
					glVertex2f((0+i*10),100);
				glEnd();

			}
			else
			{
				glColor3f( 1,0,0 );
				glBegin(GL_POINTS);
				glVertex2i (i*10,0);
				glEnd();

			}
		}

	}


	else if(n==3)
	{
		glPointSize(5);
		glColor3f( 1,0,0 );
		for(i=-10;i<10;i++)
		{

			if(i>=0)
			{

				glColor3f( 1,0,0 );
				glBegin(GL_LINES);
					glVertex2f((0+i*10),0);
					glVertex2f((0+i*10),(0+i*10));
				glEnd();

			}
			else
			{
				glColor3f( 1,0,0 );
				glBegin(GL_POINTS);
				glVertex2i (i*10,0);
				glEnd();

			}
		}

	}
	else if(n==4)
	{
		glPointSize(5);
		glColor3f( 1,0,0 );
		for(i=-10;i<19;i++)
		{

			if(i>=0)
			{

				glColor3f( 1,0,0 );
				glBegin(GL_LINES);
					glVertex2f((0+i*10),0);
					glVertex2f((0+i*10),pow(a,(0+i*1.2)));
				glEnd();

			}
			else
			{
				glColor3f( 1,0,0 );
				glBegin(GL_POINTS);
				glVertex2i (i*10,0);
				glEnd();

			}
		}

	}


	glColor3f( 1,0,0);



	glFlush();
}



int main(int argc, char **argv)
{


	int i ;
	printf(" 1. Unit Sample\n 2. Unit Step\n 3. Unit Ramp\n 4. Exponential\n");
	scanf("%d",&n) ;



	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(width,height);
	glutInitWindowPosition (300, 50);
	glutCreateWindow ("");

	glutDisplayFunc (display);

	myInit();
	glutMainLoop();

    return 0 ;
}
