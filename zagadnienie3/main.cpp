#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <fstream>
#include <iostream>

#define A 10							// stała potrzebna przy funkcji rastrigina
#define LEARNING_RATE 0.5					// współczynnik nauki
#define N_INPUT 2						// liczba danych wejściowych
#define N_LAYER1 3						// liczba węzłów wejściowych
#define N_LAYER2 2						// liczba węzłów ukrytych
#define N_LAYER3 1						// liczba węzłów wyjściowych

#define FILENAME "out/01w05n3n2n1N2.xlsx"

using namespace std;

struct FirstLayer{						// warstwa pierwsza odpowiadająca za warstwę wejściową
	vector<double> weights;					// wagi dla danych wejściowych
	vector<double> output;					// sumy = wagi * odpowiadające im dane wejściowe
	vector<double> delta;					// błędy wsteczne, uzależnione od błędów na wyższych warstwach

	FirstLayer();
	void sum(vector<double> input);				// sumowanie wag * dane wejściowe
	void correctDelta(vector<double>,vector<double>);	// ustawienie błędóœ wstecznych
	void correctWeight(vector<double>);			// poprawianie wag
};

struct SecondLayer{						// warstwa ukryta odpowiadająca za warstwę wejściową
	vector<double> weights;					// wagi dla danych wejściowych
	vector<double> output;					// sumy = wagi * odpowiadające im dane wejściowe
	vector<double> delta;					// błędy wsteczne, uzależnione od błędów na wyższych warstwach

	SecondLayer();
	void sum(vector<double> input);				// sumowanie wag * dane wejściowe
	void correctDelta(double,vector<double>);		// ustawienie błędóœ wstecznych
	void correctWeight(vector<double>);			// poprawianie wag
};

struct ThirdLayer{						// warstwa pierwsza odpowiadająca za warstwę wejściową
	vector<double> weights;					// wagi dla danych wejściowych
	vector<double> output;					// sumy = wagi * odpowiadające im dane wejściowe
	double delta;						// błędy wsteczne, uzależnione od błędów na wyższych warstwach
								
	ThirdLayer();
	double sum(vector<double> input);			// sumowanie wag * dane wejściowe
	void correctDelta(double d);				// ustawienie błędóœ wstecznych
	void correctWeight(vector<double>);			// poprawianie wag
};		

double rastriginFunction(vector<double>);			// funkcja rastrigina
void randWeights(vector<double>&);				// losowanie wag
void randInputs(vector<double>&);				// losowanie danych wejściowych
void loadFromFile(vector<double>&);				// załadowanie danych z pliku
double funkcjaAktywacji(double);				// funkcja aktywacji
double pochodnaFunkcjiAktywacji(double);			// pochodna funkcji aktywacji
/*=========================================================================*/
int main(void){
	vector<double> input;					// dane wejściowe
	vector<double> x;					// dane wzorcowe z pliku
	fstream fout;						// plik w formacie do excela z zapisywaniem wyników

	double y = 0, delta = 0, tmp = 0;		

	for(int i=0;i<N_INPUT;i++) input.push_back(0);		// wyzerowanie wektorów
	for(int i=0;i<2;i++) x.push_back(0);

	FirstLayer *l1 = new FirstLayer();
	SecondLayer *l2 = new SecondLayer();			// tworzenie warstw
	ThirdLayer *l3 = new ThirdLayer();

	randInputs(input);
	loadFromFile(x);					// załadowanie wektoróœ

	fout.open(FILENAME, ios::app);

	l1->sum(input);
	l2->sum(l1->output);					// wyliczanie sumy dla każdej z warstwy na podstawie warstwy poprzedniej
	y = l3->sum(l2->output);

	delta = rastriginFunction(x) - y;
	tmp = delta;						// wyliczanie błędu
	printf("%.20f\n",delta);

	l3->correctDelta(delta);
	l2->correctDelta(l3->delta,l3->weights);		// ustawianie błędu wstecznego
	l1->correctDelta(l2->delta, l2->weights);

	l1->correctWeight(input);
	l2->correctWeight(l1->output);				// poprawianie wag algorytmem wstecznej propagacji błędu
	l3->correctWeight(l2->output);

	l1->sum(input);
	l2->sum(l1->output);					// wyliczanie sumy dla każdej z warstwy na podstawie warstwy poprzedniej
	y = l3->sum(l2->output);

	delta = rastriginFunction(x) - y;
	printf("%.25lf\n",fabs(tmp-delta));			// wyliczanie błędu
	fout << "\nerr0: " << fabs(tmp-delta);
	//tmp = rastriginFunction(l3->output);	

	clock_t st = clock();

	int k = 0;
	do{
		tmp = delta;
		clock_t start = clock();

		l3->correctDelta(delta);
		l2->correctDelta(l3->delta,l3->weights);		// ustawianie błędu wstecznego
		l1->correctDelta(l2->delta, l2->weights);

		l1->correctWeight(input);
		l2->correctWeight(l1->output);				// poprawianie wag algorytmem wstecznej propagacji błędu
		l3->correctWeight(l2->output);	

		l1->sum(input);
		l2->sum(l1->output);					// wyliczanie sumy dla każdej z warstwy na podstawie warstwy poprzedniej
		y = l3->sum(l2->output);

		delta = rastriginFunction(x) - y;
		printf("epoka: %d\terr %.25lf\t",k,fabs(tmp-delta));		// wyliczanie błędu
		fout << "\nerr" << k+1 << ": " << fabs(tmp-delta) << " ";
		//tmp = rastriginFunction(l3->output);	

		printf("czas %lums, czasC %lums\n",clock()-start, clock()-st);
		fout << "czas " << clock()-start << " ms czasCalk " << clock()-st << "ms";
		k++;
	}while(fabs(tmp-delta)>0.00000000000000000001);

	fout.close();

	return 0;
}
/*=========================================================================*/
double rastriginFunction(vector<double> x_vec){
	double result = 0, tmp = 0;
	vector<double>::iterator it = x_vec.begin();

	result = A * x_vec.size();

	for(int i=0;i<x_vec.size();i++, ++it){
		tmp += (pow((*it),2) - A * cos(2*M_PI*(*it)));			// obliczanie funkcji rastrigina
	}
	result += tmp;

	return result;
}

