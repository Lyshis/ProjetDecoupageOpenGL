#include "stdafx.h"
#include "Remplissage.h"

infosCotesListe ajouterEnFin(infosCotesListe liste, std::vector<float> infos)
{
	infosCotesListe nouveauCote = new infosCotes;

	for (std::vector<float>::iterator it = infos.begin(); it != infos.end(); ++it) {
		if(it != infos.begin())	nouveauCote->informations.push_back(*it);
	}

	nouveauCote->next = NULL;

	if (liste == NULL)	return nouveauCote;
	else {
		infosCotes* temp = liste;
		while (temp->next != NULL) temp = temp->next;
		temp->next = nouveauCote;
		return liste;
	}
}


std::vector<float> calculInfosCotePourSI(std::vector<std::array<int, 2>>::iterator it1, std::vector<std::array<int, 2>>::iterator it2) {
	std::vector<float> newCote;
	float yMin;
	float yMax;
	float xMin;
	float invCoefDir;

	if ((*it1)[1] > (*it2)[1]) {
		yMin = (*it2)[1];
		yMax = (*it1)[1];
		xMin = (*it2)[0];
	}
	else {
		yMin = (*it1)[1];
		yMax = (*it2)[1];
		xMin = (*it1)[0];
	}
	if ((*it1)[0] - (*it2)[0] < 0.00001 && (*it1)[0] - (*it2)[0] > -0.99999) invCoefDir = 0;
	else {
		invCoefDir = ((*it1)[1] - (*it2)[1]) / ((*it1)[0] - (*it2)[0]);
		invCoefDir = invCoefDir < 0.00001 && invCoefDir > -0.99999 ? 0 : 1 / invCoefDir;
	}

	newCote.push_back(yMin);
	newCote.push_back(yMax);
	newCote.push_back(xMin);
	newCote.push_back(invCoefDir);

	return newCote;
}

std::map<float, infosCotesListe> creation_SI(std::vector<std::array<int, 2>> polygone) {
	std::map<float, infosCotesListe> SI;
	std::vector<float> newCote;
	infosCotesListe newCoteInfos = {};

	for (std::vector<std::array<int, 2>>::iterator it = polygone.begin(); it != polygone.end(); ++it) {
		if (it + 1 == polygone.end()) {
			newCote = calculInfosCotePourSI(it, polygone.begin());
			if (SI.find(newCote[0]) == SI.end()) SI.insert(std::pair<float, infosCotesListe>(newCote[0], ajouterEnFin(NULL, newCote)));
			else SI.insert(std::pair<float, infosCotesListe>(newCote[0], ajouterEnFin(SI.find(newCote[0])->second, newCote)));
		}
		else {
			newCote = calculInfosCotePourSI(it, it + 1);
			if (SI.find(newCote[0]) == SI.end()) SI.insert(std::pair<float, infosCotesListe>(newCote[0], ajouterEnFin(NULL, newCote)));
			else SI.insert(std::pair<float, infosCotesListe>(newCote[0], ajouterEnFin(SI.find(newCote[0])->second, newCote)));
		}
	}

	return SI;
}

void remplissage_LCA(std::vector<std::array<int, 2>> polygone) {
	std::map<float, infosCotesListe> SI = creation_SI(polygone);
}