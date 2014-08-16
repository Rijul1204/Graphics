.// BOX Movement using up-down-left-right key of the keyboard.
#include "glut.h"
#include <stdio.h>
#include <math.h>

// Initial size of graphics window.
int WIDTH  = 800;
int HEIGHT = 600;

// Current size of window.
int width  = WIDTH;
int height = HEIGHT;

#define RIGHT_ARROW 4
#define LEFT_ARROW 3
#define UP_ARROW 1
#define DOWN_ARROW 2

bool keyStates[256];
bool arrowKeys[5];


/** 
  * TIMERSEC int 
  * the amount of time (in milisecond) after which animate function is called
  */
const int TIMERMSECS = 1;

const int BOX_WIDTH = 100;
const int BOX_HEIGHT = 30;

int boxleft = 350, boxtop=200;


void display ()
{
	// Function that clears the window. If you don't want to clear the view window, don't call glClear function. make it as a comment statement. 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// --------------
	// YOUR CODE ...
	// --------------
	
	glColor3f(.5, .5, .5);
	glBegin(GL_QUADS);
		glVertex2f( boxleft, boxtop );
		glVertex2f( boxleft+BOX_WIDTH, boxtop );
		glVertex2f( boxleft+BOX_WIDTH, boxtop +BOX_HEIGHT);
		glVertex2f( boxleft, boxtop +BOX_HEIGHT);
	glEnd();

	glutSwapBuffers();
}


void mouseMovement (int mx, int my)
{
	// --------------
	// YOUR CODE ...
	// --------------

	glutPostRedisplay();
}

// Respond to window resizing, preserving proportions.
// Parameters give new window size in pixels.
void reshapeMainWindow (int newWidth, int newHeight)
{
	// ----------------------------
	// DO NOT ALTER THIS FUNCTION ...
	// ----------------------------

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 0, newWidth, 0, newHeight, 0, 1 );
	glViewport( 0, 0, newWidth, newHeight );
	width = newWidth;
	height = newHeight;
    glMatrixMode(GL_MODELVIEW); 
	glutPostRedisplay();
}


void keyDown(unsigned char key, int x, int y)
{
     keyStates[key] = true;
    // glutPostRedisplay();
}

void keyUp(unsigned char key, int x, int y)
{
     keyStates[key] = false;
     
    // glutPostRedisplay();
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
		 printf("abar paisi\n");
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
		    printf("paisi\n\n");
            boxtop +=1; 
	 }
     if (keyStates['j']||arrowKeys[LEFT_ARROW]) {
                                                    boxleft -= 1;
                                              
     }
     if (keyStates['k']||arrowKeys[DOWN_ARROW]){
                                                     boxtop -=1;
                                              
	 }   
     if (keyStates['l']||arrowKeys[RIGHT_ARROW]) {
     
		 boxleft +=1;
     
	 }
	 glutPostRedisplay();

}

// Respond to graphic character keys.
// Parameters give key code and mouse coordinates.
/*void graphicKeys (unsigned char key, int x, int y)
{
	switch (key) {

		case 'h':
			// --------------
			// YOUR CODE ...
			// --------------
			break;
		case 27: // Escape
			exit(0);
		default:
			printf("%c\n",key);
	}
}

// Respond to function keys.
// Parameters give key code and mouse coordinates.
void functionKeys (int key, int x, int y)
{
	switch (key)
	{

		case GLUT_KEY_UP:
			boxtop +=5;
			break;
		case GLUT_KEY_DOWN:
			boxtop -=5;
			break;
		case GLUT_KEY_LEFT:
			boxleft -= 5;
			break;
		case GLUT_KEY_RIGHT:
			boxleft += 5;
			break;
	}
	glutPostRedisplay();
}

*/
void timer(int value) {

	// --------------
	// YOUR CODE ...
	// --------------

    glutTimerFunc(TIMERMSECS, timer, 0);
	glutPostRedisplay();
}


int main (int argc, char **argv)
{
	// GLUT initialization.
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutCreateWindow("GLUT Skeleton Program");

	// Register call backs.
	glutDisplayFunc(display);
	glutReshapeFunc(reshapeMainWindow);
	//glutKeyboardFunc(graphicKeys);
    // glutSpecialFunc(functionKeys);
  //  glutPassiveMotionFunc(mouseMovement);
   // glutTimerFunc(TIMERMSECS, timer, 0);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp); 
    glutSpecialFunc(specialDown);
    glutSpecialUpFunc(specialUp);
    glutIdleFunc(keyCheck);
    
	// Enter GLUT loop.
	glutMainLoop();
	return 0;
}