#include "stdafx.h"
#include "Fenetrage.h"
#include "Remplissage.h"
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <array>

float vp_width = 800.0;
float vp_height = 600.0;

int nMenuprincipal, nSousMenuCouleurPolygone, nSousMenuCouleurFenetre, nSousMenuCouleurRemplissage, nSousMenuCouleur;

Point currentPt;
std::vector<Point> polygone;
std::vector<Point> fenetre;
std::vector<Point> fenetrage;
std::vector<Point> remplissage;
std::vector<std::vector<Point>> allPolygones;
std::vector<std::vector<Point>> allFenetrages;
std::vector<std::vector<Point>> allRemplissages;
bool closedPolygone = false;
bool closedFenetre = false;
bool typeTrace = false;
bool drawFenetrage = false;
bool traceRempli = false;
GLfloat colorPolygone[3] = { 0.75,0.0,0.0 };
GLfloat colorFenetre[3] = { 0.0,0.0,0.0 };
GLfloat colorRemplissage[3] = { 0.0,0.1f,0.0 };


void draw_polygon(int button, int state, int x, int y) {
	remplissage.clear();
	float fx = static_cast<float>(x);
	float fy = static_cast<float>(y);
	currentPt = Point{fx, vp_height - fy};

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (!typeTrace){
			if (closedPolygone)
				polygone.clear();
			closedPolygone = false;
			polygone.push_back(currentPt);
		}
		else{
			if (!allFenetrages.empty()) {
				allFenetrages.clear();
				if (traceRempli)
					traceRempli = false;
			}
			if (closedFenetre)
				fenetre.clear();
			closedFenetre = false;
			fenetre.push_back(currentPt);
		}
	}
	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
		if (!typeTrace) {
			closedPolygone = true;
			if (allFenetrages.empty()) {
				remplissage.clear();
				remplissage = algo_remplissage_LCA(polygone);
				allRemplissages.push_back(remplissage);
			}
			allPolygones.push_back(polygone);
			polygone.clear();
		}
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
		allFenetrages.clear();
		if (!fenetre.empty() && !allPolygones.empty()) {
			for (int i = 0; i < allPolygones.size(); ++i) {
				fenetrage.clear();
				fenetrage = algo_Sutherland_Hodgman(allPolygones[i], fenetre);
				allFenetrages.push_back(fenetrage);
			}
			if (traceRempli) {
				allRemplissages.clear();
				for (int i = 0; i < allFenetrages.size(); ++i) {
					remplissage.clear();
					remplissage = algo_remplissage_LCA(allFenetrages[i]);
					allRemplissages.push_back(remplissage);
				}
			}
		}
		break;

	case 4:
		traceRempli = true;
		if (!allFenetrages.empty()) {
			allRemplissages.clear();
			for (int i = 0; i < allFenetrages.size(); ++i) {
				remplissage.clear();
				remplissage = algo_remplissage_LCA(allFenetrages[i]);
				allRemplissages.push_back(remplissage);
			}
		}
		break;

	case 5:
		polygone.clear();
		fenetre.clear();
		fenetrage.clear();
		remplissage.clear();
		allPolygones.clear();
		allFenetrages.clear();
		allRemplissages.clear();
		traceRempli = false;
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

void vRappelSousMenuCouleurRemplissage(int i)
{
	switch (i) {
	case 131:
		colorRemplissage[0] = 1.0;
		colorRemplissage[1] = 0.0;
		colorRemplissage[2] = 0.0;
		break;

	case 132:
		colorRemplissage[0] = 0.0;
		colorRemplissage[1] = 1.0;
		colorRemplissage[2] = 0.0;
		break;

	case 133:
		colorRemplissage[0] = 0.0;
		colorRemplissage[1] = 0.0;
		colorRemplissage[2] = 1.0;
		break;
	}
}

void vRappelSousMenuCouleur(int i) {
	//rien à mettre là je crois
}

