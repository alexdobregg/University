package output;

import input.Contract;
import strategies.EnergyChoiceStrategyType;

import java.util.List;

public final class OutputDistributor {
    private final int id;
    private final int energyNeededKW;
    private final int contractCost;
    private final int budget;
    private final EnergyChoiceStrategyType producerStrategy;
    private final boolean isBankrupt;
    private final List<Contract> contracts;

    public OutputDistributor(final int id, final int energyNeededKW, final int contractCost,
                             final int budget, final EnergyChoiceStrategyType producerStrategy,
                             final boolean isBankrupt, final List<Contract> contracts) {
        this.id = id;
        this.energyNeededKW = energyNeededKW;
        this.contractCost = contractCost;
        this.budget = budget;
        this.producerStrategy = producerStrategy;
        this.isBankrupt = isBankrupt;
        this.contracts = contracts;
    }

    public int getId() {
        return id;
    }

    public int getEnergyNeededKW() {
        return energyNeededKW;
    }

    public int getContractCost() {
        return contractCost;
    }

    public int getBudget() {
        return budget;
    }

    public EnergyChoiceStrategyType getProducerStrategy() {
        return producerStrategy;
    }

    public boolean getIsBankrupt() {
        return isBankrupt;
    }

    public List<Contract> getContracts() {
        return contracts;
    }
}
