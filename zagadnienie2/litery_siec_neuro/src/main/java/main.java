import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;





public class main{

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) throws FileNotFoundException {

        Data.loadRecords(35, 24, "src\\main\\resources\\dane.txt");
        Layer l1 = new Layer(10);
        Neuron n1 = new Neuron();
        //nauka
        for (int e = 0; e < 10000; e++) {

            for (int i = 0; i < 10; i++) {
                for (int j = 0; j < 20; j++) {
                    l1.neurons[i].hebb(j);
                }
            }
            for (int i = 0; i < 10; i++) {
                n1.input[i] = l1.neurons[i].answer;
            }
            for (int i = 0; i < 10; i++) {
                n1.hebb2();
            }
        }
        List<Double> list = new ArrayList<Double>();
        //kalibracja
        for (int i = 10; i < 20; i++) {
            for (int j = 0; j < 10; j++) {
                l1.neurons[j].getAnswer(i);
                n1.input[j] = l1.neurons[j].answer;
            }
            n1.getAnswer2();
            list.add(n1.suma);
        }
        Collections.sort(list);
        System.out.println(list.get(0));
        double classifier;
        classifier = list.get(0);

        //test
        int dc=0;
        int mc=0;
        for (int t = 0; t < 24; t++) {
            for (int i = 0; i < 10; i++) {
                l1.neurons[i].getAnswer(t);
            }
            for (int i = 0; i < 10; i++) {
                n1.input[i] = l1.neurons[i].answer;
            }
            n1.getAnswer2();
            System.out.println(n1.answer);
            if(n1.answer<classifier){
                System.out.println("Mala");
                mc++;
            }


            else{
                System.out.println("duza");
                dc++;
            }

        }
        System.out.println("Wykryte male litery "+mc);
        System.out.println("Wykryte duze litery "+dc);
    }
}