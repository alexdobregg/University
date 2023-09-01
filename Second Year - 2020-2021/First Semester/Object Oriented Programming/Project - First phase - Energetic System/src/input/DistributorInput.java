package input;

import java.util.ArrayList;
import java.util.List;

public final class DistributorInput {
    private final int id;
    private final int contractLength;
    private boolean isBankrupt;
    private int budget;
    private int infrastructureCost;
    private int productionCost;
    private List<Contract> contracts;
    private int finalPrice;
    private int profit;
    private int totalCost;

    public DistributorInput(final int id, final int contractLength, final int initialBudget,
                            final int initialInfraCost, final int initialProdCost) {
        this.id = id;
        this.contractLength = contractLength;
        this.budget = initialBudget;
        this.infrastructureCost = initialInfraCost;
        this.productionCost = initialProdCost;
        this.isBankrupt = false;
        this.contracts = new ArrayList<>();
        this.finalPrice = 0;
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

    public int getFinalPrice() {
        return finalPrice;
    }

    public void setFinalPrice(final int finalPrice) {
        this.finalPrice = finalPrice;
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
}
