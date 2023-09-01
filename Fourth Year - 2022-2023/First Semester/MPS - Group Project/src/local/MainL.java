package local;

import utils.Utils;
import utils.Tree;
import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import java.nio.charset.StandardCharsets;
import java.util.*;
import java.util.concurrent.Semaphore;

public class MainL {

    public static void main(String[] args) throws IOException {
        // Obtaining the list of files from the data folder and randomizing it for different outputs.
        List<File> filesList = new ArrayList<>
                (Arrays.stream(Objects.requireNonNull(new File("src/local/MPS-Local").listFiles())).toList());
        Collections.shuffle(filesList);

        List<String> operations = Arrays.asList("CubicMean", "SquareMean", "ArithmeticMean",
                "GeometricMean", "HarmonicMean");

        List<Tree> bestTrees = new ArrayList<>();
        float scorePerSet;

        // Using thread parallelization for faster reading of files and computing of results
        int numberOfThreads = 16;
        Thread[] threads = new Thread[numberOfThreads];
        Semaphore semaphore = new Semaphore(1);

        List<List<List<Float>>> filePixels = new ArrayList<>();
        List<List<List<Float>>> computedWindows = new ArrayList<>();
        List<List<Float>> thresholds = new ArrayList<>();

        // Reading the pixels from the first 70% of the files and computing their windows.
        ThreadUtils.runnerRead(filesList.subList(0, filesList.size() * 70 / 100), numberOfThreads, threads, filePixels,
                               semaphore);
        Utils.compWindows(filePixels, computedWindows, numberOfThreads, threads, semaphore);

        System.out.println("The scores for the training set are:");
        // Choosing the best 5 trees with a score higher than 54 from the first 70% of files.
        while (bestTrees.size() < 5) {
            thresholds.clear();
            Tree tree = new Tree();
            tree.ourShuffle(operations.size(), operations.size() * 2);
            tree.ourShuffle(operations.size(), operations.size());
            tree.ourShuffle(operations.size(), operations.size() - 1);
            tree.ourShuffle(operations.size(), (operations.size() - 1) / 2);
            ThreadUtils.runnerThresholds(computedWindows, numberOfThreads, threads, operations, tree, thresholds, semaphore);
            scorePerSet = Utils.getScore(filePixels, numberOfThreads, threads, thresholds, semaphore);
            System.out.println(scorePerSet * 100);
            if (scorePerSet * 100 > 54) {
                System.out.println("Found a score higher than 54: " + scorePerSet * 100);
                bestTrees.add(tree);
            }
        }

        float max = 0;
        int idx = 0;
        System.out.println("The scores for the validation set are:");

        /* Keeping the best tree out of the five after being used on the validation set, based on the score obtained
        on the next 25% of the files. */
        filePixels.clear();
        computedWindows.clear();
        ThreadUtils.runnerRead(filesList.subList(filesList.size() * 70 / 100, filesList.size() * 95 / 100),
                                                    numberOfThreads, threads, filePixels, semaphore);
        Utils.compWindows(filePixels, computedWindows, numberOfThreads, threads, semaphore);

        for (Tree tre : bestTrees) {
            thresholds.clear();
            ThreadUtils.runnerThresholds(computedWindows, numberOfThreads, threads, operations, tre, thresholds, semaphore);
            scorePerSet = Utils.getScore(filePixels, numberOfThreads, threads, thresholds, semaphore);
            if (scorePerSet > max) {
                max = scorePerSet;
                idx = bestTrees.indexOf(tre);
            }
            System.out.println(scorePerSet * 100);
        }
        System.out.println("The best score was obtained by tree number " + (idx + 1) + " and it was " + max * 100 + ".");
        // Testing the best tree on the last 5% of the files;
        filePixels.clear();
        computedWindows.clear();
        ThreadUtils.runnerRead(filesList.subList(filesList.size() * 95 / 100, filesList.size()), numberOfThreads,
                                threads, filePixels, semaphore);
        Utils.compWindows(filePixels, computedWindows, numberOfThreads, threads, semaphore);

        thresholds.clear();
        ThreadUtils.runnerThresholds(computedWindows, numberOfThreads, threads, operations, bestTrees.get(idx), thresholds, semaphore);
        scorePerSet = Utils.getScore(filePixels, numberOfThreads, threads, thresholds, semaphore);
        System.out.println("The score obtained on the test set is " + scorePerSet * 100 + ".");

        // Writing the best tree to the Local Binarization Output file.
        PrintWriter writer = new PrintWriter("LocalBinarization.txt", StandardCharsets.UTF_8);
        writer.println("The indexes of the thresholds and operations, starting from leaves to root, are:\n");
        int maxWidth = operations.size();
        int depth = 0;
        boolean second = false;
        while (maxWidth >= 1) {
            depth = Utils.getDepthAndPrint(operations, bestTrees, idx, writer, maxWidth, depth);
            if (maxWidth == operations.size()) {
                maxWidth = 1;
                second = true;
            } else if (maxWidth == 1 && second){
                maxWidth *= 2;
                second = false;
            } else {
                maxWidth /= 2;
            }
        }
        writer.close();
    }
}
