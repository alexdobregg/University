package local;

import utils.Means;
import utils.Utils;
import utils.Tree;
import java.io.File;
import java.util.*;
import java.util.concurrent.Semaphore;

public class ThreadUtils {
    /**
     * This is the parent method for readFile that reads and stores all the pixels from the files.
     */
    public static void runnerRead(List<File> filesList, int numberOfThreads, Thread[] threads,
                                  List<List<List<Float>>> filePixels, Semaphore semaphore) {
        for (int t = 0; t < numberOfThreads; t++) {
            int thread = t;
            threads[t] = new Thread(() -> {
                try {
                    ThreadUtils.readFile(filesList, numberOfThreads, thread, filePixels, semaphore);
                } catch (Exception ignored) {}
            });
        }
        for (Thread t1 : threads) {
            t1.start();
        }
        for (Thread t2 : threads) {
            try {
                t2.join();
            } catch (Exception ignored) {}
        }
    }

    /**
     * Method to obtain the information about every pixel from every file.
     * @param filesList the list of files
     * @param numberOfThreads the number of threads
     * @param thread the current thread
     * @param result the variable in which we store the information
     * @param semaphore the semaphore
     */
    public static void readFile(List<File> filesList, int numberOfThreads, int thread, List<List<List<Float>>> result,
                                Semaphore semaphore) throws Exception {
        // Obtaining the start and the end for each thread to divide the files between each other.
        List<Integer> range = Utils.getThreadRange(filesList.size(), numberOfThreads, thread);
        List<File> inFiles = filesList.subList(range.get(0), range.get(1));
        List<List<List<Float>>> pixels = new ArrayList<>();
        for (File file : inFiles) {
            pixels.add(CsvParser.parse(file));
        }
        semaphore.acquire();
        result.addAll(pixels);
        semaphore.release();
    }

    /**
     * This is the parent method for computeWindows to obtain the windows of every file
     */
    public static void runnerWindows(List<List<Float>> pixels, int numberOfThreads, Thread[] threads, List<List<Float>> result,
                                     Semaphore semaphore) {
        for (int t = 0; t < numberOfThreads; t++) {
            int thread = t;
            threads[t] = new Thread(() -> {
                try {
                    ThreadUtils.computeWindows(pixels, numberOfThreads, thread, result, semaphore);
                } catch (Exception ignored) {}
            });
        }
        for (Thread t1 : threads) {
            t1.start();
        }
        for (Thread t2 : threads) {
            try {
                t2.join();
            } catch (Exception ignored) {}
        }
    }

    /**
     * Method to compute the windows. The number of windows is equal to the number of threads. We calculate general
     * thresholds for each window by using mean for each type of threshold.
     * @param pixels the list of pixels from a file
     * @param numberOfThreads the number of threads
     * @param thread the current thread
     * @param result the variable in which we store the computed windows
     * @param semaphore the semaphore
     */
    public static void computeWindows(List<List<Float>> pixels, int numberOfThreads, int thread, List<List<Float>> result,
                                      Semaphore semaphore) throws Exception {
        // Obtaining the start and the end for each thread to divide the pixels from a file between each other.
        List<Integer> range = Utils.getThreadRange(pixels.size(), numberOfThreads, thread);
        List<List<Float>> inPixels = pixels.subList(range.get(0), range.get(1));

        float[] thrSum = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

        for (List<Float> inPixel : inPixels) {
            for (int j = 0; j < 10; j++) {
                if (inPixel.get(j + 2) < 0.2) {
                    thrSum[j] += inPixel.get(j + 2) + 0.4;
                } else {
                    thrSum[j] += inPixel.get(j + 2);
                }
            }
        }
        List<Float> res = new ArrayList<>();
        for (int i = 0; i < 10; i++) {
            res.add(thrSum[i] /= inPixels.size());
        }
        semaphore.acquire();
        result.add(res);
        semaphore.release();
    }

    /**
     * This is the parent method for obtainThresholds in which we obtain a threshold for each window.
     */
    public static void runnerThresholds(List<List<List<Float>>> computedWindows, int numberOfThreads, Thread[] threads,
                                        List<String> operations, Tree tree, List<List<Float>> thresholds,
                                        Semaphore semaphore) {
        for (int t = 0; t < numberOfThreads; t++) {
            int thread = t;
            threads[t] = new Thread(() -> {
                try {
                    ThreadUtils.obtainThresholds(computedWindows, numberOfThreads, thread, operations, tree, thresholds,
                                                 semaphore);
                } catch (Exception ignored) {}
            });
        }
        for (Thread t1 : threads) {
            t1.start();
        }
        for (Thread t2 : threads) {
            try {
                t2.join();
            } catch (Exception ignored) {}
        }
    }