double funkcjaAktywacji(double x){
	return 1/(1+(exp(-0.5*x)));						// funkcja aktywacji
}

double pochodnaFunkcjiAktywacji(double x){
	double tmp;
	tmp = 0.5*exp(-0.5*x);
	return tmp/pow((exp(-0.5*x)+1),2);					// pochodna funkcji aktywacji
}

FirstLayer::FirstLayer(){
	for(int i =0; i<N_INPUT*N_LAYER1; i++){
		weights.push_back(0);
	}
	for(int i =0; i<N_LAYER1; i++){						// wyzerowanie wszystkich wektorow na warstwie pierwszej
		output.push_back(0);
		delta.push_back(0);
	}
	randWeights(weights);
}

SecondLayer::SecondLayer(){
	for(int i =0; i<N_LAYER1*N_LAYER2; i++){
		weights.push_back(0);
	}
	for(int i =0; i<N_LAYER2; i++){						// wyzerowanie wszystkich wektorow na warstwie drugiej
		output.push_back(0);
		delta.push_back(0);
	}
	randWeights(weights);
}

ThirdLayer::ThirdLayer(){
	for(int i =0; i<N_LAYER2*N_LAYER3; i++){
		weights.push_back(0);						// wyzerowanie wszystkich wektorow na warstwie trzeciej
	}
	for(int i =0; i<N_LAYER3; i++){
		output.push_back(0);
	}
	randWeights(weights);
}

void FirstLayer::sum(vector<double> input){	// input w 1 warstwie to dane wejsciowe z przedzialow od -2 do 2
	vector<double>::iterator iti;
	vector<double>::iterator itw;
	vector<double>::iterator ito;

	//fstream file;
	//file.open(FILENAME,ios::app);
	itw = weights.begin();
	for(iti = input.begin();iti!=input.end();++iti)
	{
		for(ito = output.begin();ito!=output.end();++itw, ++ito)
		{
			(*ito) += (*iti)*(*itw);
			//cout << *ito << endl;
			//file << "sum " << (*ito) << " ";
		}
	//file << "\n";

	}

	for(ito=output.begin();ito!=output.end();++ito)
	{
		(*ito) = funkcjaAktywacji(*ito);
		//printf("1warstwa: %lf\n",*ito);
	}
	//file.close();
}

void SecondLayer::sum(vector<double> input){	// input w 2 warstwie to output z warstwy 1
	vector<double>::iterator iti;
	vector<double>::iterator itw;
	vector<double>::iterator ito;

	itw = weights.begin();
	for(iti = input.begin();iti!=input.end();++iti)
	{
		for(ito = output.begin();ito!=output.end();++itw, ++ito)
		{
			(*ito) += (*iti)*(*itw);
			//cout << *ito << endl;
		}
	}
	for(ito=output.begin();ito!=output.end();++ito)
	{
		(*ito) = funkcjaAktywacji(*ito);
		//printf("2warstwa: %lf\n",*ito);
	}
}

double ThirdLayer::sum(vector<double> input){	// input w 3 warstwie to output z warstwy 2
	vector<double>::iterator iti;
	vector<double>::iterator itw;
	vector<double>::iterator ito;

	itw = weights.begin();
	for(iti = input.begin();iti!=input.end();++iti)
	{
		for(ito = output.begin();ito!=output.end();++itw, ++ito)
		{
			(*ito) += (*iti)*(*itw);
			//cout << *ito << endl;
		}
	}
	for(ito=output.begin();ito!=output.end();++ito)
	{
		(*ito) = funkcjaAktywacji(*ito);
		//printf("3warstwa: %lf\n",*ito);
	}
	return output[0];
}

