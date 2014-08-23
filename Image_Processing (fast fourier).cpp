#include "glut.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "bmploader.h"

#define maxm 1050
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
ii ff_real[maxm][maxm],ff_img[maxm][maxm];

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

BMPClass pic;

// Texture mapping code finish . . .

int print_flag=0;

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
		x=0;
		return ;
	}
	else if(x<=b){
		x=(beta*(x-a));
	}
	else{
		x=beta*(b-a);
	}
}

void image_print(){

	//puts("MainMenu");
	
	glColor3f(0,0,0);

	glPushMatrix();
    glLoadIdentity();
	glTranslatef(-512,0,0);

	//texture maping of left field
	int r,c,r1;
	double p1,c1;

	for(r=0;r<pic.width;r++)
	{
	  for(c=0;c<pic.height;c++)
	  {
		  glBegin(GL_POINTS);

		  float rr=(float)(pic.pixel(r,c,0));
		  float gg=(float)(pic.pixel(r,c,1));
		  float bb=(float)(pic.pixel(r,c,2));

		  if(print_flag && c<30 && r<30) printf("%3.0f,%3.0f,%3.0f,  ",rr,gg,bb);

		  glColor3f(rr/255,gg/255.0,bb/255.0);

		  glVertex2d(r,c);

		  glEnd();
	  }
	  if(print_flag && r<=30) puts("");
	}

	if(print_flag){
		printf("width= %d  height= %d\n",r,c);
	}

	

	glPopMatrix();


	glPushMatrix();
    glLoadIdentity();
	glTranslatef(0,0,0);

	for(r=0;r<pic.width;r++)
	{
	  for(c=0;c<pic.height;c++)
	  {
		  glBegin(GL_POINTS);

		  double magn=(F_real[r][c]*F_real[r][c]);

		  magn+=(F_img[r][c]*F_img[r][c]);
		  magn=sqrt(magn);

		  float rr=(float)magn;

		  //convert2(rr);
		  
		  rr/=255.0;

		  if(print_flag && c<30 && r<30){
			  printf("%.4lf ",rr);
		  }

		  
		  
		    
		  //if(rr>1.0){
		  //  glEnd();
		//	  continue;
		  //}

		  glColor3f(rr,rr,rr);

		  glVertex2d(r,c);

		  glEnd();
	  }
	  if(print_flag && r<=30){
		  puts("");
	  }

	}

	glPopMatrix();

	print_flag=0;


	glPushMatrix();
    glLoadIdentity();
	glTranslatef(-512,-512,0);

	for(r=0;r<pic.width;r++)
	{
	  for(c=0;c<pic.height;c++)
	  {
		  glBegin(GL_POINTS);

		  double magn=(ff_real[r][c]*ff_real[r][c]);
		  magn+=(ff_img[r][c]*ff_img[r][c]);
		  magn=sqrt(magn);

		  float rr=(float)magn;
		  
		  rr/=255.0;
		  
		  
		  //if(rr>1.0){
		  //  glEnd();
		//	  continue;
		  //}

		  glColor3f(rr,rr,rr);

		  glVertex2d(r,c);

		  glEnd();
	  }

	}

	glPopMatrix();

}

//  Display Function.........
void display(void)
{

	glClear(GL_COLOR_BUFFER_BIT);
	glC(1.0, 1.0,.5);

	//glBegin( GL_POINTS );
	//glColor3f(1,0,0);

	int i,j;

	for(i=0;i<=100;i++){
		for(j=0;j<=100;j++){
			//glVertex2d( i,j);
		}
	}

	//glEnd();

	image_print();

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
	glutInitWindowSize (1024, 1024);
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_FLAT);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("Testing");

}


/*
 * Computes the discrete Fourier transform (DFT) of the given vector.
 * All the array arguments must have the same length.
 */
void compute_dft(double inreal[], double inimag[], double outreal[], double outimag[], int n) {
	int k;
	for (k = 0; k < n; k++) {  /* For each output element */
		double sumreal = 0;
		double sumimag = 0;
		int t;
		for (t = 0; t < n; t++) {  /* For each input element */
			double angle = 2 * M_PI * t * k / n;
			sumreal +=  inreal[t] * cos(angle) + inimag[t] * sin(angle);
			sumimag += -inreal[t] * sin(angle) + inimag[t] * cos(angle);
		}
		outreal[k] = sumreal;
		outimag[k] = sumimag;
	}
}

void fourier_transform();

int main(int argc, char** argv)
{
	//freopen("out.txt","w",stdout);
	freopen("in.txt","r",stdin);

    BMPLoad("GRN.BMP",pic);

	glutInit(&argc, argv);
	init ();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	fourier_transform();

	// take input . . .

	glutMainLoop();

	return 0;
}

void fourier_transform(){

	int i,j,n,m,k,l;

	//return ;

	// take input . . .  f(i,j)

	n=pic.height;
	m=pic.width;

	for(i=0;i<pic.height;i++)
	{
	  for(j=0;j<pic.width;j++)
	  {
		  float rr=(float)(pic.pixel(j,i,0));
		  f_real[i][j]=rr;
		  f_img[i][j]=0;
		  if((i+j)%2){
			f_real[i][j]=-rr;
			f_img[i][j]=-0;
		  }
	  }
	
	}

	/// DFT  >>>>>>>>>>>>

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
		printf("j= %d\n",j);
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
		printf("i= %d\n",i);
	}

	// IDFT >>>>>>>>>>>>>>>>>

	// calculating . . .  P(i,j);

	for(j=0;j<m;j++){
		int k;
		for (k = 0; k < n; k++) {  /* For each output element */
			double sumreal = 0;
			double sumimag = 0;
			int t;
			for (t = 0; t < n; t++) {  /* For each input element */
				double angle = 2 * M_PI * t * k / (double)n;
				sumreal +=  F_real[t][j] * cos(angle) - F_img[t][j] * sin(angle);
				sumimag += F_real[t][j] * sin(angle) + F_img[t][j] * cos(angle);
			}
			P_real[k][j] = sumreal/(double)(n);
			P_img[k][j] = sumimag/(double)(n);		
		}
		printf("j= %d\n",j);
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
				sumreal +=  P_real[i][t] * cos(angle) - P_img[i][t] * sin(angle);
				sumimag += P_real[i][t] * sin(angle) + P_img[i][t] * cos(angle);
			}
			ff_real[i][k] = sumreal;
			ff_img[i][k] = sumimag;
		}
		printf("i= %d\n",i);
	}
	

	return ;

	// Printing >>>>>
	for(i=0;i<n;i++){
		for(j=0;j<m;j++){
			printf("%3.1lf, I%3.1lf  ",f_real[i][j],f_img[i][j]);
		}
		puts("");
	}
	puts("");
	puts("");

	for(i=0;i<n;i++){
		for(j=0;j<m;j++){
			printf("%3.1lf, I%3.1lf  ",F_real[i][j],F_img[i][j]);
		}
		puts("");
	}
	puts("");
	puts("");

	for(i=0;i<n;i++){
		for(j=0;j<m;j++){
			printf("%3.1lf, I%3.1lf  ",ff_real[i][j],ff_img[i][j]);
		}
		puts("");
	}
	puts("");
	puts("");

}
