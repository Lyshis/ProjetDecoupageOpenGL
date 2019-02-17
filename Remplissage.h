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

typedef struct infosCotes infosCotes;
struct infosCotes
{
	std::vector<float> informations;
	struct infosCotes *next;
};

typedef infosCotes* infosCotesListe;

std::vector<float> calculInfosCotePourSI(std::vector<std::array<float, 2>>::iterator it1, std::vector<std::array<float, 2>>::iterator it2);
std::map<float, infosCotesListe> creation_SI(std::vector<std::array<float, 2>> polygone);
void remplissage_LCA(std::vector<std::array<float, 2>> polygone);