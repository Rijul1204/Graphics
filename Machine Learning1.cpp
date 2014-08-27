#include "glut.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>
using namespace std;

#define pi  acos(-1.0)
#define glC(a,b,c) glColor3f(a,b,c)
#define glV(a,b)   glVertex2f (a,b)
#define ii double
#define inf (1<<28)

#define W 800
#define H 600

#define S_W 500
#define S_H 400

#define MX_W (W/2+(W-S_W))
#define MX_H (H/2+(H-S_H))

#define eps 1e-2

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


ii mini(ii a,ii b){
	if(a<b) return a;
	return b;
}
ii maxi(ii a,ii b){
	if(a>b) return a;
	return b;
}

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

#define maxm 1000100

int n,m;
point s,d;

double scale_x=13,scale_y=2000;
ii theta[10],tmp[10];

struct node{
	double x[10];
	double y;
	node(){}
};
node nodes[maxm];

void print_curve(vector<ii>x,vector<ii>y){

	double mx_x=0,mn_x=inf;
	double mx_y=0,mn_y=inf;

	int i;

	glBegin( GL_POINTS );
	
	for(i=0;i<x.size();i++){
		mx_x=maxi(mx_x,x[i]);
		mn_x=mini(mn_x,x[i]);

		mx_y=maxi(mx_y,y[i]);
		mn_y=mini(mn_y,y[i]);
	}

	for(i=0;i<x.size();i++){

		ii v_x=((x[i]-mn_x)/(mx_x-mn_x+1))*MX_W;
		ii v_y=((y[i]-mn_y)/(mx_y-mn_y+1))*MX_H;
		glVertex2f(v_x,v_y);
	}

	glEnd();

}

void show_original(){
	
	glTranslatef(S_W-W,S_H-H,0);

	draw_line(segment(point(0,4000),point(0,-4000)));
	draw_line(segment(point(4000,0),point(-1000,0)));
	
	//draw_line(segment(point(-4000,MX_H),point(1000,MX_H)));
	//draw_line(segment(point(MX_W,-1000),point(MX_W,1000)));

	glBegin( GL_POINTS );
	glColor3f(1,0,0);
	
	int i,j;
	vector<ii>x,y;

	for(i=1;i<=n;i++){
		x.push_back(nodes[i].x[1]);
		y.push_back(nodes[i].y);
		//glVertex2f(i,nodes[i].y);
	}

	print_curve(x,y);
	
	glEnd();
}

double h_theta(ii theta[],ii x[]){

	double val=0;

	for(int i=0;i<=m;i++){
		val+=(theta[i]*x[i]);
	}
	
	return val;
}

void show_predicted(){	


	glBegin( GL_POINTS );
	glColor3f(0,1,0);
	
	int i,j;
	vector<ii>x,y;

	for(i=1;i<=n;i++){		
		x.push_back(nodes[i].x[1]);
		y.push_back(h_theta(theta,nodes[i].x));
		//glVertex2f(i,nodes[i].y);
	}

	print_curve(x,y);
	
	glEnd();
}



void show_cost(){
	
	//glRotatef(45,1,1,0);
	
	draw_line(segment(point(0,4000),point(0,-4000)));
	draw_line(segment(point(4000,0),point(-1000,0)));

	glBegin( GL_POINTS );
	glColor3f(0,0,1);

	double theta[10]={0};

	vector<ii>x,y;
	
	int i,j;
	for(theta[1]=20000,j=1;theta[1]<=30000;theta[1]+=.5,j++){
		
		double sum=0,y1,y2;
		for(i=1;i<=n;i++){
			y1=h_theta(theta,nodes[i].x);
			sum+=((y1-nodes[i].y)*(y1-nodes[i].y));
		}
		sum/=(2*n);
		//sum=sqrt(sum);
		//sum/=scale_y;
		
		x.push_back(j);
		y.push_back(sum);

		//glVertex2f(j,sum);		
		//draw_line(segment(point(-1000,y1),point(1000,y2)));
	}

	print_curve(x,y);
	
	glEnd();
}


void gradient_descent(){

	double alpha=.12;
	int i,j;

	vector<ii>x,y;

	for(i=0;i<=m;i++){
		theta[i]=tmp[i]=1;
	}

	glColor3f(0,1,1);
	
	int iter=0;
	while(1){

		ii y1=theta[0] + theta[1]*(double)-4000;
		ii y2=theta[0] + theta[1]*(double)4000;				
	
		ii cost=0;

		for(j=0;j<=m;j++){
			printf("%.3lf ",theta[j]);
		}
		puts("");

		for(j=0;j<=m;j++){
			ii val=0;
			for(i=1;i<=n;i++){
				ii val1=h_theta(theta,nodes[i].x);
				val1-= nodes[i].y;
				val+=( val1 * nodes[i].x[j]);				
			}
			val/=(double)n;
			val*=alpha;
			tmp[j]=(theta[j]-val);
		}
		
		ii val=0;
		for(i=1;i<=n;i++){
			ii val1=h_theta(theta,nodes[i].x)- nodes[i].y;
			val1*=val1;
			val+=val1;
		}		
		val/=(double)n;
		val*=.5;

		x.push_back(iter);
		y.push_back(val);

		printf("Cost : %.4lf\n",val);
		
		ii mx_diff=0;
		for(i=0;i<=m;i++){
			mx_diff=maxi(mx_diff,fabs(tmp[i]-theta[i]));
			theta[i]=tmp[i];
		}

	

		if(mx_diff<=eps) break;

		iter++;
		printf("%d\n",iter);
	}

	print_curve(x,y);

	
	
}



//  Display Function.........
void display(void)
{
	
	glClear(GL_COLOR_BUFFER_BIT);
	glC(1.0, 1.0,.5);	
	
	// Original - data 
	show_original();

	// Cost - Function 
	show_cost();

	// Gradient - descent 
	gradient_descent();

	//predicted - data 
	show_predicted();
	
		
	glFlush();
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w/2, w/2, -h/2, h/2, -w/2, w/2);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void init(void)
{
	glutInitWindowSize (W, H);
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
	int i,j;

	glutInit(&argc, argv);
	init ();

	// Input .............

	freopen("in.txt","r",stdin);

	scanf("%d %d",&n,&m);
	
	ii mx[10]={0};		
	for(i=1;i<=n;i++){
		nodes[i].x[0]=1;
		for(j=1;j<=m;j++){
			scanf("%lf",&nodes[i].x[j]);
			mx[j]=maxi(mx[j],nodes[i].x[j]);
		}
		int tmp;
		scanf("%d",&tmp);
		nodes[i].y=tmp;
		//nodes[i].y/=scale_y;
	}

	for(i=1;i<=n;i++){
		for(j=1;j<=m;j++){
			nodes[i].x[j]/=mx[j];
		}
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