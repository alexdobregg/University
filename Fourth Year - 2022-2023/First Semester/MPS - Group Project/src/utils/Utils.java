package utils;

import local.ThreadUtils;

import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.concurrent.Semaphore;

public class Utils {
    /**
     * Method to compute the range of each thread
     * @param size total size
     * @param numberOfThreads the number of threads
     * @param thread the current thread
     * @return the range that the current thread is handling
     */
    public static List<Integer> getThreadRange(int size, int numberOfThreads, int thread) {
        int start = (int) (thread * (double) size / numberOfThreads);
        int end = (int) Math.min((thread + 1) * (double) size / numberOfThreads, size);
        return Arrays.asList(start, end);
    }

    /**
     * Method to compute the windows; based on the number of threads
     * @param filePixels the list containing information about every pixel from all the files
     * @param computedWindows the one that will store the result of this method
     * @param numberOfThreads the number of threads
     * @param threads the threads
     * @param semaphore the semaphore
     */
    public static void compWindows(List<List<List<Float>>> filePixels, List<List<List<Float>>> computedWindows,
                                   int numberOfThreads, Thread[] threads, Semaphore semaphore) {
        for (List<List<Float>> pixels : filePixels) {
            List<List<Float>> result = new ArrayList<>();
            ThreadUtils.runnerWindows(pixels, numberOfThreads, threads, result, semaphore);
            computedWindows.add(result);
        }
    }

    /**
     * Method to obtain the score of every tree.
     * @param filePixels the list containing information about every pixel from all the files
     * @param numberOfThreads the number of threads
     * @param threads the threads
     * @param thresholds the thresholds for every window of every file
     * @param semaphore the semaphore
     * @return the score of every tree
     */
    public static float getScore(List<List<List<Float>>> filePixels, int numberOfThreads, Thread[] threads,
                                 List<List<Float>> thresholds, Semaphore semaphore) {
        float sum = 0;
        for (int i = 0; i < filePixels.size(); i++) {
            int[] pix = {0, 0, 0, 0};
            ThreadUtils.runnerScore(filePixels.get(i), numberOfThreads, threads, thresholds.get(i), pix, semaphore);
            sum += pix[0] / (pix[0] + 0.5 * (pix[2] + pix[3]));
        }
        return sum / filePixels.size();
    }

    /**
     * Method used to write information to file and increase the counter of row to extract from the tree
     * @param operations the operations used
     * @param bestTrees the 5 best trees
     * @param idx the index of the best one out of the 5 previously mentioned trees
     * @param writer the writer
     * @param maxWidth the number of operations for each row of nodes
     * @param depth the level on which the writing is
     * @return the next level
     */
    public static int getDepthAndPrint(List<String> operations, List<Tree> bestTrees, int idx, PrintWriter writer, int maxWidth, int depth) {
        writer.println("Threshold indexes: " + bestTrees.get(idx).getThreshIdx().get(depth));
        writer.print("Operations are (Every 2 thresholds): ");
        for (int i = 0; i < maxWidth; i++) {
            writer.print(operations.get(bestTrees.get(idx).getOperIdx().get(depth).get(i)));
            if (i != maxWidth - 1) {
                writer.print("; ");
            }
        }
        writer.print("\n\n");
        return depth + 1;
    }
}
