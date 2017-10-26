import java.io.*;
import java.util.Random;
import java.util.logging.Level;
import java.util.logging.Logger;

import java.io.BufferedReader;


public class Neuron {

    int numberOfWeights;
    double answer;
    double[] weight;
    double[] input;
    double suma;
    double error;

    public Neuron(){
        Random rnd = new Random();
        this.weight = new double[35];
        this.input = new double[10];
        for(int i=0; i<35; i++){
            this.weight[i] = rnd.nextDouble();
        }
    }
    public void getSuma(int j){
        this.suma=0;
        for(int i=0; i<35; i++){
            this.suma += Data.records[j][i] * this.weight[i];
        }
    }
    public void getSuma2(){
        this.suma=0;
        for(int i=0; i<10; i++){
            this.suma += this.input[i] * this.weight[i];
        }
    }
    //        public void getAnswer(int j){
//           this.getSuma(j);
//           this.answer=(1/( 1 + Math.pow(Math.E,(-1*this.suma))));
//        }
    public void getAnswer(int j){
        this.getSuma(j);
        if(this.suma<0)
            this.answer=0;
        else
            this.answer=this.suma;
    }
    public void getAnswer2(){
        this.getSuma2();
        if(this.suma<0)
            this.answer=0;
        else
            this.answer=this.suma;
    }
    public void hebb(int j){
        this.getAnswer(j);
        for(int i=0; i<35; i++){
            this.weight[i] +=  0.000000025  * Data.records[j][i] * this.answer;
        }
        this.getAnswer(j);
    }
    public void hebb2(){
        this.getAnswer2();
        for(int i=0; i<10; i++){
            this.weight[i] += 0.000000025 * this.input[i] * this.answer;
        }
    }
}






