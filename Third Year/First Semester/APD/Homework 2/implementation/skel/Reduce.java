import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.concurrent.RecursiveTask;

public class Reduce extends RecursiveTask<ReduceResult> {
    private final String filename;
    private final List<MapResult> mapResults;

    public Reduce(String filename, List<MapResult> mapResults) {
        this.filename = filename;
        this.mapResults = mapResults;
    }

    private ReduceCombine combineResults() {
        List<java.util.Map<Integer, Integer>> dictionary = new ArrayList<>();
        List<List<String>> maxWords = new ArrayList<>();
        for (MapResult mapResult : mapResults) {
            if (mapResult.getFileName() == filename) {
                dictionary.add(mapResult.getDictionary());
                maxWords.add(mapResult.getMaxWords());
            }
        }
        return new ReduceCombine(filename, dictionary, maxWords);
    }


    public static int fib(int n)
    {
        if (n <= 1) {
            return n;
        }

        return fib(n - 1) + fib(n - 2);
    }

    @Override
    protected ReduceResult compute() {
        ReduceCombine reduceResult = combineResults();
        Float rang = 0.f;
        int wordsNumber = 0;
        int maxLength = 0;
        int wordsLg = 0;
        for (Map<Integer, Integer> dict : reduceResult.getDictionary()) {
            for (Map.Entry<Integer, Integer> entry : dict.entrySet()) {
                if (maxLength < entry.getKey()) {
                    maxLength = entry.getKey();
                    wordsLg = entry.getValue();
                } else if (maxLength == entry.getKey()){
                    wordsLg += entry.getValue();
                }

                rang += fib(entry.getKey() + 1) * entry.getValue();
                wordsNumber += entry.getValue();
            }
        }
        rang = rang / wordsNumber;
        return new ReduceResult(filename.substring(12), rang, maxLength, wordsLg);
    }
}
