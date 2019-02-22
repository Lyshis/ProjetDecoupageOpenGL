#include "stdafx.h"
#include "Fenetrage.h"

bool coupe(Point S, Point Pj, Point Fi, Point Fip1, bool ori) {
	bool Svis = visible(S, Fi, Fip1, ori);
	bool Pjvis = visible(Pj, Fi, Fip1, ori);
	bool ret = false;
	if (Svis != Pjvis)
		ret = true;
	return ret;
}


std::vector<Point> inverse(std::vector<Point> _matrix) {
	float det = _matrix[0][0] * _matrix[1][1] - _matrix[1][0] * _matrix[0][1];
	std::vector<Point> Inv;
	Point vec1;
	Point vec2;
	vec1[0] = _matrix[1][1] / (det);
	vec2[0] = -_matrix[1][0] / (det);
	vec1[1] = -_matrix[0][1] / (det);
	vec2[1] = _matrix[0][0] / (det);
	Inv.push_back(vec1);
	Inv.push_back(vec2);
	return Inv;
}

Point matmul(std::vector<Point> _matrix, Point _vector) {
	Point retour;
	retour[0] = _vector[0] * _matrix[0][0] +_vector[1] * _matrix[1][0];
	retour[1] = _vector[0] * _matrix[0][1] + _vector[1] * _matrix[1][1];
	return retour;
}

Point intersection(Point S, Point Pj, Point Fi, Point Fip1) {
	Point matVec1;
	Point matVec2;
	Point vec;
	matVec1[0] = Pj[0] - S[0];
	matVec2[0] = Fi[0] - Fip1[0];
	matVec1[1] = Pj[1] - S[1];
	matVec2[1] = Fi[1] - Fip1[1];
	std::vector<Point> matA;
	matA.push_back(matVec1);
	matA.push_back(matVec2);
	vec[0] = Fi[0] - S[0];
	vec[1] = Fi[1] - S[1];
	std::vector<Point> Inv;
	Inv = inverse(matA);
	Point vecX = matmul(Inv, vec);
	float s = vecX[1];
	Point retour;
	retour[0] = Fi[0] + (Fip1[0] - Fi[0])*s;
	retour[1] = Fi[1] + (Fip1[1] - Fi[1])*s;
	return retour;
}


bool visible(Point S, Point Fi, Point Fip1, bool ori) {
	float x = S[0] - Fi[0];
	float y = S[1] - Fi[1];
	float xp = Fip1[0] - Fi[0];
	float yp = Fip1[1] - Fi[1];
	float orientation = x * yp - y * xp;

	return (orientation >= 0) ^ !ori; // retourne true si le point est à droite. Inverse la visibilitée du point si la fenetre est antihoraire
}

/* determine le sens de la fentre : true horaire false antihoraire */
/* F0 F1 et F2 les trois premiers points de la fenetre */
/* la fenetre doit etre concave */
bool sensFenetre(Point F0, Point F1, Point F2) {
	float x = F2[0] - F0[0];
	float y = F2[1] - F0[1];
	float xp = F1[0] - F0[0];
	float yp = F1[1] - F0[1];
	float orientation = x * yp - y * xp;

	return orientation >= 0;
}

std::vector<Point> algo_Sutherland_Hodgman(std::vector<Point> polygone, std::vector<Point> fenetre) {
	int N1, N2, i, j;
	Point S, F, I;
	std::vector<Point> PS;
	std::vector<Point> vectorRetour;
	for (auto &pt : polygone) {
		vectorRetour.push_back(pt);
	}
	std::vector<Point> vectorFenetreExtend;
	for (auto &pt : fenetre) {
		vectorFenetreExtend.push_back(pt);
	}
	vectorFenetreExtend.push_back(fenetre[0]);
	
	N1 = static_cast<int>(vectorRetour.size());
	bool sens = (fenetre.size() < 3) ?
		true :
		sensFenetre(fenetre[0], fenetre[1], fenetre[2]);

	for (i = 0; i < vectorFenetreExtend.size()-1;i++) {
		N2 = 0;
		PS.clear();
		for (j = 0; j < N1; j++) {
			if (j == 0) {
				F[0] = vectorRetour[j][0];
				F[1] = vectorRetour[j][1];
			}
			else {
				if (coupe(S, vectorRetour[j], vectorFenetreExtend[i], vectorFenetreExtend[i+1], sens)) {
					I = intersection(S, vectorRetour[j], vectorFenetreExtend[i], vectorFenetreExtend[i + 1]);
					PS.push_back(I);
					N2++;
				}
			}
			S[0] = vectorRetour[j][0];
			S[1] = vectorRetour[j][1];
			if (visible(S, vectorFenetreExtend[i], vectorFenetreExtend[i + 1], sens)) {
				PS.push_back(S);
				N2++;
			}
		}
		if (N2 > 0) {
			if (coupe(S, F, vectorFenetreExtend[i], vectorFenetreExtend[i + 1], sens)) {
				I = intersection(S, F, vectorFenetreExtend[i], vectorFenetreExtend[i + 1]);
				PS.push_back(I);
				N2++;
			}
			vectorRetour.clear();
			for (auto &pt : PS) {
				Point temp;
				temp[0] = pt[0];
				temp[1] = pt[1];
				vectorRetour.push_back(temp);
			}
			N1 = N2;
		}
	}

	return vectorRetour; 
}
