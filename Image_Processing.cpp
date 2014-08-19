#include "glut.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "bmploader.h"

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

BMPClass menu5;

int print_flag=1;

void convert1(float &x){

	int L=255,a=50,b=150,ya=30,yb=200;
	double alpha=.2,beta=2,gamma=1;

	if(x<a){
		x*=alpha;
		return ;
	}
	else if(x<=b){
		x=(beta*(x-a))+ya;
	}
	else{
		x=gamma*(x-b)+yb;
	}
}

void convert2(float &x){

	int L=255,a=50,b=150,ya=30,yb=200;
	double alpha=.2,beta=2,gamma=1;

	if(x<a){
		0;
		return ;
	}
	else if(x<=b){
		x=(beta*(x-a));
	}
	else{
		x=beta*(b-a);
	}
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
	glTranslatef(-350,-280,0);

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
		  
		  if(print_flag && c<30) printf("%3.0f,%3.0f,%3.0f,  ",rr,gg,bb);

		  glColor3f(rr/255,gg/255.0,bb/255.0);
		  
		  glVertex2d(r,c);		  
		  
		  glEnd();
	  }
	  if(print_flag) puts("");
	}

	if(print_flag) printf("%d %d\n",r,c);
	
	print_flag=0;	

	glPopMatrix();
	
	glPushMatrix();
    glLoadIdentity();
	glTranslatef(-300,0,0);

	for(r=0;r<menu5.width;r++)
	{
	  for(c=0;c<menu5.height;c++)
	  {
		  glBegin(GL_POINTS);
		  
		  float rr=(float)(menu5.pixel(r,c,0));
		  float gg=(float)(menu5.pixel(r,c,1));
		  float bb=(float)(menu5.pixel(r,c,2));		  		  
		  
		  convert2(rr);
		  convert2(gg);
		  convert2(bb);		  

		  glColor3f(rr/255,gg/255.0,bb/255.0);
		  
		  glVertex2d(r,c);		  
		  
		  glEnd();
	  }

	  if(!print_flag)puts("");
	
	}

	glPopMatrix();

}



//  Display Function.........
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glC(1.0, 1.0,.5);

	glBegin( GL_POINTS );
	glColor3f(1,0,0);

	int i,j;

	for(i=0;i<=100;i++){
		for(j=0;j<=100;j++){
			glVertex2d( i,j);
		}
	}

	glEnd();

	mainmenu();


	//glTranslatef(-100,-100,0);
	glScalef(35,35,1);

	//  for  debug  .....................

	draw_circle(circle(s,.03));
	draw_circle(circle(d,.03));

	for(i=1;i<=n;i++){
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
	freopen("out.txt","w",stdout);

    BMPLoad("Lena512.bmp",menu5);

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
