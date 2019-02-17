#pragma once

#include "stdafx.h"
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <array>

bool coupe(std::array<float, 2> S, std::array<float, 2> Pj, std::array<float, 2> Fi, std::array<float, 2> Fip1);
std::array<float, 2> intersection(std::array<float, 2> S, std::array<float, 2> Pj, std::array<float, 2> Fi, std::array<float, 2> Fip1);
bool visible(std::array<float, 2> S, std::array<float, 2> Fi, std::array<float, 2> Fip1);
std::vector<std::array<float, 2>> algo_Sutherland_Hodgman(std::vector<std::array<float, 2>> polygone, std::vector<std::array<float, 2>> fenetre);
std::vector<std::array<float, 2>> inverse(std::vector<std::array<float, 2>> _matrix);
std::array<float, 2> matmul(std::vector<std::array<float, 2>> _matrix, std::array<float, 2> _vector);