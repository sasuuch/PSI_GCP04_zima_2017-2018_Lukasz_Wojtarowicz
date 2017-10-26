import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;
import java.util.logging.Level;
import java.util.logging.Logger;

public class Data {


    public static double[][] records;
    static String[] results;

    public static void loadRecords(int n, int m, String path) throws FileNotFoundException {


        records = new double[m][n];
        results = new String[m];


        File fileDir = new File(path);

        BufferedReader bfr=null;
        try {
            bfr = new BufferedReader(
                    new InputStreamReader(
                            new FileInputStream(fileDir), "Cp1250"));
        } catch (UnsupportedEncodingException ex) {
            Logger.getLogger(Data.class.getName()).log(Level.SEVERE, null, ex);
        }

        for (int i = 0; i < m; i++) {
            try {
                String parts[] = bfr.readLine().split(",");
                for (int j = 0; j < n; j++) {
                    records[i][j] = Double.parseDouble(parts[j]);
                }
                results[i] = parts[n];
            } catch (IOException ex) {
                Logger.getLogger(Neuron.class.getName()).log(Level.SEVERE, null, ex);
            }

        }


    }


}