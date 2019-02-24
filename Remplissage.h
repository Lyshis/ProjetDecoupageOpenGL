#pragma once

#include "stdafx.h"
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <array>
#include <stdlib.h>
#include <map>
#include <utility>
#include "Fenetrage.h"

typedef struct infosCotes infosCotes;
struct infosCotes
{
	std::vector<float> informations;
	infosCotes *next;
};

typedef infosCotes* infosCotesListe;

std::vector<float> calculInfosCotePourSI(std::vector<Point>::iterator it1, std::vector<Point>::iterator it2);
std::map<float, infosCotesListe> creation_SI(std::vector<Point> polygone);
std::vector<Point> algo_remplissage_LCA(std::vector<Point> polygone);