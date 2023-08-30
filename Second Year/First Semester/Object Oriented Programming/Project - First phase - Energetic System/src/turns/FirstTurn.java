package turns;

import common.Constants;
import input.ConsumerInput;
import input.Contract;
import input.CostsChanges;
import input.DistributorInput;

import java.util.List;

public final class FirstTurn implements Turns {
    /**
     * A method to make the changes that occur in the first month.
     * @param numberOfTurns the number of turns to be made; not used in this method
     * @param consumers the list of consumers
     * @param distributors the list of distributors
     * @param newConsumers a list of list of new consumers for every month
     * @param costsChanges a list of list of cost changes for every month
     */
    public void computeTurns(final long numberOfTurns, final List<ConsumerInput> consumers,
                       final List<DistributorInput> distributors,
                       final List<List<ConsumerInput>> newConsumers,
                       final List<List<CostsChanges>> costsChanges) {
        // distributorMin is the distributor with the lowest price.
        DistributorInput distributorMin = null;
        int minPrice = Integer.MAX_VALUE;
        for (DistributorInput distributor : distributors) {
            int prof = Math.round((int) Math.floor(Constants.PROF * distributor.
                    getProductionCost()));
            distributor.setProfit(prof);
            int finPrice = distributor.getInfrastructureCost()
                    + distributor.getProductionCost()
                    + distributor.getProfit();
            distributor.setFinalPrice(finPrice);
            if (finPrice < minPrice) {
                minPrice = finPrice;
                distributorMin = distributor;
            }
        }
        // We create contracts with distributorMin for every consumer.
        for (ConsumerInput consumer : consumers) {
            Contract contract = new Contract(consumer.getId(),
                    distributorMin.getFinalPrice(),
                    distributorMin.getContractLength() - 1);
            consumer.setContract(contract);
            int idx = distributors.indexOf(distributorMin);
            distributors.get(idx).getContracts().add(contract);
            // We check to see if the consumer can pay his first contract price.
            if (consumer.getBudget() - consumer.getContract().getPrice()
                    + consumer.getMonthlyIncome() < 0) {
                consumer.setInDebt(true);
                consumer.setMonthsInDebt(consumer.getMonthsInDebt() + 1);
                consumer.setToPayDebt(consumer.getContract().getPrice());
            }
            // If he is not in debt, the distributor receives consumer's first contract price.
            if (!consumer.isInDebt()) {
                distributors.get(idx).setBudget(distributors.get(idx).getBudget()
                        + distributorMin.getFinalPrice());
            }
        }
        // We compute the total cost for every distributor, based on their number of contracts.
        for (DistributorInput distributor : distributors) {
            int totalCost = distributor.getInfrastructureCost()
                    + distributor.getProductionCost() * distributor.getContracts().size();
            distributor.setTotalCost(totalCost);
            distributor.setBudget(distributor.getBudget() - distributor.getTotalCost());
            // If he goes bankrupt, all his contracts are removed and if there is any
            // debt from any consumer that had a contract with him, it is erased.
            if (distributor.getBudget() < 0) {
                distributor.setIsBankrupt();
                for (ConsumerInput consumer : consumers) {
                    if (distributor.getContracts().contains(consumer.getContract())) {
                        consumer.setToPayDebt(0);
                        consumer.setInDebt(false);
                        consumer.setMonthsInDebt(0);
                        distributor.getContracts().remove(consumer.getContract());
                        consumer.setContract(null);
                    }
                }
            }
        }
        // If the consumer is not in debt, he will pay the contract price.
        for (ConsumerInput consumer : consumers) {
            if (consumer.isInDebt()) {
                consumer.setBudget(consumer.getBudget() + consumer.getMonthlyIncome());
            } else {
                consumer.setBudget(consumer.getBudget() - consumer.getContract().getPrice()
                        + consumer.getMonthlyIncome());
            }
        }
    }
}
