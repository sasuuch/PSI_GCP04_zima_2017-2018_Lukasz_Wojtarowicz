#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<string>
#include<Eigen/Dense>
#include<fstream>
#include<iostream>

#define SIZE 15*15
#define N 10
#define norma sqrt(225)

#define FILENAME "test.xlsx"

using namespace std;
using namespace Eigen;

MatrixXf vec_emoticons(225, 4); 				/* dane uczace: 0,x - smiech, 1,x - calus, 2,x - milosc, 3,x - placz */
MatrixXf vec_normalized(225, 4);				/* dane uczace znormalizowane: vec_emoticons/sqrt(255) */

MatrixXf input(225, 1);					/* dane testowe */
MatrixXf inputSmiech80pr(225, 1);
MatrixXf inputCalus60pr(225, 1);
MatrixXf inputMilosc80pr(225, 1);
MatrixXf inputPlacz60pr(225, 1);
MatrixXf w(225, 4);					/* wagi */
VectorXf a(255);					/* suma: wagi * dane uczace */
VectorXf atest(255);					/* wyniki dla danych testuj¹cych */
VectorXf deltaw(225);					/* poprawka wag */

void zeruj();
void generateWeights();
void generateInput();
void setLearnVectors();
int fa(double);
void test();
void porownaj();

int
main(){

	int i, j;
	int epoka = 0;					// liczba epok 

	const double LEARNING_RATE = 0.5;		/* wspolczynnik uczenia */
	const double FORGET_RATE = 0.05;		/* wspolczynnik zapominania */

	double globalError = 0.;			/* bledy */
	double localError = 0.;			
	double MSE = 0.;				
	double MAPE = 0.;				

	double pom = 0.;				/* zmienne pomocnicze */
	double pomt = 0.;

	zeruj();					/* ustawienie wartosci domyslnych dla wektora a  */

	//UCZENIE WG HEBBA	
	generateWeights();				/* wygenerowanie wag */
	setLearnVectors();				/* ustawienie wektorow uczacych */

	do{
		cout << "epoka: " << epoka << endl;

		/* BEZ WSPOLCZYNNIKA ZAPOMINANIA
		for (i = 0; i<4; ++i){
			globalError = 0.;
			for (j = 0; j<SIZE; ++j){
				pom = a(j);
				a(j) = (w(j, i)*vec_emoticons(j, i));
				w(j, i) = w(j, i) + LEARNING_RATE*a(j)*vec_emoticons(j, i);

				if (localError == abs(pom - a(j))) break;
				localError = abs(pom - a(j));
				globalError = globalError + pow(localError, 2);

			}

			MSE = pow(globalError, 2) / SIZE;
			MAPE = (globalError * 10) / SIZE;
			cout << " MSE: " << MSE << "\tMAPE: " << MAPE << "%\n";
		}*/

		// ZE WSPOLCZYNNIKIEM ZAPOMINANIA
		for(i=0;i<4;++i){
		globalError = 0.;
		for(j=0;j<SIZE;++j){
		pom = a(j);
		a(j) = (w(j,i)*vec_emoticons(j,i));
		w(j,i) = w(j,i)*FORGET_RATE + LEARNING_RATE*a(j)*vec_emoticons(j,i);

		if(localError==abs(pom-a(j))) break;
		localError = abs(pom - a(j));
		globalError = globalError + pow(localError,2);
		}

		MSE = pow(globalError,2)/(SIZE);
		MAPE = (globalError*10/SIZE);
		cout << "i:" << i << " MSE: " << MSE << "\tMAPE: " << MAPE << "%\n";
		}

		// REGU£A OJI
		/*for(i=0;i<4;++i){
		globalError = 0.;
		for(j=0;j<SIZE;++j){
		pom = a(j);
		a(j) = (w(j,i)*vec_emoticons(j,i));
		w(j,i) = w(j,i) + (LEARNING_RATE*a(j)*(vec_emoticons(j,i)-a(j)*w(j,i)));

		if(localError==abs(pom-a(j))) break;
		localError = abs(pom - a(j));
		globalError = globalError + pow(localError,2);
		}

		MSE = pow(globalError,2)/(SIZE);
		MAPE = (globalError*10/SIZE);
		cout << "i:" << i << " MSE: " << MSE << "\tMAPE: " << MAPE << "%\n";
		}*/

		epoka++;

	} while (globalError != 0 && epoka<1000);

	cout << "\nliczba epok: " << epoka << endl;


	test();

	return 0;
}

