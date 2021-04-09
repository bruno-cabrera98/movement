#include <SDL.h>
#include <SDL_opengl.h>
#include <FreeImage.h>
#include <iostream>
#include <GL/glu.h>
#include <time.h>
#include <math.h>

int main(int argc, char* argv[]) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "There was an error\n";
		exit(1);
	}

	SDL_Window* win = SDL_CreateWindow("movement",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	SDL_GLContext context = SDL_GL_CreateContext(win);

	glClearColor(0.0, 0.0, 0.0, 1);

	glMatrixMode(GL_PROJECTION);
	gluPerspective(45, 640 / 480.f, 0.1, 100);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);

	SDL_Event event;

	bool full = false;

	float angle = 0;
	const double angPerMil = (0.2 * 360.0)/1000; // Angulo por milisegundo (Una vueta cada 5 segundos)
	double delta = 0; 
	Uint64 now = SDL_GetPerformanceCounter(); 
	Uint64 last; 
	do {

		last = now;
		now = SDL_GetPerformanceCounter();

		delta = ((now - last) * 1000 / (double)SDL_GetPerformanceFrequency());

		glMatrixMode(GL_MODELVIEW);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		glLoadIdentity();
		gluLookAt(0, 0, 6, 0, 0, 0, 0, 1, 0);


		angle = fmod((angle + angPerMil*delta), 360);

		
		// Rotación global
		glTranslatef(0, 0, -6);
		glRotatef(angle, 0, 1, 0);
		glTranslatef(0, 0, 6);

		// Triangulo
		glPushMatrix();
		glTranslatef(-1.5, 0, -6);
		glRotatef(angle , 0, 1, 0);
		glTranslatef(1.5, 0, 6);

		glBegin(GL_TRIANGLES); 
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(-1.5, 1, -6);
		glColor3f(0, 1.0, 0.0);
		glVertex3f(-2.5, -1, -6);
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(-0.5, -1, -6);
		glEnd(); 

		glPopMatrix();


		// Rectangulo

		glPushMatrix();
		glTranslatef(1.5, 0, -6);
		glRotatef(angle, 0, 1, 0);
		glTranslatef(-1.5, 0, 6);
		glBegin(GL_QUADS); 
		glColor3f(0.0, 1.0, 1.0);
		glVertex3f(0.5, 1, -6);
		glVertex3f(2.5, 1, -6);
		glVertex3f(2.5, -1, -6);
		glVertex3f(0.5, -1, -6);
		glEnd(); 

		glPopMatrix();

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
					return 0;
					break;
				case SDLK_F11:
					glMatrixMode(GL_PROJECTION);
					if (full) {
						SDL_SetWindowFullscreen(win, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
						full = false;
					}
					else {
						SDL_SetWindowFullscreen(win, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);
						full = true;
						std::cout << "FULL";
					}
					break;
				default:
					break;
				}
			}
		}
		SDL_GL_SwapWindow(win);
	} while (true);

	return 0;

}

