package entities;

import input.Contract;
import strategies.EnergyChoiceStrategyType;
import strategies.EnergyChoiceFactory;

import java.util.ArrayList;
import java.util.List;
import java.util.Observable;
import java.util.Observer;

public final class Distributor implements Observer {
    private final int id;
    private final int contractLength;
    private boolean isBankrupt;
    private int budget;
    private int infrastructureCost;
    private final int energyNeededKW;
    private final EnergyChoiceStrategyType producerStrategy;
    private int productionCost;
    private final List<Contract> contracts;
    private final List<Producer> producers;
    /**
     * changingProducers is a field that will change to true if a producer changes its energy per
     * distributor.
     */
    private boolean changingProducers;
    private int contractCost;
    private int profit;
    private int totalCost;

    public Distributor(final int id, final int contractLength, final int initialBudget,
                       final int initialInfraCost, final int energyNeededKW,
                       final String type) {
        this.id = id;
        this.contractLength = contractLength;
        this.budget = initialBudget;
        this.infrastructureCost = initialInfraCost;
        this.energyNeededKW = energyNeededKW;
        this.producerStrategy = EnergyChoiceFactory.getInstance().create(type);
        this.productionCost = 0;
        this.isBankrupt = false;
        this.contracts = new ArrayList<>();
        this.producers = new ArrayList<>();
        this.changingProducers = false;
        this.contractCost = 0;
        this.profit = 0;
        this.totalCost = 0;
    }

    public void setBudget(final int budget) {
        this.budget = budget;
    }

    public void setInfrastructureCost(final int infrastructureCost) {
        this.infrastructureCost = infrastructureCost;
    }

    public void setProductionCost(final int productionCost) {
        this.productionCost = productionCost;
    }

    /**
     * A method to set the isBankrupt field to true.
     */
    public void setIsBankrupt() {
        this.isBankrupt = true;
    }

    public int getId() {
        return id;
    }

    public int getContractLength() {
        return contractLength;
    }

    public int getBudget() {
        return budget;
    }

    public int getInfrastructureCost() {
        return infrastructureCost;
    }

    public int getProductionCost() {
        return productionCost;
    }

    public boolean getIsBankrupt() {
        return isBankrupt;
    }

    public int getContractCost() {
        return contractCost;
    }

    public void setContractCost(final int contractCost) {
        this.contractCost = contractCost;
    }

    public int getProfit() {
        return profit;
    }

    public void setProfit(final int profit) {
        this.profit = profit;
    }

    public int getTotalCost() {
        return totalCost;
    }

    public void setTotalCost(final int costTotal) {
        this.totalCost = costTotal;
    }

    public List<Contract> getContracts() {
        return contracts;
    }

    public int getEnergyNeededKW() {
        return energyNeededKW;
    }

    public EnergyChoiceStrategyType getProducerStrategy() {
        return producerStrategy;
    }

    public List<Producer> getProducers() {
        return producers;
    }

    public boolean isChangingProducers() {
        return changingProducers;
    }

    public void setChangingProducers(boolean changingProducers) {
        this.changingProducers = changingProducers;
    }

    @Override
    public void update(Observable o, Object arg) {
        this.setChangingProducers(true);
    }
}
