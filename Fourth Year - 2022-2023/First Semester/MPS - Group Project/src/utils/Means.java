package utils;

import java.util.List;

public class Means {
    /**
     * Method to obtain the mean of two thresholds
     * @param thr1 the first threshold
     * @param thr2 the second threshold
     * @return the mean
     */
    public static float mean(float thr1, float thr2) {
        return (thr1 + thr2) / 2;
    }

    /**
     * Method to obtain the geometric mean of two thresholds
     * @param thr1 the first threshold
     * @param thr2 the second threshold
     * @return the geometric mean
     */
    public static float geometric_mean(float thr1, float thr2) {
        return (float) Math.sqrt(thr1 * thr2);
    }

    /**
     * Method to obtain the harmonic mean of two thresholds
     * @param thr1 the first threshold
     * @param thr2 the second threshold
     * @return the harmonic mean
     */
    public static float harmonic_mean(float thr1, float thr2) {
        return 2 * thr1 * thr2 / (thr1 + thr2);
    }

    /**
     * Method to obtain the square mean of two thresholds
     * @param thr1 the first threshold
     * @param thr2 the second threshold
     * @return the square mean
     */
    public static float square_mean(float thr1, float thr2) {
        return (float) Math.sqrt((Math.pow(thr1, 2) + Math.pow(thr2, 2)) / 2);
    }

    /**
     * Method to obtain the cubic mean of two thresholds
     * @param thr1 the first threshold
     * @param thr2 the second threshold
     * @return the cubic mean
     */
    public static Float cubic_mean(float thr1, float thr2) {
        return (float) Math.cbrt((Math.pow(thr1, 3) + Math.pow(thr2, 3)) / 2);
    }

    /**
     * Method to obtain the mean of the scores.
     * @param numbers list of scores
     * @return the mean of scores
     */
    public static Float mean_score(List<Float> numbers) {
        float sum = 0;
        for (Float i : numbers) {
            sum += i;
        }
        return sum / numbers.size();
    }
}