void mouse_move(int x, int y) {
	float fx = static_cast<float>(x);
	float fy = static_cast<float>(y);
	currentPt = Point{fx, vp_height - fy};
	glutPostRedisplay();
}

void drawPolygone() {
	glColor3fv(colorPolygone);
	for (int i = 0; i < allPolygones.size(); ++i) {
		glBegin(GL_LINE_STRIP);
		for (auto &pt : allPolygones[i])
			glVertex2f(pt[0], pt[1]);
		auto &endPt2 = allPolygones[i].front();
		glVertex2f(endPt2[0], endPt2[1]);
		glEnd();
	}
	if (!polygone.empty()) {
		glBegin(GL_LINE_STRIP);
		for (auto &pt : polygone)
			glVertex2f(pt[0], pt[1]);
		auto &endPt = closedPolygone ? polygone.front() : currentPt;
		glVertex2f(endPt[0], endPt[1]);
		glEnd();
	}
}

void drawFenetre() {
	glBegin(GL_LINE_STRIP);
	glColor3fv(colorFenetre);
	for (auto &pt : fenetre)
		glVertex2f(pt[0], pt[1]);
	auto &endPt2 = closedFenetre ? fenetre.front() : currentPt;
	glVertex2f(endPt2[0], endPt2[1]);
	glEnd();
}

void draw_Fenetrage() {
	glColor3f(1.f, 1.f, 1.f);
	for (int i = 0; i < allFenetrages.size(); ++i) {
		glBegin(GL_LINE_STRIP);
		for (auto &pt : allFenetrages[i])
			glVertex2f(pt[0], pt[1]);
		auto &endPt2 = allFenetrages[i].front();
		glVertex2f(endPt2[0], endPt2[1]);
		glEnd();
	}
}

void draw_Remplissage() {
	glColor3fv(colorRemplissage);
	for (int i = 0; i < allRemplissages.size(); ++i) {
		for (int j = 0; j < allRemplissages[i].size(); j += 2) {
			glBegin(GL_LINE_STRIP);
			glVertex2f(allRemplissages[i][j][0], allRemplissages[i][j][1]);
			glVertex2f(allRemplissages[i][j + 1][0], allRemplissages[i][j + 1][1]);
			glEnd();
		}
	}
}


void display(void)
{
	glClearColor(0.5, 0.5, 0.5, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	
	if (traceRempli)
		draw_Remplissage();
	drawPolygone();
	if(!fenetre.empty())
		drawFenetre();
	if (!allFenetrages.empty())
		draw_Fenetrage();

	glutSwapBuffers();
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(
		static_cast<int>(vp_width),
		static_cast<int>(vp_height)
	);
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

	nSousMenuCouleurRemplissage = glutCreateMenu(vRappelSousMenuCouleurRemplissage);
	glutAddMenuEntry("Couleur 1 remplissage", 131);
	glutAddMenuEntry("Couleur 2 remplissage", 132);
	glutAddMenuEntry("Couleur 3 remplissage", 133);

	nSousMenuCouleur = glutCreateMenu(vRappelSousMenuCouleur);
	glutAddSubMenu("Couleur du polygone", nSousMenuCouleurPolygone);
	glutAddSubMenu("Couleur de la fenêtre", nSousMenuCouleurFenetre);
	glutAddSubMenu("Couleur du remplissage", nSousMenuCouleurRemplissage);

	nMenuprincipal = glutCreateMenu(vRappelMenuPrincipal);
	glutAddSubMenu("Couleurs", nSousMenuCouleur);
	glutAddMenuEntry("Tracé polygone", 1);
	glutAddMenuEntry("Tracé fenêtre", 2);
	glutAddMenuEntry("Fenêtrage", 3);
	glutAddMenuEntry("Remplissage", 4);
	glutAddMenuEntry("Effacer tout", 5);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glMatrixMode(GL_PROJECTION);
	glOrtho(0.0f, vp_width, 0.0f, vp_height, -1.0, 1.0);
	glutMainLoop();
	return 0;
}

