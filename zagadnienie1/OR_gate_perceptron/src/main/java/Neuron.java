import java.util.Random;
import java.util.Scanner;

public class Neuron {

   int answer;
    double[] weight;
    double[] input;
    double suma;
    double error;

    public Neuron(){
        Random rnd = new Random();
        this.weight = new double[2];
        this.input = new double[2];
        for(int i=0; i<2; i++){
            this.weight[i] = rnd.nextDouble();
        }

    }

    public void getSuma(int j){
        this.suma=0;
        for(int i=0; i<2; i++){
            this.suma += Data.records[j][i] * this.weight[i];
        }
    }

    public void getAnswer(){
        if(this.suma>0){
            this.answer=1;
        }
        else if(this.suma<=0){
            this.answer=0;
        }
    }

    public void delta(int j){
        System.out.println("Wartosc oczekiwana = "+Data.results[j]);
        System.out.println("Wartosc obliczona przed modyfikacja wag = "+this.answer);
        this.error=Data.results[j]-this.answer;
        System.out.println("Error = "+this.error);
        for(int i=0; i<2; i++){

            System.out.println("Waga przed modyfikacja = "+this.weight[i]);
            this.weight[i]+=0.001*this.error*Data.records[j][i];
            System.out.println("Waga po modyfikacji = "+this.weight[i]);
        }

        this.getSuma(j);
        this.getAnswer();
    }

    public void getManualData(){
        Scanner read = new Scanner(System.in);
        System.out.println("Wprowadz wartosci testujace <0; 1>");
        for(int i=0; i<2; i++){
            this.input[i] = read.nextDouble();
        }
    }
    public void getManualSuma(){
        this.suma=0;
        for(int i=0; i<2; i++){
            this.suma += this.input[i] * this.weight[i];
        }
    }
}
