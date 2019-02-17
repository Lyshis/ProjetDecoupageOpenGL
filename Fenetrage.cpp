#include "stdafx.h"
#include "Fenetrage.h"


bool coupe(std::array<float, 2> S, std::array<float, 2> Pj, std::array<float, 2> Fi, std::array<float, 2> Fip1) {
	bool Svis = visible(S, Fi, Fip1);
	bool Pjvis = visible(Pj, Fi, Fip1);
	bool ret = false;
	if (Svis != Pjvis)
		ret = true;
	return ret;
}


std::vector<std::array<float, 2>> inverse(std::vector<std::array<float, 2>> _matrix) {
	float det = _matrix[0][0] * _matrix[1][1] - _matrix[1][0] * _matrix[0][1];
	std::vector<std::array<float, 2>> Inv;
	std::array<float, 2> vec1;
	std::array<float, 2> vec2;
	vec1[0] = _matrix[1][1] / (det);
	vec2[0] = -_matrix[1][0] / (det);
	vec1[1] = -_matrix[0][1] / (det);
	vec2[1] = _matrix[0][0] / (det);
	Inv.push_back(vec1);
	Inv.push_back(vec2);
	return Inv;
}

std::array<float, 2> matmul(std::vector<std::array<float, 2>> _matrix, std::array<float, 2> _vector) {
	std::array<float, 2> retour;
	retour[0] = _vector[0] * _matrix[0][0] +_vector[1] * _matrix[1][0];
	retour[1] = _vector[0] * _matrix[0][1] + _vector[1] * _matrix[1][1];
	return retour;
}

std::array<float, 2> intersection(std::array<float, 2> S, std::array<float, 2> Pj, std::array<float, 2> Fi, std::array<float, 2> Fip1) {
	std::array<float, 2> matVec1;
	std::array<float, 2> matVec2;
	std::array<float, 2> vec;
	matVec1[0] = Pj[0] - S[0];
	matVec2[0] = Fi[0] - Fip1[0];
	matVec1[1] = Pj[1] - S[1];
	matVec2[1] = Fi[1] - Fip1[1];
	std::vector<std::array<float, 2>> matA;
	matA.push_back(matVec1);
	matA.push_back(matVec2);
	vec[0] = Fi[0] - S[0];
	vec[1] = Fi[1] - S[1];
	std::vector<std::array<float, 2>> Inv;
	Inv = inverse(matA);
	std::array<float, 2> vecX = matmul(Inv, vec);
	float s = vecX[1];
	std::array<float, 2> retour;
	retour[0] = Fi[0] + (Fip1[0] - Fi[0])*s;
	retour[1] = Fi[1] + (Fip1[1] - Fi[1])*s;
	return retour;
}


bool visible(std::array<float, 2> S, std::array<float, 2> Fi, std::array<float, 2> Fip1) {
	int x = S[0] - Fi[0];
	int y = S[1] - Fi[1];
	int xp = Fip1[0] - Fi[0];
	int yp = Fip1[1] - Fi[1];
	int orientation = x * yp - y * xp;

	bool ret;
	if (orientation > 0) {
		ret = true;		//on suppose qu'être à droite veut toujours dire être dans la fenêtre
	}
	else if (orientation < 0) {
		ret = false;
	}
	else {
		ret = true; //on le compte comme visible s'il est sur la ligne ?
	}

	return ret;
}


std::vector<std::array<float, 2>> algo_Sutherland_Hodgman(std::vector<std::array<float, 2>> polygone, std::vector<std::array<float, 2>> fenetre) {
	int N1, N2, i, j;
	std::array<float, 2> S, F, I;
	std::vector<std::array<float, 2>> PS;
	std::vector<std::array<float, 2>> vectorRetour;
	for (auto &pt : polygone) {
		vectorRetour.push_back(pt);
	}
	std::vector<std::array<float, 2>> vectorFenetreExtend;
	for (auto &pt : fenetre) {
		vectorFenetreExtend.push_back(pt);
	}
	vectorFenetreExtend.push_back(fenetre[0]);
	
	N1 = vectorRetour.size();
	for (i = 0; i < vectorFenetreExtend.size()-1;i++) {
		N2 = 0;
		PS.clear();
		for (j = 0; j < N1; j++) {
			if (j == 0) {
				F[0] = vectorRetour[j][0];
				F[1] = vectorRetour[j][1];
			}
			else {
				if (coupe(S, vectorRetour[j], vectorFenetreExtend[i], vectorFenetreExtend[i+1])) {
					I = intersection(S, vectorRetour[j], vectorFenetreExtend[i], vectorFenetreExtend[i + 1]);
					PS.push_back(I);
					N2++;
				}
			}
			S[0] = vectorRetour[j][0];
			S[1] = vectorRetour[j][1];
			if (visible(S, vectorFenetreExtend[i], vectorFenetreExtend[i + 1])) {
				PS.push_back(S);
				N2++;
			}
		}
		if (N2 > 0) {
			if (coupe(S, F, vectorFenetreExtend[i], vectorFenetreExtend[i + 1])) {
				I = intersection(S, F, vectorFenetreExtend[i], vectorFenetreExtend[i + 1]);
				PS.push_back(I);
				N2++;
			}
			vectorRetour.clear();
			for (auto &pt : PS) {
				std::array<float, 2> temp;
				temp[0] = pt[0];
				temp[1] = pt[1];
				vectorRetour.push_back(temp);
			}
			N1 = N2;
		}
	}

	return vectorRetour; 
}