#include "glut.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "bmploader.h"

#define maxm 500
#define pi  acos(-1.0)
#define M_PI pi
#define glC(a,b,c) glColor3f(a,b,c)
#define glV(a,b)   glVertex2f (a,b)
#define ii double
#define eps 1e-9
#define iseq(a,b) (fabs(a-b)<eps)

ii f_real[maxm][maxm],f_img[maxm][maxm];

ii P_real[maxm][maxm],P_img[maxm][maxm];

ii F_real[maxm][maxm],F_img[maxm][maxm];
ii F_real1[maxm][maxm],F_img1[maxm][maxm];
ii F_real2[maxm][maxm],F_img2[maxm][maxm];
ii F_real3[maxm][maxm],F_img3[maxm][maxm];

//Code a texture  mapping . . . .

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

BMPClass pic1,pic2,pic3,pic4;

// Texture mapping code finish . . .

void image_print2();
void image_print3();
void image_print4();

void image_print1(){

	int r,c;	
	for(r=0;r<pic1.width;r++)
	{
	  for(c=0;c<pic1.height;c++)
	  {
		  glBegin(GL_POINTS);

		  float rr=(float)(pic1.pixel(r,c,0));
		  float gg=(float)(pic1.pixel(r,c,1));
		  float bb=(float)(pic1.pixel(r,c,2));

		  glColor3f(rr/255,gg/255.0,bb/255.0);

		  glVertex2d(r,c);

		  glEnd();
	  }
	}

	glTranslatef(0,-250,0);

	for(r=0;r<pic1.width;r++)
	{
	  for(c=0;c<pic1.height;c++)
	  {
		  glBegin(GL_POINTS);

		  double magn=(F_real[r][c]*F_real[r][c]);

		  magn+=(F_img[r][c]*F_img[r][c]);
		  magn=sqrt(magn);

		  float rr=(float)magn;
		  rr/=255.0;

		  glColor3f(rr,rr,rr);
		  glVertex2d(r,c);

		  glEnd();
	  }
	}
	
	glTranslatef(0,250,0);


}

//  Display Function.........
void display(void)
{

	glClear(GL_COLOR_BUFFER_BIT);
	glC(1.0, 1.0,.5);	
	
	glPushMatrix();
    glLoadIdentity();
	glTranslatef(-300,50,0);
	
	image_print1();
	
	glTranslatef(150,0,0);
	image_print2();	
	
	glTranslatef(150,0,0);
	image_print3();	
	
	glTranslatef(150,0,0);
	image_print4();

	glPopMatrix();

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
	glutInitWindowSize (1024, 1024);
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_FLAT);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("Testing");
}

void fourier_transform2();
void fourier_transform3();
void fourier_transform4();


void fourier_transform1(){

	int i,j,n,m,k,l;

	// take input . . .  f(i,j)
	n=pic1.height;
	m=pic1.width;
	for(i=0;i<pic1.height;i++)
	{
	  for(j=0;j<pic1.width;j++)
	  {
		  float rr=(float)(pic1.pixel(i,j,0));
		  f_real[i][j]=rr;
		  f_img[i][j]=0;
		  if((i+j)%2){
			f_real[i][j]=-rr;
			f_img[i][j]=-0;
		  }
	  }	
	}

	// calculating . . .  P(i,j);
	for(j=0;j<m;j++){
		int k;
		for (k = 0; k < n; k++) {  /* For each output element */
			double sumreal = 0;
			double sumimag = 0;
			int t;
			for (t = 0; t < n; t++) {  /* For each input element */
				double angle = 2 * M_PI * t * k / (double)n;
				sumreal +=  f_real[t][j] * cos(angle) + f_img[t][j] * sin(angle);
				sumimag += -f_real[t][j] * sin(angle) + f_img[t][j] * cos(angle);
			}
			P_real[k][j] = sumreal;
			P_img[k][j] = sumimag;
		}
	}

	// taking Input . . .  F(i,j);
	for(i=0;i<n;i++){
		int k;
		for (k = 0; k < m; k++) {  /* For each output element */
			double sumreal = 0;
			double sumimag = 0;
			int t;
			for (t = 0; t < m; t++) {  /* For each input element */
				double angle = 2 * M_PI * t * k / (double)n;
				sumreal +=  P_real[i][t] * cos(angle) + P_img[i][t] * sin(angle);
				sumimag += -P_real[i][t] * sin(angle) + P_img[i][t] * cos(angle);
			}
			F_real[i][k] = sumreal/(double)m;
			F_img[i][k] = sumimag/(double)m;
		}
	}
	puts("FINISH");	

	return ;
}


