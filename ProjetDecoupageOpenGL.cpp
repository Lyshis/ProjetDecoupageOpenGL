// ProjetDecoupageOpenGL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Fenetrage.h"
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <array>

//#pragma comment(lib, "opengl32.lib")
//#pragma comment(lib, "glu32.lib")

int vp_width = 800;
int vp_height = 600;

int nMenuprincipal, nSousMenuCouleurPolygone, nSousMenuCouleurFenetre, nSousMenuCouleur;

std::array<int, 2> currentPt;
std::vector<std::array<int, 2>> polygone;
std::vector<std::array<int, 2>> fenetre;
bool closedPolygone = false;
bool closedFenetre = false;
bool typeTrace = false;
GLfloat colorPolygone[3] = { 0.75,0.0,0.0 };
GLfloat colorFenetre[3] = { 0.0,0.0,0.0 };

void draw_polygon(int button, int state, int x, int y)
{
	currentPt = std::array<int, 2>{x, vp_height - y};

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (typeTrace)
		{
			if (closedPolygone)
				polygone.clear();
			closedPolygone = false;
			polygone.push_back(currentPt);
		}
		else
		{

			if (closedFenetre)
				fenetre.clear();
			closedFenetre = false;
			fenetre.push_back(currentPt);
		}

	}
	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
		if (typeTrace)
			closedPolygone = true;
		else
			closedFenetre = true;
	}
}


void vRappelMenuPrincipal(int i) {
	switch (i) {
	case 1:
		typeTrace = false;
		break;

	case 2:
		typeTrace = true;
		break;

	case 3:
		//fonction fenêtrage ici et exploitation
		algo_Sutherland_Hodgman(polygone, fenetre);
		break;

	case 4:
		//fonction remplissage ici
		break;
	}
}

void vRappelSousMenuCouleurPolygone(int i)
{
	switch (i) {
	case 111:
		colorPolygone[0] = 1.0;
		colorPolygone[1] = 0.0;
		colorPolygone[2] = 0.0;
		break;

	case 112:
		colorPolygone[0] = 0.0;
		colorPolygone[1] = 1.0;
		colorPolygone[2] = 0.0;
		break;

	case 113:
		colorPolygone[0] = 0.0;
		colorPolygone[1] = 0.0;
		colorPolygone[2] = 1.0;
		break;
	}
}

void vRappelSousMenuCouleurFenetre(int i)
{
	switch (i) {
	case 121:
		colorFenetre[0] = 0.0;
		colorFenetre[1] = 1.0;
		colorFenetre[2] = 1.0;
		break;

	case 122:
		colorFenetre[0] = 1.0;
		colorFenetre[1] = 0.0;
		colorFenetre[2] = 1.0;
		break;

	case 123:
		colorFenetre[0] = 1.0;
		colorFenetre[1] = 1.0;
		colorFenetre[2] = 0.0;
		break;
	}
}

void vRappelSousMenuCouleur(int i)
{
	//rien à mettre là je crois
}


void mouse_move(int x, int y)
{
	currentPt = std::array<int, 2>{x, vp_height - y};
	glutPostRedisplay();
}

void drawPolygone() {
	glBegin(GL_LINE_STRIP);
	glColor3fv(colorFenetre);			//je sais que ça devrait être polygone ici, mais je comprends pas pourquoi les deux sont inversés
	for (auto &pt : polygone)
		glVertex2f((float)pt[0], (float)pt[1]);
	auto &endPt = closedPolygone ? polygone.front() : currentPt;
	glVertex2f((float)endPt[0], (float)endPt[1]);
	glEnd();
}


void drawFenetre() {
	glBegin(GL_LINE_STRIP);
	glColor3fv(colorPolygone);			//pareil ici
	for (auto &pt : fenetre)
		glVertex2f((float)pt[0], (float)pt[1]);
	auto &endPt2 = closedFenetre ? fenetre.front() : currentPt;
	glVertex2f((float)endPt2[0], (float)endPt2[1]);
	glEnd();
}


void display(void)
{
	glClearColor(0.5, 0.5, 0.5, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	drawPolygone();
	drawFenetre();

	//glFlush();
	glutSwapBuffers();
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(vp_width, vp_height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("OpenGL Sample Drawing");


	glutDisplayFunc(display);
	glutPassiveMotionFunc(mouse_move);
	glutMouseFunc(draw_polygon);


	nSousMenuCouleurPolygone = glutCreateMenu(vRappelSousMenuCouleurPolygone);
	glutAddMenuEntry("Couleur 1 polygone", 111);
	glutAddMenuEntry("Couleur 2 polygone", 112);
	glutAddMenuEntry("Couleur 3 polygone", 113);

	nSousMenuCouleurFenetre = glutCreateMenu(vRappelSousMenuCouleurFenetre);
	glutAddMenuEntry("Couleur 1 fenetre", 121);
	glutAddMenuEntry("Couleur 2 fenetre", 122);
	glutAddMenuEntry("Couleur 3 fenetre", 123);

	nSousMenuCouleur = glutCreateMenu(vRappelSousMenuCouleur);
	glutAddSubMenu("Couleur du polygone", nSousMenuCouleurPolygone);
	glutAddSubMenu("Couleur de la fenêtre", nSousMenuCouleurFenetre);

	nMenuprincipal = glutCreateMenu(vRappelMenuPrincipal);
	glutAddSubMenu("couleurs", nSousMenuCouleur);
	glutAddMenuEntry("tracé polygone", 1);
	glutAddMenuEntry("tracé fenêtre", 2);
	glutAddMenuEntry("fenêtrage", 3);
	glutAddMenuEntry("remplissage", 4);
	glutAttachMenu(GLUT_RIGHT_BUTTON);


	glMatrixMode(GL_PROJECTION);
	glOrtho(0.0f, (float)vp_width, 0.0f, (float)vp_height, -1.0, 1.0);
	glutMainLoop();
	return 0;
}

