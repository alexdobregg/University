import java.util.List;
import java.util.Map;

public class ReduceCombine {
    private String filename;
    private List<java.util.Map<Integer, Integer>> dictionary;
    private List<List<String>> maxWords;

    public ReduceCombine(String filename, List<Map<Integer, Integer>> dictionary, List<List<String>> maxWords) {
        this.filename = filename;
        this.dictionary = dictionary;
        this.maxWords = maxWords;
    }

    public String getFilename() {
        return filename;
    }

    public List<Map<Integer, Integer>> getDictionary() {
        return dictionary;
    }

    public List<List<String>> getMaxWords() {
        return maxWords;
    }
}