int main(int argc, char** argv)
{
	//freopen("out.txt","w",stdout);
	freopen("in.txt","r",stdin);

    BMPLoad("sin1.bmp",pic1);
    BMPLoad("sin3.bmp",pic2);
    BMPLoad("sin5.bmp",pic3);
    BMPLoad("sin7.bmp",pic4);

	glutInit(&argc, argv);
	init ();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	
	// Fourier Transform . . . . . .
	fourier_transform1();
	fourier_transform2();
	fourier_transform3();
	fourier_transform4();

	glutMainLoop();

	return 0;
}


void fourier_transform2(){

	int i,j,n,m,k,l;

	// take input . . .  f(i,j)
	n=pic2.height;
	m=pic2.width;
	for(i=0;i<pic2.height;i++)
	{
	  for(j=0;j<pic2.width;j++)
	  {
		  float rr=(float)(pic2.pixel(i,j,0));
		  f_real[i][j]=rr;
		  f_img[i][j]=0;
		  if((i+j)%2){
			f_real[i][j]=-rr;
			f_img[i][j]=-0;
		  }
	  }	
	}

	// calculating . . .  P(i,j);
	for(j=0;j<m;j++){
		int k;
		for (k = 0; k < n; k++) {  /* For each output element */
			double sumreal = 0;
			double sumimag = 0;
			int t;
			for (t = 0; t < n; t++) {  /* For each input element */
				double angle = 2 * M_PI * t * k / (double)n;
				sumreal +=  f_real[t][j] * cos(angle) + f_img[t][j] * sin(angle);
				sumimag += -f_real[t][j] * sin(angle) + f_img[t][j] * cos(angle);
			}
			P_real[k][j] = sumreal;
			P_img[k][j] = sumimag;
		}
	}

	// taking Input . . .  F(i,j);
	for(i=0;i<n;i++){
		int k;
		for (k = 0; k < m; k++) {  /* For each output element */
			double sumreal = 0;
			double sumimag = 0;
			int t;
			for (t = 0; t < m; t++) {  /* For each input element */
				double angle = 2 * M_PI * t * k / (double)n;
				sumreal +=  P_real[i][t] * cos(angle) + P_img[i][t] * sin(angle);
				sumimag += -P_real[i][t] * sin(angle) + P_img[i][t] * cos(angle);
			}
			F_real1[i][k] = sumreal/(double)m;
			F_img1[i][k] = sumimag/(double)m;
		}
	}

	puts("FINISH");	

	return ;
}


void fourier_transform3(){

	int i,j,n,m,k,l;

	// take input . . .  f(i,j)
	n=pic3.height;
	m=pic3.width;
	for(i=0;i<pic3.height;i++)
	{
	  for(j=0;j<pic3.width;j++)
	  {
		  float rr=(float)(pic3.pixel(i,j,0));
		  f_real[i][j]=rr;
		  f_img[i][j]=0;
		  if((i+j)%2){
			f_real[i][j]=-rr;
			f_img[i][j]=-0;
		  }
	  }	
	}

	// calculating . . .  P(i,j);
	for(j=0;j<m;j++){
		int k;
		for (k = 0; k < n; k++) {  /* For each output element */
			double sumreal = 0;
			double sumimag = 0;
			int t;
			for (t = 0; t < n; t++) {  /* For each input element */
				double angle = 2 * M_PI * t * k / (double)n;
				sumreal +=  f_real[t][j] * cos(angle) + f_img[t][j] * sin(angle);
				sumimag += -f_real[t][j] * sin(angle) + f_img[t][j] * cos(angle);
			}
			P_real[k][j] = sumreal;
			P_img[k][j] = sumimag;
		}
	}

	// taking Input . . .  F(i,j);
	for(i=0;i<n;i++){
		int k;
		for (k = 0; k < m; k++) {  /* For each output element */
			double sumreal = 0;
			double sumimag = 0;
			int t;
			for (t = 0; t < m; t++) {  /* For each input element */
				double angle = 2 * M_PI * t * k / (double)n;
				sumreal +=  P_real[i][t] * cos(angle) + P_img[i][t] * sin(angle);
				sumimag += -P_real[i][t] * sin(angle) + P_img[i][t] * cos(angle);
			}
			F_real2[i][k] = sumreal/(double)m;
			F_img2[i][k] = sumimag/(double)m;
		}
	}
	puts("FINISH");	

	return ;
}


