package entities;

import input.MonthlyStats;

import java.util.ArrayList;
import java.util.List;
import java.util.Observable;

public final class Producer extends Observable {
    private final int id;
    private final EnergyType energyType;
    private final int maxDistributors;
    private final double priceKW;
    private int energyPerDistributor;
    private final List<MonthlyStats> monthlyStats;
    private final List<Integer> distributorsIds;


    public Producer(final int id, final String energyType,
                    final int maxDistributors, final double priceKW,
                    final int energyPerDistributor) {
        this.id = id;
        this.energyType = EnergyTypeFactory.getInstance().create(energyType);
        this.maxDistributors = maxDistributors;
        this.priceKW = priceKW;
        this.energyPerDistributor = energyPerDistributor;
        this.monthlyStats = new ArrayList<>();
        this.distributorsIds = new ArrayList<>();
    }

    public int getId() {
        return id;
    }

    public EnergyType getEnergyType() {
        return energyType;
    }

    public int getMaxDistributors() {
        return maxDistributors;
    }

    public double getPriceKW() {
        return priceKW;
    }

    public int getEnergyPerDistributor() {
        return energyPerDistributor;
    }

    /**
     * Being observed by distributors, after setting the new energy per distributor, every
     * distributor that observes the producer will be notified.
     * @param energyPerDistributor the value to be changed with because of the monthly updates
     */
    public void setEnergyPerDistributor(final int energyPerDistributor) {
        this.energyPerDistributor = energyPerDistributor;
        setChanged();
        notifyObservers(energyPerDistributor);
        clearChanged();
    }

    public List<MonthlyStats> getMonthlyStats() {
        return monthlyStats;
    }

    public List<Integer> getDistributorsIds() {
        return distributorsIds;
    }
}
