/*
 *  Lorenz Attractor
 *  Author - Pallavi Madasu
 *  Display 3D visualization of Lorenz attractor
 *
 *  Key bindings:
 *
 *  ESC     Exit
 *  0       Reset view angle
 *  +   	Zoom In
 *  -       Zoom Out
 *  s       Increase parameter 's' by 0.2
 *  S		Decrease Parameter 's' by 0.2
 *  b       Increase parameter 'b' by 0.1
 *  B		Decrease Parameter 'b' by 0.1
 *  r       Increase parameter 'r' by 0.5
 *  R		Decrease Parameter 'r' by 0.5
 *  o       Start/Stop Animation
 *  x  	 	Scene viewed through X axis
 *  y    	Scene viewed through Y axis
 *  z   	Scene viewed through Z axis
 *  top/bottom arrows   Change elevation angle
 *	left/right arrows	Change Azimuth angle
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

//  Globals
int th=0;       // Azimuth of view angle
int ph=0;       // Elevation of view angle
double w=1;     // W variable
double dim=2;   // Dimension of orthogonal box

/*  Lorenz Parameters  */
double s  = 10;
double b  = 2.6666;
double r  = 28;
// Flag to toggle animation of the scene
int animateFlag = 0;

/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  // Maximum length of text string


/*
 *  Function to print any errors encountered
 */
void ErrCheck(char* where)
{
   int err = glGetError();
   if (err) fprintf(stderr,"ERROR: %s [%s]\n",gluErrorString(err),where);
}


void Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
   ErrCheck("Print");
}

/*
 *  Display the scene
 */
void display()
{
   //  Clear the image
   glClear(GL_COLOR_BUFFER_BIT);
   //  Reset previous transforms
   glLoadIdentity();
   //  Set view angle
   glRotated(ph,1,0,0);
   glRotated(th,0,1,0);
   //  Draw 1 pixel points - Randomly generated colors
   glColor3ub( rand()%255, rand()%255, rand()%255 );
   glPointSize(1);
   glBegin(GL_LINE_STRIP);
   
   int i;
   /*  Coordinates  */
   double x = 1;
   double y = 1;
   double z = 1;
   /*  Time step  */
   double dt = 0.001;
   
   for (i=0;i<50000;i++)
   {
      double dx = s*(y-x);
      double dy = x*(r-z)-y;
      double dz = x*y - b*z;
      x += dt*dx;
      y += dt*dy;
      z += dt*dz;
      glVertex4f(x*0.03,y*0.03,z*0.03,w);
   }
   glEnd();
   //  Draw axes in white
   glColor3f(1,1,1);
   glBegin(GL_LINES);
   glVertex3d(0,0,0);
   glVertex3d(1,0,0);
   glVertex3d(0,0,0);
   glVertex3d(0,1,0);
   glVertex3d(0,0,0);
   glVertex3d(0,0,1);
   glEnd();
   //  Label axes
   glRasterPos3d(1,0,0);
   Print("X");
   glRasterPos3d(0,1,0);
   Print("Y");
   glRasterPos3d(0,0,1);
   Print("Z");
   //  Display parameters
   glWindowPos2i(5,5);
   Print("View Angle=%d,%d; s = %f; b = %f; r = %f",th,ph,s,b,r);
   //  Sanity check
   ErrCheck("display");
   //  Flush and swap
   glFlush();
   glutSwapBuffers();
}

/*
 * This function is called by GLUT when idle
 */
void idle()
{
	// When animate flag is enabled, rotate the scene and update the display
	if( animateFlag == 1)
	{	
		double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   		//  Calculate spin angle 90 degrees/second
   		th = fmod(90*t,360);
   		ph = fmod(90*t,360);
   		//  Request display update
   		glutPostRedisplay();
   	}
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
	switch(ch)
	{
		//  Exit on ESC
		case 27  :	exit(0);
					break;
		
		//	Reset view angle
		case '0' :  th = ph = 0;
					animateFlag = 0;
					break;
					
		//	Zoom Out - Increase w by 0.01
		case '-' :  w += 0.01;
					animateFlag = 0;
					break;
		
		//	Zoom In - Decrease w by 0.01
		case '+' : 	w -= 0.01;
					animateFlag = 0;
					break;
					
		//	Lorenz Parameter s - Increase by 0.2
		case 's' :  s += 0.2;
					animateFlag = 0;
					break;
		
		//	Lorenz Parameter s - Decrease by 0.2
		case 'S' : 	s -= 0.2;
					animateFlag = 0;
					break;
					
		//	Lorenz Parameter b - Increase by 0.1
		case 'b' :  b += 0.1;
					animateFlag = 0;
					break;
		
		//	Lorenz Parameter b - Decrease by 0.1
		case 'B' : 	b -= 0.1;
					animateFlag = 0;
					break;
					
		//	Lorenz Parameter r - Increase by 0.5
		case 'r' :  r += 0.5;
					animateFlag = 0;
					break;
		
		//	Lorenz Parameter s - Increase by 0.5
		case 'R' : 	r -= 0.5;
					animateFlag = 0;
					break;
					
		// Enable and Disable Animation - Rotate the scene
		case 'o':   animateFlag = (animateFlag == 0) ? 1 : 0;
					break;
					
		//	Scene viewed through X axis
		case 'x' : 	th = 90;
					ph = 0;
					animateFlag = 0;
					break;
		
		//	Scene viewed through Y axis
		case 'y' :  th = 0;
					ph = -90;
					animateFlag = 0;
					break;
					
		// Scene viewed through Z axis
		case 'z' :  th = 0;
					ph = 0;
					animateFlag = 0;
					break;
 	}
   ErrCheck("key");
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase azimuth by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease azimuth by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   ErrCheck("special");
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   double w2h = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Orthogonal projection box adjusted for the aspect ratio of the window
   glOrtho(-dim*w2h,+dim*w2h, -dim,+dim, -dim,+dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
   ErrCheck("reshape");
}



/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
  //  Initialize GLUT and process user parameters
   glutInit(&argc,argv);
   //  Request double buffered, true color window 
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
   //  Request 500 x 500 pixel window
   glutInitWindowSize(500,500);
   //  Create the window
   glutCreateWindow("HW 1 - Pallavi Madasu");
   //  Tell GLUT to call "display" when the scene should be drawn
   glutDisplayFunc(display);
  //  Tell GLUT to call "reshape" when the window is resized
   glutReshapeFunc(reshape);
   //  Tell GLUT to call "special" when an arrow key is pressed
   glutSpecialFunc(special);
   //  Tell GLUT to call "key" when a key is pressed
   glutKeyboardFunc(key);
   //  Pass control to GLUT so it can interact with the user
   glutIdleFunc(idle);
   glutMainLoop();
   //  Return code
   return 0;
}