void
generateWeights(){
	srand(time(NULL));
	int i;
	for (i = 0; i<SIZE; i++){
		w(i, 0) = (float)rand() / (float)RAND_MAX;				// losowanie wag
		w(i, 1) = (float)rand() / (float)RAND_MAX;				// losowanie wag
		w(i, 2) = (float)rand() / (float)RAND_MAX;				// losowanie wag
		w(i, 3) = (float)rand() / (float)RAND_MAX;				// losowanie wag
	}
}

void
generateInput(){
	srand(time(NULL));
	int i, j, tmp;
	for (i = 0; i<SIZE; i++){
		tmp = rand() % 2 - 1;						// generowanie losowych danych uczacych
		if (tmp == 0) input(i) = 1;
		else input(i) = -1;
	}

	for (i = 0; i<SIZE; i++){							// generowanie zdeformowanych emotikon
		inputSmiech80pr(i) = vec_emoticons(i, 0);
		inputCalus60pr(i) = vec_emoticons(i, 1);
		inputMilosc80pr(i) = vec_emoticons(i, 2);
		inputPlacz60pr(i) = vec_emoticons(i, 3);
	}

	for (i = 0; i<45; i++){
		j = rand() % 225;
		inputMilosc80pr(j) = !inputMilosc80pr(j);
		j = rand() % 225;
		inputSmiech80pr(j) = !inputSmiech80pr(j);
	}

	for (i = 0; i<90; i++){
		j = rand() % 225;
		inputCalus60pr(j) = !inputCalus60pr(j);
		j = rand() % 225;
		inputPlacz60pr(j) = !inputPlacz60pr(j);
	}

}

void
setLearnVectors(){
	int d, j, tmp;
	fstream fd;
	fd.open("dane.txt");
	if (fd.is_open()){
		j = 0, d = 0;
		while (!fd.eof()){
			if (j == 225){
				j = 0; d++;
			}
			if (d == 4) break;

			fd >> tmp;
			vec_emoticons(j, d) = tmp;
			vec_normalized(j, d) = vec_emoticons(j, d) / norma;

			++j;
		}
		fd.close();
	}
}

void
zeruj(){
	int i = 0;
	for (i = 0; i<SIZE; i++){
		a(i) = 1.;
	}
}


void test(){

	fstream fout;						// plik excela 
	fout.open(FILENAME, ios::app);

	double globalError = 0.;
	double localError = 0.;
	double MSE = 0.;
	double MAPE = 0.;
	double pom = 0.;
	int epoka = 0;
	int i, j;

	generateInput();

	do{
		epoka++;
		for (j = 0; j<SIZE; ++j){
			pom = a(j);
			a(j) = (w(j, 0)*inputPlacz60pr(j, 0));
			//cout << w(j,0) << " " <<  input(j,0);

			atest(j) = fa(a(j));
			localError = abs(pom - a(j));
			globalError = globalError + pow(localError, 2);
		}

		MSE = pow(globalError, 2) / (SIZE);
		MAPE = (globalError * 10 / SIZE);
		fout << "MSE: " << MSE << "\tMAPE: " << MAPE << "%\n";
	} while (globalError != 0 && epoka<1);

	fout.close();

	porownaj();
}

int fa(double a){
	return (a >= 0) ? 1 : -1;
}

void porownaj(){
	fstream fout;						// plik excela
	fout.open(FILENAME, ios::app);
	int i, j, tmp[4], result;

	for (i = 0; i<4; i++) tmp[i] = 0;

	for (i = 0; i<SIZE; ++i){
		if (atest(i) == vec_emoticons(i, 0)) tmp[0]++;
		if (atest(i) == vec_emoticons(i, 1)) tmp[1]++;
		if (atest(i) == vec_emoticons(i, 2)) tmp[2]++;
		if (atest(i) == vec_emoticons(i, 3)) tmp[3]++;
	}

	cout << "\nsmiech: " << (double(tmp[0]) / 225.)*100. << endl;
	cout << "calus: " << (double(tmp[1]) / 225.)*100. << endl;
	cout << "milosc: " << (double(tmp[2]) / 225.)*100. << endl;
	cout << "placz: " << (double(tmp[3]) / 225.)*100. << endl;


	fout.close();
	system("pause");
}








