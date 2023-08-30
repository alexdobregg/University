package turns;

import common.Constants;

import entities.Consumer;
import input.DistributorChanges;
import entities.Distributor;
import input.ProducerChanges;
import entities.Producer;
import input.Contract;
import java.util.List;

public final class FirstTurn implements Turns {
    /**
     * A method to make the changes that occur in the first month.
     * @param numberOfTurns the number of turns to be made; not used in this method
     * @param consumers the list of consumers
     * @param distributors the list of distributors
     * @param producers the list of producers
     * @param newConsumers a list of list of new consumers for every month
     * @param distributorChanges a list of list of distributor changes for every month
     * @param producerChanges a list of list of producer changes for every month
     */
    public void computeTurns(final long numberOfTurns, final List<Consumer> consumers,
                       final List<Distributor> distributors,
                       final List<Producer> producers,
                       final List<List<Consumer>> newConsumers,
                       final List<List<DistributorChanges>> distributorChanges,
                       final List<List<ProducerChanges>> producerChanges) {

        for (Distributor distributor : distributors) {
            updateProducers(distributor, producers);
        }

        // distributorMin is the distributor with the lowest price.
        Distributor distributorMin = null;
        int minPrice = Integer.MAX_VALUE;
        for (Distributor distributor : distributors) {
            int prof = Math.round((int) Math.floor(Constants.PROF * distributor.
                    getProductionCost()));
            distributor.setProfit(prof);
            int finPrice = distributor.getInfrastructureCost()
                    + distributor.getProductionCost()
                    + distributor.getProfit();
            distributor.setContractCost(finPrice);
            if (finPrice < minPrice) {
                minPrice = finPrice;
                distributorMin = distributor;
            }
        }

        // We create contracts with distributorMin for every consumer.
        for (Consumer consumer : consumers) {
            if (distributorMin != null) {
                Contract contract = new Contract(consumer.getId(),
                        distributorMin.getContractCost(),
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
                            + distributorMin.getContractCost());
                }
            }
        }

        // We compute the total cost for every distributor, based on their number of contracts.
        for (Distributor distributor : distributors) {
            int totalCost = distributor.getInfrastructureCost()
                    + distributor.getProductionCost() * distributor.getContracts().size();
            distributor.setTotalCost(totalCost);
            distributor.setBudget(distributor.getBudget() - distributor.getTotalCost());
            // If he goes bankrupt, all his contracts are removed and if there is any
            // debt from any consumer that had a contract with him, it is erased.
            setDistributorBankrupt(distributor, consumers, producers);
        }

        // If the consumer is not in debt, he will pay the contract price.
        for (Consumer consumer : consumers) {
            if (consumer.isInDebt()) {
                consumer.setBudget(consumer.getBudget() + consumer.getMonthlyIncome());
            } else {
                consumer.setBudget(consumer.getBudget() - consumer.getContract().getPrice()
                        + consumer.getMonthlyIncome());
            }
        }
    }
}
