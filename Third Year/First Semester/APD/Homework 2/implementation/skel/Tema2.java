import java.io.*;
import java.util.*;
import java.util.concurrent.ForkJoinPool;

public class Tema2 {
    public static int fragDim = 0;
    public static int docNum = 0;
    public static List<String> filesName;
    public static List<MapResult> mapResults;
    public static List<ReduceResult> reduceResults;

    public static void main(String[] args) {
        if (args.length < 3) {
            System.err.println("Usage: Tema2 <workers> <in_file> <out_file>");
            return;
        }
        int nrWorkers = Integer.parseInt(args[0]);
        String testInFile = args[1];
        String outFile = args[2];
        filesName = new ArrayList<>();
        mapResults = new ArrayList<>();
        try {
            Scanner myReader = new Scanner(new File(testInFile));
            fragDim = Integer.parseInt(myReader.nextLine());
            docNum = Integer.parseInt(myReader.nextLine());
            while (myReader.hasNextLine()) {
                filesName.add(myReader.nextLine());
            }
        } catch (FileNotFoundException e) {
            System.out.println("An error occurred");
            e.printStackTrace();
        }

        ForkJoinPool fjp = new ForkJoinPool(nrWorkers);
        for (int i = 0; i < docNum; i++) {
            mapResults.add(fjp.invoke(new Map(filesName.get(i),
                    (new File(filesName.get(i))).length(), 0, fragDim, false, 0)));
        }
        reduceResults = new ArrayList<>();
        for (int i = 0; i < docNum; i++) {
            reduceResults.add(fjp.invoke(new Reduce(filesName.get(i), mapResults)));
        }

        Collections.sort(reduceResults, new Comparator<ReduceResult>() {
            @Override
            public int compare(ReduceResult r1, ReduceResult r2) {
                if (r1.getRang() == r2.getRang()) {
                    return r1.getFilename().compareTo(r2.getFilename());
                }
                return r2.getRang().compareTo(r1.getRang());
            }
        });

        FileWriter myWriter = null;
        try {
            myWriter = new FileWriter(outFile);
        } catch (IOException e) {
            e.printStackTrace();
        }

        for (ReduceResult result : reduceResults) {
            String rangStr = String.format("%.2f", result.getRang());
            try {
                myWriter.write(result.getFilename() + "," + rangStr + "," +
                                result.getMaxLength() + "," + result.getWordsLg() + "\n");
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        try {
            myWriter.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
