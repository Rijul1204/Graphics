#include<stdio.h>
#include<math.h>
#include <windows.h>
#include "glut.h"
#define round(a) ((int)(a+0.5))

#define MAX 100000

#define height 480
#define width 640

double a[MAX] ;
double b[MAX] ;
double y[MAX] ;
int N,M,n ;

void myInit (void);
void display(void);
void reshape (int w, int h);
void drawPixel(int x,int y);

double U(int n) // unit step
{
    if(n>=0)        return 1 ;
    return 0 ;
}

void myInit (void)
{
	glClearColor(0.0,0.0,0.0,0.0) ;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-320,319,-240,239);
}

void drawLine ( int hspace,int yspace, int v) {


	printf (" Position x %d y %d value %d\n",hspace,yspace,v);

		glBegin(GL_LINES);
		glVertex2f(hspace,v*yspace);
		glVertex2f(hspace,0);
		glEnd();

		if ( hspace == 0 ){

			printf ("    \n");

			glColor3f(1.0f,0.0f,0.0f);
			glPointSize(5);
			glBegin(GL_POINTS);
			glVertex2f(0.0,0.0);
			glColor3f(0.0f,0.0f,0.0f);
			glEnd();
		}

	glEnd();
	glFlush();

}

void drawAxis () {

	//glColor3f(0.0f,0.0f,0.0f);

	glBegin(GL_LINES);
		glVertex2f(-width/2,0);
		glVertex2f(width/2,0);
	glEnd();

	glBegin(GL_LINES);
		glVertex2f(0,-height/2);
		glVertex2f(0,height/2);
	glEnd();
	//glFlush();
}



void display(void)
{
	int i,k;
	glColor3f(1.0,1.0,1.0) ;
	glClear(GL_COLOR_BUFFER_BIT);

	drawAxis () ;


	glColor3f( 1,0,0);

	for(i=0;i<n;i++)
	{
		y[i] = 0 ;

		for(k=1;k<=i;k++)
		{
			y[i] += - a[k]*y[i-k] ;
		}

		for(k=0;k<=i;k++)
		{
			y[i] += b[k]*U(i-k) ;
		}
	}

	for(i=0;i<n;i++)
		drawPixel(i*10,y[i]*1.0);

	glFlush();
}
void drawPixel(int x,int y)
{
	glPointSize(3);
	glColor3f( 1,0,0 );
	printf("%d %d\n",x,y) ;
    glBegin(GL_POINTS);
	glVertex2i (x,y);
    glEnd();
}

int main(int argc, char **argv)
{
	freopen("in.txt","r",stdin) ;

	int i ;

	scanf("%d",&n) ;

	for(i=0;i<n;i++)	scanf("%lf",&a[i]) ;
	for(i=0;i<n;i++)	scanf("%lf",&b[i]) ;

	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(width,height);
	glutInitWindowPosition (300, 50);
	glutCreateWindow (" ");

	glutDisplayFunc (display);
	//glutReshapeFunc(reshape);
	myInit();
	glutMainLoop();

    return 0 ;
}
