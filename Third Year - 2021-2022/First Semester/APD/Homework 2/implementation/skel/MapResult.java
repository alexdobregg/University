import java.util.List;
import java.util.Map;


public class MapResult {
    private String fileName;
    private Map<Integer, Integer> dictionary;
    private List<String> maxWords;

    public MapResult(String fileName, Map<Integer, Integer> dictionary, List<String> maxWords) {
        this.fileName = fileName;
        this.dictionary = dictionary;
        this.maxWords = maxWords;
    }

    public String getFileName() {
        return fileName;
    }

    public Map<Integer, Integer> getDictionary() {
        return dictionary;
    }

    public List<String> getMaxWords() {
        return maxWords;
    }
}
