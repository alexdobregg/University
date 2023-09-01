package strategies;

import entities.Producer;

import java.util.Comparator;
import java.util.List;

public final class QuantitySortingStrategy implements SortingStrategy {
    /**
     * It sorts after quantity.
     * @param producers a list of producers to be sorted
     */
    public void sort(List<Producer> producers) {
        producers.sort(Comparator.comparingInt(Producer::getEnergyPerDistributor).
                            reversed());
    }
}
