#include "stdafx.h"
#include "Fenetrage.h"


bool coupe(std::array<int, 2> S, std::array<int, 2> Pj, std::array<int, 2> Fi, std::array<int, 2> Fip1) {
	bool Svis = visible(S, Fi, Fip1);
	bool Pjvis = visible(Pj, Fi, Fip1);
	bool ret = false;
	if (Svis != Pjvis)
		ret = true;
	return ret;
}


void inverse(std::vector<std::array<int, 2>> _matrix) {
	int det = _matrix[0][0] * _matrix[1][1] - _matrix[1][0] * _matrix[0][1];
	int temp = _matrix[0][0];
	_matrix[0][0] = _matrix[1][1] / (det);
	_matrix[1][0] = -_matrix[1][0] / (det);
	_matrix[0][1] = -_matrix[0][1] / (det);
	_matrix[1][1] = temp / (det);
}

std::array<int, 2> matmul(std::vector<std::array<int, 2>> _matrix, std::array<int, 2> _vector) {
	std::array<int, 2> retour;
	retour[0] = _vector[0] * _matrix[0][0] +_vector[1] * _matrix[1][0];
	retour[1] = _vector[0] * _matrix[0][1] + _vector[1] * _matrix[1][1];
	return retour;
}

std::array<int, 2> intersection(std::array<int, 2> S, std::array<int, 2> Pj, std::array<int, 2> Fi, std::array<int, 2> Fip1) {
	std::vector<std::array<int, 2>> matA;
	std::array<int, 2> vec;
	matA[0][0] = Pj[0] - S[0];
	matA[1][0] = Fi[0] - Fip1[0];
	matA[0][1] = Pj[1] - S[1];
	matA[1][1] = Fi[1] - Fip1[1];
	vec[0] = Fi[0] - S[0];
	vec[1] = Fi[1] - S[1];
	inverse(matA);
	std::array<int, 2> retour = matmul(matA, vec);
	int s = retour[1];
	retour[0] = Fi[0] + (Fip1[0] - Fi[0])*s;
	retour[1] = Fi[1] + (Fip1[1] - Fi[1])*s;
	return retour;
}


bool visible(std::array<int, 2> S, std::array<int, 2> Fi, std::array<int, 2> Fip1) {
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


std::vector<std::array<int, 2>> algo_Sutherland_Hodgman(std::vector<std::array<int, 2>> polygone, std::vector<std::array<int, 2>> fenetre) {
	int N2, i, j;
	std::array<int, 2> S, F, I;
	std::vector<std::array<int, 2>> PS;
	std::vector<std::array<int, 2>> vectorRetour;
	for (auto &pt : polygone) {
		std::array<int, 2> temp;
		temp[0] = pt[0];
		temp[1] = pt[1];
		vectorRetour.push_back(temp);
	}

	for (i = 1; i < fenetre.size()-2;i++) {
		N2 = 0;
		PS.clear();
		for (j = 1; j < vectorRetour.size() - 1; j++) {
			if (j == 1) {
				F[0] = vectorRetour[j][0];
				F[1] = vectorRetour[j][1];
			}
			else {
				if (coupe(S, vectorRetour[j],fenetre[i],fenetre[i+1])) {
					I = intersection(S, vectorRetour[j], fenetre[i], fenetre[i + 1]);	//possiblement des erreurs sur le retour de fonction et le =
					PS.push_back(I);
					N2++;
				}
			}
			S[0] = vectorRetour[j][0];
			S[1] = vectorRetour[j][1];
			if (visible(S, fenetre[i], fenetre[i + 1])) {
				PS.push_back(S);
				N2++;
			}
		}
		if (N2 > 0) {
			if (coupe(S, F, fenetre[i], fenetre[i + 1])) {
				I = intersection(S, F, fenetre[i], fenetre[i + 1]);	//possiblement des erreurs sur le retour de fonction et le =
				PS.push_back(I);
				N2++;
			}
			vectorRetour.clear();
			for (auto &pt : PS) {
				std::array<int, 2> temp;
				temp[0] = pt[0];
				temp[1] = pt[1];
				vectorRetour.push_back(temp);
			}
		}
	}

	return vectorRetour;
}