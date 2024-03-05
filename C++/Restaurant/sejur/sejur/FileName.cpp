#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string.h>
using namespace std;

class ExceptieInput {
	string mesaj;
public:
	ExceptieInput(string mesaj) :mesaj(mesaj) {}
	string what() {
		return this->mesaj;
	}
};

class Sejur {

	const int ID;
	string destinatie = "nicaieri";
	float pretBaza = 0;
	int nrExcursiiOptionale = 0;
	float* pretExcursie = nullptr;
	int nota = 0;
	string dataPlecare = "niciodata";

public:

	Sejur(int ID) :ID(ID) {

	}

	Sejur(int ID, string destinatie, float pretBaza, int nrExcursiiOptionale, const float* pretExcursie, int nota, string dataPlecare) :ID(ID) {
		this->destinatie = destinatie;
		this->pretBaza = pretBaza;
		this->nrExcursiiOptionale = nrExcursiiOptionale;
		if (nrExcursiiOptionale > 0 && pretExcursie != nullptr) {
			this->pretExcursie = new float[this->nrExcursiiOptionale];
			for (int i = 0; i < this->nrExcursiiOptionale; i++) {
				this->pretExcursie[i] = pretExcursie[i];
			}
		}
		if (nota >= 0 && nota <= 10) {
			this->nota = nota;
		}
		this->dataPlecare = dataPlecare;
	}

	Sejur(const Sejur& s) :ID(s.ID) {
		this->destinatie = s.destinatie;
		this->pretBaza = s.pretBaza;
		this->nrExcursiiOptionale = s.nrExcursiiOptionale;
		if (s.nrExcursiiOptionale > 0 && s.pretExcursie != nullptr) {
			this->pretExcursie = new float[s.nrExcursiiOptionale];
			for (int i = 0; i < s.nrExcursiiOptionale; i++) {
				this->pretExcursie[i] = s.pretExcursie[i];
			}
		}
		if (s.nota >= 0 && s.nota <= 10) {
			this->nota = s.nota;
		}
		this->dataPlecare = s.dataPlecare;
	}

	void setNota(int notaNou) {
		if (notaNou >= 0 && notaNou <= 10) {
			this->nota = notaNou;
		}
		else {
			throw ExceptieInput("\nNota nu este valida.");
		}
	}

	void setDataPlecare(const string dataPlecareNou) {
		if (dataPlecareNou.length()>5) {
			this->dataPlecare = dataPlecareNou;
		}
		else {
			throw ExceptieInput("\nData invalida.");
		}
	}

	int getNota() {
		return this->nota;
	}

	string getDataPlecare() {
		return this->dataPlecare;
	}

	int getNrMaximExcursii(float bugetMaximSejur) {
		int nrMaximExcursii = 0;
		int j;
		float baniRamasi = bugetMaximSejur;
		float* copie;
		copie = new float[this->nrExcursiiOptionale];
		for (int i = 0; i < this->nrExcursiiOptionale; i++) {
			copie[i] = this->pretExcursie[i];
		}
		if (baniRamasi >= this->pretBaza) {
			baniRamasi = baniRamasi - this->pretBaza;
			while (nrMaximExcursii != nrExcursiiOptionale) {
				float minim = 99999;
				//cautam cel mai mic pret
				for (int i = 0; i < this->nrExcursiiOptionale; i++) {
					if (copie[i] < minim && copie[i] != -1) {
						minim = copie[i];
						j = i; //tinem minte pozitia
					}
				}
				if (baniRamasi >= minim) {
					baniRamasi = baniRamasi - minim;
					nrMaximExcursii++;
					copie[j] = -1;
				}
				else {
					break;
				}
			}
		}
		delete[] copie;
		return nrMaximExcursii;
	}

	Sejur& operator= (const Sejur& s) {
		if (&s != this) {
			delete[] this->pretExcursie;
			this->pretExcursie = nullptr;
			this->destinatie = s.destinatie;
			this->pretBaza = s.pretBaza;
			this->nrExcursiiOptionale = s.nrExcursiiOptionale;
			if (s.nrExcursiiOptionale > 0 && s.pretExcursie != nullptr) {
				this->pretExcursie = new float[s.nrExcursiiOptionale];
				for (int i = 0; i < s.nrExcursiiOptionale; i++) {
					this->pretExcursie[i] = s.pretExcursie[i];
				}
			}
			if (s.nota >= 0 && s.nota <= 10) {
				this->nota = s.nota;
			}
			this->dataPlecare = s.dataPlecare;
		}
		return *this;
	}
	
