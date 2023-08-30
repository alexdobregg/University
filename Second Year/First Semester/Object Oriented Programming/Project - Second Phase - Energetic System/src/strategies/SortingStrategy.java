package strategies;

import entities.Producer;

import java.util.List;

public interface SortingStrategy {
    /**
     * A method to sort the producers.
     * @param producers a list of producers to be sorted
     */
    void sort(List<Producer> producers);
}
