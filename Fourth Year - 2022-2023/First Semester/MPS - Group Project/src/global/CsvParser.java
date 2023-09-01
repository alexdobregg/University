package global;

import java.io.* ;
import java.util.*;

public class CsvParser {
    /**
     * Method to obtain the threshold and F measures from the file
     * @param file the parsed file
     * @return a list composed of thresholds and fMeasures
     */
    public static List<List<Float>> parse(File file) throws Exception {
        Scanner sc = new Scanner(file);
        List<Float> thresholds = new ArrayList<>(Arrays.stream(sc.next().split(",")).map(Float::valueOf).toList());
        List<Float> fMeasures = new ArrayList<>(Arrays.stream(sc.next().split(",")).map(Float::valueOf).toList());
        sc.close();
        return Arrays.asList(thresholds, fMeasures);
    }
}