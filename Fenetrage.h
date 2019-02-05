#pragma once

#include "stdafx.h"
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <array>

bool coupe(std::array<int, 2> S, std::array<int, 2> Pj, std::array<int, 2> Fi, std::array<int, 2> Fip1);
std::array<int, 2> intersection(std::array<int, 2> S, std::array<int, 2> Pj, std::array<int, 2> Fi, std::array<int, 2> Fip1);
bool visible(std::array<int, 2> S, std::array<int, 2> Fi, std::array<int, 2> Fip1);
std::vector<std::array<int, 2>> algo_Sutherland_Hodgman(std::vector<std::array<int, 2>> polygone, std::vector<std::array<int, 2>> fenetre);
std::vector<std::array<float, 2>> inverse(std::vector<std::array<float, 2>> _matrix);
std::array<float, 2> matmul(std::vector<std::array<float, 2>> _matrix, std::array<float, 2> _vector);