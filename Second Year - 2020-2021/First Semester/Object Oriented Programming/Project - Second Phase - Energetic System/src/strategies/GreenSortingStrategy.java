package strategies;

import entities.Producer;
import java.util.Comparator;
import java.util.List;

public final class GreenSortingStrategy implements SortingStrategy {
    /**
     * It sorts after the type of energy, price and quantity.
     * @param producers a list of producers to be sorted
     */
    public void sort(List<Producer> producers) {
        producers.sort(Comparator.comparingInt(Producer::getEnergyPerDistributor).
                            reversed());
        producers.sort(Comparator.comparingDouble(Producer::getPriceKW));
        producers.sort((Producer p1, Producer p2) -> Boolean.compare(
                                p2.getEnergyType().isRenewable(), p1.getEnergyType().
                                isRenewable()));
    }
}