void fourier_transform4(){

	int i,j,n,m,k,l;

	// take input . . .  f(i,j)
	n=pic4.height;
	m=pic4.width;
	for(i=0;i<pic4.height;i++)
	{
	  for(j=0;j<pic4.width;j++)
	  {
		  float rr=(float)(pic4.pixel(i,j,0));
		  f_real[i][j]=rr;
		  f_img[i][j]=0;
		  if((i+j)%2){
			f_real[i][j]=-rr;
			f_img[i][j]=-0;
		  }
	  }	
	}

	// calculating . . .  P(i,j);
	for(j=0;j<m;j++){
		int k;
		for (k = 0; k < n; k++) {  /* For each output element */
			double sumreal = 0;
			double sumimag = 0;
			int t;
			for (t = 0; t < n; t++) {  /* For each input element */
				double angle = 2 * M_PI * t * k / (double)n;
				sumreal +=  f_real[t][j] * cos(angle) + f_img[t][j] * sin(angle);
				sumimag += -f_real[t][j] * sin(angle) + f_img[t][j] * cos(angle);
			}
			P_real[k][j] = sumreal;
			P_img[k][j] = sumimag;
		}
	}

	// taking Input . . .  F(i,j);
	for(i=0;i<n;i++){
		int k;
		for (k = 0; k < m; k++) {  /* For each output element */
			double sumreal = 0;
			double sumimag = 0;
			int t;
			for (t = 0; t < m; t++) {  /* For each input element */
				double angle = 2 * M_PI * t * k / (double)n;
				sumreal +=  P_real[i][t] * cos(angle) + P_img[i][t] * sin(angle);
				sumimag += -P_real[i][t] * sin(angle) + P_img[i][t] * cos(angle);
			}
			F_real3[i][k] = sumreal/(double)m;
			F_img3[i][k] = sumimag/(double)m;
		}
	}

	puts("FINISH");
	

	return ;
}
void image_print2(){

	int r,c;	
	for(r=0;r<pic2.width;r++)
	{
	  for(c=0;c<pic2.height;c++)
	  {
		  glBegin(GL_POINTS);

		  float rr=(float)(pic2.pixel(r,c,0));
		  float gg=(float)(pic2.pixel(r,c,1));
		  float bb=(float)(pic2.pixel(r,c,2));

		  glColor3f(rr/255,gg/255.0,bb/255.0);

		  glVertex2d(r,c);

		  glEnd();
	  }
	}	

	glTranslatef(0,-250,0);

	for(r=0;r<pic2.width;r++)
	{
	  for(c=0;c<pic2.height;c++)
	  {
		  glBegin(GL_POINTS);

		  double magn=(F_real1[r][c]*F_real1[r][c]);

		  magn+=(F_img1[r][c]*F_img1[r][c]);
		  magn=sqrt(magn);

		  float rr=(float)magn;
		  rr/=255.0;

		  glColor3f(rr,rr,rr);
		  glVertex2d(r,c);

		  glEnd();
	  }
	}
	glTranslatef(0,250,0);

}

void image_print3(){
	
	int r,c;	
	for(r=0;r<pic3.width;r++)
	{
	  for(c=0;c<pic3.height;c++)
	  {
		  glBegin(GL_POINTS);

		  float rr=(float)(pic3.pixel(r,c,0));
		  float gg=(float)(pic3.pixel(r,c,1));
		  float bb=(float)(pic3.pixel(r,c,2));

		  glColor3f(rr/255,gg/255.0,bb/255.0);

		  glVertex2d(r,c);

		  glEnd();
	  }
	}	

	glTranslatef(0,-250,0);

	for(r=0;r<pic3.width;r++)
	{
	  for(c=0;c<pic3.height;c++)
	  {
		  glBegin(GL_POINTS);

		  double magn=(F_real2[r][c]*F_real2[r][c]);

		  magn+=(F_img2[r][c]*F_img2[r][c]);
		  magn=sqrt(magn);

		  float rr=(float)magn;
		  rr/=255.0;

		  glColor3f(rr,rr,rr);
		  glVertex2d(r,c);

		  glEnd();
	  }
	}
	
	glTranslatef(0,250,0);


}

void image_print4(){

	int r,c;	
	for(r=0;r<pic4.width;r++)
	{
	  for(c=0;c<pic4.height;c++)
	  {
		  glBegin(GL_POINTS);

		  float rr=(float)(pic4.pixel(r,c,0));
		  float gg=(float)(pic4.pixel(r,c,1));
		  float bb=(float)(pic4.pixel(r,c,2));

		  glColor3f(rr/255,gg/255.0,bb/255.0);

		  glVertex2d(r,c);

		  glEnd();
	  }
	}

	glTranslatef(0,-250,0);

	for(r=0;r<pic4.width;r++)
	{
	  for(c=0;c<pic4.height;c++)
	  {
		  glBegin(GL_POINTS);

		  double magn=(F_real3[r][c]*F_real3[r][c]);

		  magn+=(F_img3[r][c]*F_img3[r][c]);
		  magn=sqrt(magn);

		  float rr=(float)magn;
		  rr/=255.0;

		  glColor3f(rr,rr,rr);
		  glVertex2d(r,c);

		  glEnd();
	  }
	}
	glTranslatef(0,250,0);

}
