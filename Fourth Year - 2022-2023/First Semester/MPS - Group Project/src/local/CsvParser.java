package local;

import java.io.File;
import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class CsvParser {
    /**
     * Method to obtain the pixels for the given file.
     * @param file the parsed file
     * @return a list composed of lists of pixels and their information
     */
    public static List<List<Float>> parse(File file) throws Exception {
        List<List<Float>> pixels = new ArrayList<>();
        Path filePath = file.toPath();
        Charset charset = Charset.defaultCharset();
        List<String> stringList = Files.readAllLines(filePath, charset);
        for (String line : stringList) {
            List<Float> pixel = new ArrayList<>(Arrays.stream(line.split(",")).map(Float::valueOf).toList());
            pixels.add(pixel);
        }
        return pixels;
    }
}