    /**
     * Method to obtain the thresholds for each file.
     * @param computedWindows the computed windows previously obtained
     * @param numberOfThreads the number of threads
     * @param thread the current thread
     * @param operations the operations
     * @param tree the tree used to obtain the thresholds
     * @param thresholds the variable in which we store the obtained thresholds
     * @param semaphore the semaphore
     */
    public static void obtainThresholds(List<List<List<Float>>> computedWindows, int numberOfThreads, int thread,
                                        List<String> operations, Tree tree, List<List<Float>> thresholds,
                                        Semaphore semaphore) throws Exception {
        // Obtaining the start and the end for each thread to divide the files they handle between each other.
        List<Integer> range = Utils.getThreadRange(computedWindows.size(), numberOfThreads, thread);
        List<List<List<Float>>> inWindows = computedWindows.subList(range.get(0), range.get(1));
        List<Float> tempThresh = new ArrayList<>();
        List<Float> thresh = new ArrayList<>();
        List<Float> fileThresholds = new ArrayList<>();
        List<List<Float>> fileTemp = new ArrayList<>();

        for (List<List<Float>> inWindow : inWindows) {
            for (List<Float> window : inWindow) {
                thresh.clear();
                thresh.addAll(window);
                // This is the maximum width of the operations at each level.
                int maxWidth = operations.size();
                /* Considering we start from the leaves and go to root, in the index list we have the indexes stored
                the same way. */
                int depth = 0;
                boolean second = false;
                while (maxWidth >= 1) {
                    for (int i = 0; i < maxWidth; i++) {
                        float thr1 = thresh.get(tree.getThreshIdx().get(depth).get(i * 2));
                        float thr2 = thresh.get(tree.getThreshIdx().get(depth).get(i * 2 + 1));
                        String operation = operations.get(tree.getOperIdx().get(depth).get(i));
                        if (Objects.equals(operation, "CubicMean")) {
                            tempThresh.add(Means.cubic_mean(thr1, thr2));
                        } else if (Objects.equals(operation, "SquareMean")) {
                            tempThresh.add(Means.square_mean(thr1, thr2));
                        } else if (Objects.equals(operation, "ArithmeticMean")) {
                            tempThresh.add(Means.mean(thr1, thr2));
                        } else if (Objects.equals(operation, "HarmonicMean")) {
                            tempThresh.add(Means.harmonic_mean(thr1, thr2));
                        } else if (Objects.equals(operation, "GeometricMean")) {
                            tempThresh.add(Means.geometric_mean(thr1, thr2));
                        }
                    }
                    depth++;
                    // On the second level, we only use one operation and pass the rest of thresholds one level above.
                    if (maxWidth == operations.size()) {
                        maxWidth = 1;
                        second = true;
                    } else if (maxWidth == 1 && second){
                        tempThresh.add(thresh.get(thresh.size() - 1));
                        tempThresh.add(thresh.get(thresh.size() - 2));
                        tempThresh.add(thresh.get(thresh.size() - 3));
                        maxWidth *= 2;
                        second = false;
                    } else {
                        maxWidth /= 2;
                    }
                    thresh.clear();
                    thresh.addAll(tempThresh);
                    tempThresh.clear();
                }
                fileThresholds.add(thresh.get(0));
            }
            fileTemp.add(fileThresholds);
        }
        semaphore.acquire();
        thresholds.addAll(fileTemp);
        semaphore.release();
    }

    /**
     * This is the parent method for obtainScores in which we calculate the scores of each tree.
     */
    public static void runnerScore(List<List<Float>> pixels, int numberOfThreads, Thread[] threads,
                                   List<Float> threshold, int[] scores, Semaphore semaphore) {
        for (int t = 0; t < numberOfThreads; t++) {
            int thread = t;
            threads[t] = new Thread(() -> {
                try {
                    ThreadUtils.obtainScores(pixels, numberOfThreads, thread, threshold, scores, semaphore);
                } catch (Exception ignored) {}
            });
        }
        for (Thread t1 : threads) {
            t1.start();
        }
        for (Thread t2 : threads) {
            try {
                t2.join();
            } catch (Exception ignored) {}
        }
    }

    /**
     *
     * @param pixels a list with every pixel from a file
     * @param numberOfThreads the number of threads
     * @param thread the current thread
     * @param threshold the list of thresholds for each file
     * @param scores the variable in which we store the scores
     * @param semaphore the semaphores
     */

    public static void obtainScores(List<List<Float>> pixels, int numberOfThreads, int thread,
                                    List<Float> threshold, int[] scores, Semaphore semaphore) throws Exception {
        List<Integer> range = Utils.getThreadRange(pixels.size(), numberOfThreads, thread);
        List<List<Float>> inPixels = pixels.subList(range.get(0), range.get(1));
        int localTP = 0;
        int localTN = 0;
        int localFP = 0;
        int localFN = 0;

        for (List<Float> pixel : inPixels) {
            if (pixel.get(0) > threshold.get(thread) && pixel.get(1) == 0) {
                localTN++;
            } else if (pixel.get(0) > threshold.get(thread) && pixel.get(1) == 1) {
                localFN++;
            } else if (pixel.get(0) <= threshold.get(thread) && pixel.get(1) == 0) {
                localFP++;
            } else if (pixel.get(0) <= threshold.get(thread) && pixel.get(1) == 1) {
                localTP++;
            }
        }
        semaphore.acquire();
        scores[0] += localTP;
        scores[1] += localTN;
        scores[2] += localFP;
        scores[3] += localFN;
        semaphore.release();
    }
}
