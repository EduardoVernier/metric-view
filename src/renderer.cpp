#include "../include/renderer.h"

extern int main_window;

// Singletons
Mouse *mouse = NULL;
Canvas *p_canvas = NULL;
Canvas *t_canvas = NULL;

// Glui functions
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_TRIANGLES);
      glColor3f(0.0, 0.0, 1.0);  /* blue */
      glVertex2i(0, 0);
      glColor3f(0.0, 1.0, 0.0);  /* green */
      glVertex2i(200, 200);
      glColor3f(1.0, 0.0, 0.0);  /* red */
      glVertex2i(20, 200);
    glEnd();
    glFlush();  /* Single buffered, so needs a flush. */
}

void reshape(int w, int h)
{
	  glViewport(0, 0, w, h);       /* Establish viewing area to cover entire window. */
	  glMatrixMode(GL_PROJECTION);  /* Start modifying the projection matrix. */
	  glLoadIdentity();             /* Reset project matrix. */
	  glOrtho(0, w, 0, h, -1, 1);   /* Map abstract coords directly to window coords. */
	  glScalef(1, -1, 1);           /* Invert Y axis so increasing Y goes down. */
	  glTranslatef(0, -h, 0);       /* Shift origin up to upper-left corner. */
}

void idle()
{
	if ( glutGetWindow() != main_window )
		glutSetWindow(main_window);
	glutPostRedisplay();
}

// Drawing
void render()
{

}



// Interaction
void mouse_click(int button, int state, int x, int y)
{
    mouse->click(button, state, x, y);
	//printf("%d %d\n", x,y );
}

void setCanvassesSizes(int w, int h)
{
	Point p_tl, p_br;
	p_tl.x = 10; p_tl.y = 10;
	p_br.x = 100; p_br.y = 100;

	if (p_canvas == NULL && t_canvas == NULL)
	{
		p_canvas = new Canvas(p_tl, p_br);
	}
}
