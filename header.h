void glText(GLdouble x, GLdouble y, char *str, void* font=GLUT_BITMAP_8_BY_13)
{
	glRasterPos3d(x, y, 0);
	int i;
	for (i=0; str[i]; i++)
	{
		glutBitmapCharacter(font, str[i]); //,GLUT_BITMAP_8_BY_13, GLUT_BITMAP_TIMES_ROMAN_24
	}
}

void glColoredLine( double r, double g, double b, int x1, int y1, int x2, int y2 )
{
	glColor3f(r,g,b);
	glBegin(GL_LINES);
	glVertex2f(x1,y1);
	glVertex2f(x2,y2);
	glEnd();
}
void glFilledCircle(double x, double y, double r, int slices=100)
{
	
	
	double t, dt, x1,y1, xp, yp;
	dt = 2*3.1416/slices;
	xp = x+r;
	yp = y;
	glBegin(GL_POLYGON);
	for(t = 0; t <= 2*3.1416; t+=dt)
	{
		x1 = x + r * cos(t);
		y1 = y + r * sin(t);

		glVertex2f(xp, yp);
		xp = x1;
		yp = y1;
	}
	glEnd();
}
