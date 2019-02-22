#pragma once

#include "stdafx.h"
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <array>

typedef std::array<float, 2> Point;

bool coupe(Point S, Point Pj, Point Fi, Point Fip1, bool ori);
Point intersection(Point S, Point Pj, Point Fi, Point Fip1);
bool visible(Point S, Point Fi, Point Fip1, bool ori);
bool sensFenetre(Point F0, Point F1, Point F2);
std::vector<Point> algo_Sutherland_Hodgman(std::vector<Point> polygone, std::vector<Point> fenetre);
std::vector<Point> inverse(std::vector<Point> _matrix);
Point matmul(std::vector<Point> _matrix, Point _vector);
