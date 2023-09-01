public class ReduceResult {
    private String filename;
    private Float rang;
    private int maxLength;
    private int wordsLg;

    public ReduceResult(String filename, Float rang, int maxLength, int wordsLg) {
        this.filename = filename;
        this.rang = rang;
        this.maxLength = maxLength;
        this.wordsLg = wordsLg;
    }

    public String getFilename() {
        return filename;
    }

    public Float getRang() {
        return rang;
    }

    public int getMaxLength() {
        return maxLength;
    }

    public int getWordsLg() {
        return wordsLg;
    }
}
