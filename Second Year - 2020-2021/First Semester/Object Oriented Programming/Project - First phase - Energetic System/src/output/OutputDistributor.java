package output;

import input.Contract;

import java.util.List;

public final class OutputDistributor {
    private final int id;
    private final int budget;
    private final boolean isBankrupt;
    private final List<Contract> contracts;

    public OutputDistributor(final int id, final int budget, final boolean isBankrupt,
                             final List<Contract> contracts) {
        this.id = id;
        this.budget = budget;
        this.isBankrupt = isBankrupt;
        this.contracts = contracts;
    }

    public int getId() {
        return id;
    }

    public int getBudget() {
        return budget;
    }

    public boolean getIsBankrupt() {
        return isBankrupt;
    }

    public List<Contract> getContracts() {
        return contracts;
    }
}
