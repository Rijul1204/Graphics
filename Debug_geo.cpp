#include "glut.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define pi  acos(-1.0)
#define glC(a,b,c) glColor3f(a,b,c)
#define glV(a,b)   glVertex2f (a,b)
#define ii double

//  for keyboard...........
#define RIGHT_ARROW 4
#define LEFT_ARROW 3
#define UP_ARROW 1
#define DOWN_ARROW 2

bool keyStates[256];
bool arrowKeys[5];

struct point{
	double x,y;
	point(){}
	point(double _x,double _y){ x=_x; y=_y; }
};
struct point3d{
	double x,y,z;
	int X,Y;
	point3d(){}
	point3d(double _x,double _y,double _z){
		X=x=_x; Y=y=_y; z=_z;
	}
};
struct line { // Creates a line with equation ax + by + c = 0
    double a, b, c;
    line() {}
    line( point p1,point p2 ) {
        a = p1.y - p2.y;
        b = p2.x - p1.x;
        c = p1.x * p2.y - p2.x * p1.y;
    }
};
struct circle { // Creates a circle with point 'center' as center and r as radius
    point center;
    double r;
    circle() {}
    circle( point P, double rr ) { center = P; r = rr; }
};
struct segment { // Creates a segment with two end points -> A, B
    point A, B;
    segment() {}
    segment( point P1, point P2 ) { A = P1, B = P2; }    
};
struct quad { // quadrilateral  with four points .. counterclock wise...
    point p[5];
    quad(){}
    quad(point a,point b,point c,point d){
        p[0]=a; p[1]=b; p[2]=c; p[3]=d;
    }
};
struct rect {//  rectangle ....
	point p1,p2;
	rect(){}
	rect(point _p1,point _p2){
		p1=_p1; p2=_p2;
	}

};
struct cube{
	point3d p[10];
};

// function prototype...

// line draw......
void draw_line(segment s);
// point draw......
void draw_pixel(double x,double y){ 	glBegin(GL_POINTS); glV(x,y); glEnd(); }
// rectangle draw..........
void draw_rect(rect r);
//  quad  draw......
void draw_quad(quad q);
//  circle draw...... 
void draw_circle(circle c);
// set point...
void set_point(point p);

int n;
point s,d;
rect rects[100];


//  Display Function.........
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glC(1.0, 1.0,.5);	
	
	//glTranslatef(-100,-100,0);
	glScalef(35,35,1);

	//  for  debug  .....................

	draw_circle(circle(s,.03));
	draw_circle(circle(d,.03));

	for(int i=1;i<=n;i++){
		draw_rect(rects[i]);
	}


	// ..........................
	
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

/*
* Request double buffer display mode.
* Register mouse input callback functions
*/
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	init ();

	// Input .............

	freopen("in.txt","r",stdin);

	point p,p1;
	scanf("%lf %lf",&p.x,&p.y);
	s=p;
	scanf("%lf %lf",&p.x,&p.y);
	d=p;

	scanf("%d",&n);

	for(int i=1;i<=n;i++){
		scanf("%lf %lf",&p.x,&p.y);
		scanf("%lf %lf",&p1.x,&p1.y);
		rects[i]=rect(p,p1);
	}

	//  ...................

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutMainLoop();

	return 0;
}

void set_point(point p){
	glV(p.x,p.y);
}
void draw_line(segment s){	
	glBegin(GL_LINES);
    set_point(s.A);
	set_point(s.B);
    glEnd();
}

void draw_quad(quad q){
	glC(1.0,0.0,0.0);  
	glBegin(GL_QUADS);	
	set_point(q.p[0]);
	set_point(q.p[1]);
	set_point(q.p[2]);
	set_point(q.p[3]);
    glEnd();
}

void draw_circle(circle c){ 
	double cx,cy,r;
	int i,num_segments=100;
	cx=c.center.x; cy=c.center.y;
	r=c.r;

	glBegin(GL_LINE_LOOP); 
	for(i = 0; i < num_segments; i++) 
	{ 
		float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);//get the current angle 

		float x = r * cosf(theta);//calculate the x component 
		float y = r * sinf(theta);//calculate the y component 

		glVertex2f(x + cx, y + cy);//output vertex 

	} 
	glEnd(); 
}

void draw_rect(rect r){

	glRectf(r.p1.x,r.p1.y,r.p2.x,r.p2.y);
}