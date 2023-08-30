import java.io.*;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.concurrent.RecursiveTask;
import java.util.stream.Collectors;

public class Map extends RecursiveTask<MapResult> {
    private final String fileName;
    private final long fileLength;
    private final int offset;
    private final int fragDim;
    private final boolean needSkip;
    private final int toSkip;

    public Map(String fileName, long fileLength, int offset, int fragDim, boolean needSkip, int toSkip) {
        this.fileName = fileName;
        this.fileLength = fileLength;
        this.offset = offset;
        this.fragDim = fragDim;
        this.needSkip = needSkip;
        this.toSkip = toSkip;
    }

    private RandomAccessFile openRaf() {
        RandomAccessFile raf = null;
        try {
            raf = new RandomAccessFile(fileName, "r");
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
        return raf;
    }

    private String getString(int length, RandomAccessFile raf) throws IOException {
        byte[] readTxt = new byte[length];
        raf.seek(offset);
        raf.read(readTxt);
        String neededTxt = new String(readTxt);
        if (needSkip) {
            neededTxt = neededTxt.substring(toSkip);
        }
        return neededTxt;
    }

    private MapResult getResult(String neededTxt) {
        String[] splitWords = neededTxt.split("\\W+");
        java.util.Map<Integer, Integer> dict = new HashMap<>();
        int maxLength = 0;
        for (String word : splitWords) {
            int wordLength = word.length();
            if (wordLength == 0) {
                continue;
            }
            if (wordLength > maxLength) {
                maxLength = wordLength;
            }
            if(dict.containsKey(wordLength)) {
                dict.put(wordLength, dict.get(wordLength) + 1);
            } else {
                dict.put(wordLength, 1);
            }
        }
        List<String> splitW = Arrays.asList(splitWords.clone());
        int finalMaxLength = maxLength;
        List<String> maxW = splitW.stream().filter(w -> w.length() == finalMaxLength).collect(Collectors.toList());
        MapResult result = new MapResult(fileName, dict, maxW);
        return result;
    }

    @Override
    protected MapResult compute() {
        RandomAccessFile raf = openRaf();
        String neededTxt = null;
        MapResult result = null;
        if (offset + fragDim >= fileLength) {
            try {
                neededTxt = getString((int) fileLength - offset, raf);
            } catch (IOException e) {
                e.printStackTrace();
            }
            result = getResult(neededTxt);
            return result;
        } else {
            try {
                neededTxt = getString(fragDim, raf);
            } catch (IOException e) {
                e.printStackTrace();
            }
            int count = 0;
            boolean stop = false;
            boolean skip = false;
            if (Character.isLetterOrDigit(neededTxt.charAt(neededTxt.length() - 1))) {
                while (!stop && offset + fragDim + count < fileLength) {
                    char charToAdd = 0;
                    try {
                        charToAdd = (char) raf.read();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    if (Character.isLetterOrDigit(charToAdd)) {
                        neededTxt += charToAdd;
                        count++;
                        skip = true;
                    } else {
                        stop = true;
                    }
                }
            }
            result = getResult(neededTxt);
            Map t = new Map(fileName, fileLength, offset + fragDim, fragDim, skip, count);
            t.fork();
            Tema2.mapResults.add(t.join());
        }
        return result;
    }
}