void randWeights(vector<double>& iv){						// losowanie wag
	srand(time(NULL));
	vector<double>::iterator it;

	it = iv.begin();
	for(;it!=iv.end();++it){
		(*it) = (double)rand()/(double)RAND_MAX;
	}
}

void randInputs(vector<double>& i){						// losowanie danych wejściowych
	srand(time(NULL));
	vector<double>::iterator it;
	fstream fout;

	fout.open(FILENAME,ios::app);	
	it = i.begin();
	for(int k=0;it!=i.end();++it,k++){
		double tmp = (double)rand()/(double)RAND_MAX;
		int b = (k%2==0)?-1:1;
		(*it) = tmp * b * 2;
		fout << "input " << k << ": " << (*it) << "\n";
		//cout << *it << endl;
	}
	fout.close();
}

void loadFromFile(vector<double>& vec){						// załadowanie danych z pliku
	fstream file;
	vector<double>::iterator it;
	file.open("input.txt");
	it = vec.begin();
	do
	{
		file >> (*it);
		//cout << *it << endl;
		++it;
	}while(it!=vec.end());
	file.close();
}

void FirstLayer::correctDelta(vector<double> d, vector<double> w){		// poprawianie błędów wstecznych
	vector<double>::iterator itd;
	vector<double>::iterator itw;
	vector<double>::iterator ito;

	itw = w.begin();
	for(ito = delta.begin();ito!=delta.end();++ito)
	{
		for(itd=d.begin();itd!=d.end();++itw,++itd)
		{
			(*ito) += (*itd)*(*itw);
		}
	}
}

void SecondLayer::correctDelta(double d, vector<double> w){			// poprawianie błędów wstecznych
	vector<double>::iterator itd;
	vector<double>::iterator itw;

	itw=w.begin();

	for(itd = delta.begin();itd!=delta.end();++itd,++itw)
	{
		(*itd) = d*(*itw);
	}
}

void ThirdLayer::correctDelta(double d){					// poprawianie błędów wstecznych
	delta = d;
}

void FirstLayer::correctWeight(vector<double> input){
	vector<double>::iterator itw = weights.begin();
	vector<double>::iterator iti = input.begin();
	vector<double>::iterator itd = delta.begin();				// poprawianie wag na podstawie algorytmu wstecznej propagacji błędu
	vector<double>::iterator ito = output.begin();

	int k = 0;
	for(itw=weights.begin(),ito=output.begin(),itd=delta.begin();k<N_LAYER1;++itw,k++, ++itd,++ito);
	{
		for(iti = input.begin();iti!=input.end();++iti)
		{
			//cout <<"itw"<< *itw << endl;
			(*itw)+=	LEARNING_RATE
				*(*itd)
				*pochodnaFunkcjiAktywacji(*ito)
				*(*iti);

			for(int z=0;z<N_INPUT;z++){++itw;}
		}
		

		for(int z=0;z<N_INPUT;z++){--itw;}
	}
}

void SecondLayer::correctWeight(vector<double> input){
	vector<double>::iterator itw = weights.begin();
	vector<double>::iterator iti = input.begin();				// poprawianie wag na podstawie algorytmu wstecznej propagacji błędu
	vector<double>::iterator itd = delta.begin();
	vector<double>::iterator ito = output.begin();

	int k = 0;
	for(itw=weights.begin(),ito=output.begin(),itd=delta.begin();k<N_LAYER2;++itw,k++, ++itd,++ito);
	{
		for(iti = input.begin();iti!=input.end();++iti)
		{
			(*itw)+=	LEARNING_RATE
				*(*itd)
				*pochodnaFunkcjiAktywacji(*ito)
				*(*iti);

			for(int z=0;z<N_LAYER1;z++){++itw;}
		}
		

		for(int z=0;z<N_LAYER1;z++){--itw;}
	}
}

void ThirdLayer::correctWeight(vector<double> input){
	vector<double>::iterator itw = weights.begin();
	vector<double>::iterator iti = input.begin();
	double itd = delta;
	vector<double>::iterator ito = output.begin();				// poprawianie wag na podstawie algorytmu wstecznej propagacji błędu

	int k = 0;
	for(itw=weights.begin(),ito=output.begin();k<N_LAYER3;++itw,k++,++ito);
	{
		for(iti = input.begin(); iti!=input.end();++iti)
		{
			(*itw)+=	LEARNING_RATE
				*itd
				*pochodnaFunkcjiAktywacji(*ito)
				*(*iti);

			for(int z=0;z<N_LAYER2;z++){++itw;}
		}
		

		for(int z=0;z<N_LAYER2;z++){--itw;}
	}
}