	Sejur operator--(int) {
		int nrExcursiiCuAcelasiPret = 1;
		Sejur copie = *this;
		float minim = pretExcursie[0];
		for (int i = 1; i < this->nrExcursiiOptionale; i++) {
			if (this->pretExcursie[i] < minim) {
				minim = pretExcursie[i];
				nrExcursiiCuAcelasiPret = 1;
			}
			else if (this->pretExcursie[i] == minim) {
					nrExcursiiCuAcelasiPret++;
			}
		}
		while (nrExcursiiCuAcelasiPret > 0) {
			for (int i = 0; i < this->nrExcursiiOptionale; i++) {
				if (this->pretExcursie[i] == minim) {
					for (int j = i; j < this->nrExcursiiOptionale - 1; j++) {
						this->pretExcursie[j] = this->pretExcursie[j + 1];
					}
					this->nrExcursiiOptionale--;
					nrExcursiiCuAcelasiPret--;
					break;
				}
			}
		}
		return copie;
	}

	bool operator<(float cost) {
		float maxim = this->pretExcursie[0];
		for (int i = 1; i < nrExcursiiOptionale; i++) {
			if (this->pretExcursie[i] > maxim) {
				maxim = this->pretExcursie[i];
			}
		}
		if (maxim < cost) {
			return true;
		}
		else {
			return false;
		}
	}

	friend bool operator<=(float cost, Sejur s) {
		float maxim = s.pretExcursie[0];
		for (int i = 1; i < s.nrExcursiiOptionale; i++) {
			if (s.pretExcursie[i] > maxim) {
				maxim = s.pretExcursie[i];
			}
		}
		if (cost <= maxim) {
			return true;
		}
		else {
			return false;
		}
	}

	friend istream& operator>>(istream& in, Sejur& s) {
		delete[] s.pretExcursie;
		s.pretExcursie = nullptr;

		cout << "\nIntrodu destinatia: ";
		in >> s.destinatie;
		cout << "\nIntrodu pretul de baza: ";
		in >> s.pretBaza;
		cout << "\nIntrodu numarul de excursii optionale: ";
		in >> s.nrExcursiiOptionale;
		if (s.nrExcursiiOptionale <= 0) {
			s.nrExcursiiOptionale = 0;
			s.pretExcursie = nullptr;
		}
		else {
			s.pretExcursie = new float[s.nrExcursiiOptionale];
			cout << "\nIntrodu pretul pentru fiecare excursie: ";
			for (int i = 0; i < s.nrExcursiiOptionale; i++) {
				cout << "\nPret[" << i + 1 << "]= ";
				in >> s.pretExcursie[i];
			}
		}
		cout << "\nIntrodu nota sujurului: ";
		in >> s.nota;
		cout << "\nIntrodu data de plecare: ";
		in >> s.dataPlecare;
		return in;
	}

	friend ostream& operator<<(ostream& out, const Sejur& s) {
		out << "\n*************************************************";
		out << "\nId-ul este: " << s.ID;
		out << "\nDenumire: " << s.destinatie;
		out << "\nPretul de baza: " << s.pretBaza;
		out << "\nNumarul de excursii optionale: " << s.nrExcursiiOptionale;
		out << "\nPreturile pt excursiile optionale: ";
		if (s.nrExcursiiOptionale > 0 && s.pretExcursie != nullptr) {
			for (int i = 0; i < s.nrExcursiiOptionale; i++) {
				out << " " << s.pretExcursie[i];
			}
		}
		if (s.nota <= 10 && s.nota >= 0) {
			out << "\nNota sejurului (0-10): " << s.nota;
		}
		out << "\nData plecarii: " << s.dataPlecare;
		out << "\n**************************************************";
		return out;
	}

	~Sejur() {
		delete[] pretExcursie;
		pretExcursie = nullptr;
	}

	explicit operator float() {
		float costTotal;
		costTotal = this->pretBaza;
		for (int i = 0; i < nrExcursiiOptionale; i++) {
			costTotal += pretExcursie[i];
		}
		return costTotal;
	}
};

int main() {

	float preturi[] = {200,400,300};
	Sejur s1(1234, "Craiova", 1000, 3, preturi, 8, "decembrie");
	cout << s1;

	//s1--;
	//cout<<s1;

	//Sejur s3 = s1;
	//cout << s3;

	//Sejur s2(s1);
	//cout << s2;

	//if (600 <= s1) cout << "da";
	//else cout << "nu";

	//Sejur s2(123);
	//cout << s2;
	//cin >> s2;
	//cout << s2;

	//Sejur s5(232);
	//cout << s5;
	//cout << "\n" << s1.getNrMaximExcursii(1500);

	//cout << (float)s1;

	//try {
	//	s1.setNota(11);
	//}
	//catch (ExceptieInput &ex) {
	//	cout << ex.what();
	//}
	//s1.setDataPlecare("noiembrie");
	//cout << s1;

	//cout << s1.getNota();
	//cout << s1.getDataPlecare();

	return 0;
}