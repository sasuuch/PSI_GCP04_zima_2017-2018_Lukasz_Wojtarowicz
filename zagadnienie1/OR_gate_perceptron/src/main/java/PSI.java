import java.io.FileNotFoundException;

/**
 * Created by Łukasz on 2017-10-18.
*/



//Main class

public class PSI{

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) throws FileNotFoundException {



        Data.loadRecords(2, 4, "src\\main\\resources\\data");
        Neuron n1 = new Neuron();
        for(int j=0; j<4; j++){
            n1.getSuma(j);
            n1.getAnswer();
            n1.delta(j);
        }

        for(int i=0; i<2; i++){
            n1.getManualData();
            n1.getManualSuma();
            n1.getAnswer();
            System.out.println("Odpowiedz bramki OR="+n1.answer);
        }


    }

}