package global;

import utils.Means;
import utils.Tree;
import utils.Utils;

import java.io.*;
import java.nio.charset.StandardCharsets;
import java.util.*;
import java.util.concurrent.Semaphore;

public class Main {
    public static void main(String[] args) throws IOException {
        // Obtaining the list of files from the data folder and randomizing it for different outputs.
        List<File> filesList = new ArrayList<>
                (Arrays.stream(Objects.requireNonNull(new File("src/global/MPS-Global").listFiles())).toList());
        Collections.shuffle(filesList);

        List<String> operations = Arrays.asList("CubicMean", "SquareMean", "ArithmeticMean",
                                                "GeometricMean", "HarmonicMean", "Minimum", "Maximum");

        List<Float> scores = new ArrayList<>();
        List<Tree> bestTrees = new ArrayList<>();

        // Using thread parallelization for faster reading of files and computing of results
        int numberOfThreads = 16;
        Thread[] threads = new Thread[numberOfThreads];
        Semaphore semaphore = new Semaphore(1);

        float scorePerSet;
        System.out.println("The scores for the training set are:");
        // Choosing the best 5 trees with a score higher than 86 from the first 70% of files.
        while (bestTrees.size() < 5) {
            Tree tree = new Tree();
            tree.ourShuffle(operations.size(), operations.size() * 2 + 1);
            tree.ourShuffle(operations.size(), operations.size() + 1);
            tree.ourShuffle(operations.size(), (operations.size() + 1) / 2);
            tree.ourShuffle(operations.size(), (operations.size() + 1) / 4);

            ThreadUtils.runner(filesList.subList(0, filesList.size() * 70 / 100), numberOfThreads, threads,
                                operations, tree, scores, semaphore);

            scorePerSet = Means.mean_score(scores);
            System.out.println(scorePerSet);
            if (scorePerSet > 86) {
                System.out.println("Found a score higher than 86: " + scorePerSet);
                bestTrees.add(tree);
            }
            scores.clear();
        }
        /* Keeping the best tree out of the five after being used on the validation set, based on the score obtained
        on the next 25% of the files. */
        float max = 0;
        int idx = 0;
        System.out.println("The scores for the validation set are:");
        for (Tree tre : bestTrees) {
            ThreadUtils.runner(filesList.subList(filesList.size() * 70 / 100, filesList.size() * 95 / 100),
                                numberOfThreads, threads, operations, tre, scores, semaphore);
            scorePerSet = Means.mean_score(scores);
            if (scorePerSet > max) {
                max = scorePerSet;
                idx = bestTrees.indexOf(tre);
            }
            System.out.println(scorePerSet);
        }
        scores.clear();
        System.out.println("The best score was obtained by tree number " + (idx + 1) + " and it was " + max + ".");
        // Testing the best tree on the last 5% of the files;
        ThreadUtils.runner(filesList.subList(filesList.size() * 95 / 100, filesList.size()), numberOfThreads,
                            threads, operations, bestTrees.get(idx), scores, semaphore);
        scorePerSet = Means.mean_score(scores);
        System.out.println("The score obtained on the test set is " + scorePerSet + ".");

        // Writing the best tree to the Global Binarization Output file.
        PrintWriter writer = new PrintWriter("GlobalBinarization.txt", StandardCharsets.UTF_8);
        writer.println("The indexes of the thresholds and operations, starting from leaves to root, are:\n");
        int maxWidth = operations.size();
        int depth = 0;
        while (maxWidth >= 1) {
            depth = Utils.getDepthAndPrint(operations, bestTrees, idx, writer, maxWidth, depth);
            if (maxWidth == operations.size()) {
                maxWidth = (maxWidth + 1) / 2;
            } else {
                maxWidth /= 2;
            }
        }
        writer.close();
    }
}