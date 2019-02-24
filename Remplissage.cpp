#include "stdafx.h"
#include "Remplissage.h"

infosCotesListe ajouterEnFin(infosCotesListe liste, std::vector<float> infos, float currentX = 0)
{
	infosCotesListe nouveauCote = new infosCotes;

	if (currentX == 0) {
		for (std::vector<float>::iterator it = infos.begin(); it != infos.end(); ++it) {
			if (it != infos.begin())	nouveauCote->informations.push_back(*it);
		}
	}
	else {
		nouveauCote->informations.push_back(infos[0]);
		nouveauCote->informations.push_back(currentX);
		nouveauCote->informations.push_back(infos[2]);
	}

	nouveauCote->next = NULL;

	if (liste == NULL || liste->informations.size() == 0)	return nouveauCote;
	else {
		infosCotesListe temp = liste;
		while (temp->next != NULL) temp = temp->next;
		temp->next = nouveauCote;
		return liste;
	}
}

std::vector<float> calculInfosCotePourSI(std::vector<std::array<float, 2>>::iterator it1, std::vector<std::array<float, 2>>::iterator it2) {
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
		invCoefDir = invCoefDir < 0.00001 && invCoefDir > -0.00001 ? 0 : 1 / invCoefDir;
	}

	newCote.push_back(yMin);
	newCote.push_back(yMax);
	newCote.push_back(xMin);
	newCote.push_back(invCoefDir);

	return newCote;
}

void swapInfosCotes(infosCotesListe a, infosCotesListe b) {
	std::vector<float> temp = a->informations;
	a->informations = b->informations;
	b->informations = temp;
}

void triABullesDeSI(infosCotesListe start) {
	int swapped, i;
	infosCotesListe ptr1 = new infosCotes;
	ptr1->next = NULL;
	infosCotesListe lptr = NULL;

	if (start == NULL) return;

	do {
		swapped = 0;
		ptr1 = start;
		while (ptr1->next != lptr) {
			if (ptr1->informations[1] > ptr1->next->informations[1]) {
				swapInfosCotes(ptr1, ptr1->next);
				swapped = 1;
			}
			else if (ptr1->informations[1] == ptr1->next->informations[1]) {
				if (ptr1->informations[2] > ptr1->next->informations[2]) {
					swapInfosCotes(ptr1, ptr1->next);
					swapped = 1;
				}
			}
			ptr1 = ptr1->next;
		}
		lptr = ptr1;
	} while (swapped);
}

std::map<float, infosCotesListe> creation_SI(std::vector<std::array<float, 2>> polygone) {
	std::map<float, infosCotesListe> SI;
	std::vector<float> newCote;

	for (std::vector<std::array<float, 2>>::iterator it = polygone.begin(); it != polygone.end(); ++it) {
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
	for (std::map<float, infosCotesListe>::iterator it = SI.begin(); it != SI.end(); ++it) {
		triABullesDeSI(it->second);
	}
	return SI;
}

std::vector<float> calculYMinMaxDeSI(std::map<float, infosCotesListe> SI) {
	std::vector<float> minMaxDeSI;
	float tmpYmin, tmpYmax, tmpXmin, tmpXmax;
	for (std::map<float, infosCotesListe>::iterator it = SI.begin(); it != SI.end(); ++it) {
		if (it == SI.begin()) tmpYmin = it->first;
		else if (it->first < tmpYmin) tmpYmin = it->first;

		if (it == SI.begin()) tmpYmax = it->second->informations[0];
		else if (it->second->informations[0] > tmpYmax) tmpYmax = it->second->informations[0];
	}
	minMaxDeSI.push_back(tmpYmin);
	minMaxDeSI.push_back(tmpYmax);
	return minMaxDeSI;
}

infosCotesListe triABullesDeLCA(infosCotesListe start) {
	int swapped, i;
	infosCotesListe ptr1 = new infosCotes;
	ptr1->next = NULL;
	infosCotesListe lptr = NULL;

	if (start == NULL) return NULL;

	do {
		swapped = 0;
		ptr1 = start;
		while (ptr1->next != lptr) {
			if (ptr1->informations[1] > ptr1->next->informations[1]) {
				swapInfosCotes(ptr1, ptr1->next);
				swapped = 1;
			}
			ptr1 = ptr1->next;
		}
		lptr = ptr1;
	} while (swapped);
	return start;
}

int nombreElements(infosCotesListe liste) {
	if (liste == NULL) return 0;
	return nombreElements(liste->next) + 1;
}

std::vector<Point> remplirRemplissage(infosCotesListe LCA, int i, std::vector<Point> remplissage) {
	bool trace = true;
	infosCotesListe tmp = new infosCotes;
	tmp->informations = LCA->informations;
	tmp->next = LCA->next;
	Point tmpArray;
	for (int j = 0; j < nombreElements(LCA); ++j) {
		if (trace) {
			tmpArray[0] = tmp->informations[1];
			tmpArray[1] = i;
			remplissage.push_back(tmpArray);
		}
		//if(j % 2 != 0) trace = !trace;
		tmp = tmp->next;
	}
	return remplissage;
}

infosCotesListe supprimerInfosCoteSiyMax(infosCotesListe liste, int valeur) {
	infosCotesListe temp;
	infosCotesListe previous;
	previous = liste;
	if (previous->informations[0] == valeur) {
		liste = previous->next;
		return liste;
	}
	temp = previous->next; 
	while (temp != NULL) {
		if (temp->informations[0] == valeur){
			previous->next = temp->next;
			return liste;
		}
		previous = temp; 
		temp = temp->next;
	}
	return liste;
}

infosCotesListe miseAJourLCA(std::map<float, infosCotesListe> SI, infosCotesListe LCA, int i) {
	infosCotesListe tmp;
	for (std::map<float, infosCotesListe>::iterator it = SI.begin(); it != SI.end(); ++it) {
		tmp = it->second;
		for (int j = 0; j < nombreElements(it->second); ++j) {
			if (it->first == i) LCA = ajouterEnFin(LCA, tmp->informations, tmp->informations[1]);
			if(tmp->informations[0] == i) LCA = supprimerInfosCoteSiyMax(LCA, tmp->informations[0]);
			tmp = tmp->next;
		}
	}
	return LCA;
}

infosCotesListe miseAJourCurrentX(infosCotesListe LCA) {
	infosCotesListe tete = LCA;
	while (LCA->next != NULL) {
		LCA->informations[1] += LCA->informations[2];
		LCA = LCA->next;
	}
	LCA->informations[1] += LCA->informations[2];
	LCA = tete;
	return LCA;
}

std::vector<std::array<float, 2>> algo_remplissage_LCA(std::vector<std::array<float, 2>> polygone) {
	std::map<float, infosCotesListe> SI = creation_SI(polygone);
	std::vector<Point> remplissage;
	infosCotesListe LCA = new infosCotes;
	LCA->next = NULL;
	std::vector<float> minMaxDeSI = calculYMinMaxDeSI(SI);
	float yMin = minMaxDeSI[0];
	float yMax = minMaxDeSI[1];
	for (int i = yMin; i < yMax; ++i) {
		LCA = miseAJourLCA(SI, LCA, i);
		LCA = triABullesDeLCA(LCA);
		remplissage = remplirRemplissage(LCA, i, remplissage);
		LCA = miseAJourCurrentX(LCA);
	}
	return remplissage;
}