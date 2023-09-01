package turns;

import common.Constants;
import input.ConsumerInput;
import input.Contract;
import input.CostsChanges;
import input.DistributorInput;

import java.util.List;

public final class OtherTurns implements Turns {
    /**
     * A method to make the changes that occur every month until it reaches
     * number of turns.
     * @param numberOfTurns the number of turns to be made.
     * @param consumers the list of consumers
     * @param distributors the list of distributors
     * @param newConsumers a list of list of new consumers for every month
     * @param costsChanges a list of list of cost changes for every month
     */
    public void computeTurns(final long numberOfTurns, final List<ConsumerInput> consumers,
                       final List<DistributorInput> distributors,
                       final List<List<ConsumerInput>> newConsumers,
                       final List<List<CostsChanges>> costsChanges) {
        for (long i = 0; i < numberOfTurns; i++) {
            // distributorMin is the distributor with the lowest price.
            DistributorInput distributorMin = null;
            int minPrice = Integer.MAX_VALUE;
            // We update the infrastructure cost and the production cost, if there are costs
            // changes during the month.
            if (costsChanges.get((int) i) != null) {
                for (CostsChanges costChange : costsChanges.get(((int) i))) {
                    for (DistributorInput distributor : distributors) {
                        if (!distributor.getIsBankrupt()) {
                            if (distributor.getId() == costChange.getId()) {
                                distributor.setInfrastructureCost(costChange.
                                        getInfrastructureCost());
                                distributor.setProductionCost(costChange.getProductionCost());
                            }
                        }
                    }
                }
            }
            // We update the prices of a distributor.
            for (DistributorInput distributor : distributors) {
                if (!distributor.getIsBankrupt()) {
                    int prof = Math.round((int) Math.floor(Constants.PROF * distributor.
                            getProductionCost()));
                    distributor.setProfit(prof);
                    int finPrice;
                    if (distributor.getContracts().isEmpty()) {
                        finPrice = distributor.getInfrastructureCost()
                                + distributor.getProductionCost()
                                + distributor.getProfit();
                    } else {
                        finPrice = Math.round(
                                (int) Math.floor((float) distributor.getInfrastructureCost()
                                        / distributor.getContracts().size())
                                        + distributor.getProductionCost()
                                        + distributor.getProfit());
                    }
                    distributor.setFinalPrice(finPrice);
                    if (finPrice < minPrice) {
                        minPrice = finPrice;
                        distributorMin = distributor;
                    }
                }
            }
            // We check if a consumer has an ending contract and we remove it if he does.
            for (ConsumerInput consumer : consumers) {
                if (!consumer.getIsBankrupt()) {
                    if (consumer.getContract().getRemainedContractMonths() == 0) {
                        for (DistributorInput distributor : distributors) {
                            if (distributor.getContracts().contains(consumer.getContract())) {
                                distributor.getContracts().remove(consumer.getContract());
                                break;
                            }
                        }
                        consumer.setContract(null);
                    }
                }
            }
            // We add all the new consumers if there are any.
            if (newConsumers.get((int) i) != null) {
                consumers.addAll(newConsumers.get((int) i));
            }
            // If a consumer does not have a contract, we create a new one with the
            // distributorMin.
            for (ConsumerInput consumer : consumers) {
                if (!consumer.getIsBankrupt()) {
                    int idx = distributors.indexOf(distributorMin);
                    if (consumer.getContract() == null) {
                        Contract contract = new Contract(consumer.getId(),
                                distributorMin.getFinalPrice(),
                                distributorMin.getContractLength());
                        consumer.setContract(contract);
                        distributors.get(idx).getContracts().add(contract);
                    }
                    consumer.setBudget(consumer.getBudget() + consumer.getMonthlyIncome());
                    if (!consumer.isInDebt()) {
                        // If he does not have enough money to pay, he will be in debt.
                        if (consumer.getBudget() < consumer.getContract().getPrice()) {
                            consumer.setInDebt(true);
                            consumer.setMonthsInDebt(consumer.getMonthsInDebt() + 1);
                            consumer.setToPayDebt(consumer.getContract().getPrice());
                        } else {
                            consumer.setBudget(consumer.getBudget() - consumer.getContract().
                                    getPrice());
                        }
                    } else if (consumer.getMonthsInDebt() == 1) {
                        // If he does not have enough money to pay the debt, he will go bankrupt.
                        int toPay = Math.round((int) Math.floor(Constants.FACT_IN_DEBT
                                * consumer.getToPayDebt()))
                                + consumer.getContract().getPrice();
                        if (consumer.getBudget() < toPay) {
                            consumer.setMonthsInDebt(consumer.getMonthsInDebt() + 1);
                            consumer.setIsBankrupt();
                        } else {
                            consumer.setBudget(consumer.getBudget() - toPay);
                            consumer.setMonthsInDebt(0);
                            consumer.setInDebt(false);
                            consumer.setToPayDebt(0);
                        }
                    }
                }
            }
            // We compute the total cost for every distributor, based on their number of contracts.
            for (DistributorInput distributor : distributors) {
                if (!distributor.getIsBankrupt()) {
                    int costTotal = distributor.getInfrastructureCost()
                            + distributor.getProductionCost() * distributor.getContracts().size();
                    distributor.setTotalCost(costTotal);
                    distributor.setBudget(distributor.getBudget() - distributor.getTotalCost());
                    for (ConsumerInput consumer : consumers) {
                        if (!distributor.getContracts().isEmpty()
                                && distributor.getContracts().contains(consumer.getContract())) {
                            if (consumer.isInDebt()) {
                                // If there are any consumers that went bankrupt this month, the
                                // contract will be removed.
                                if (consumer.getMonthsInDebt() == 2) {
                                    distributor.getContracts().remove(consumer.getContract());
                                }
                            } else {
                                distributor.setBudget(distributor.getBudget()
                                        + consumer.getContract().getPrice());
                            }
                        }
                    }
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
            }
            // Decreasing the months in the contract.
            for (ConsumerInput consumer : consumers) {
                if (!consumer.getIsBankrupt()) {
                    int months = consumer.getContract().getRemainedContractMonths();
                    consumer.getContract().setRemainedContractMonths(months - 1);
                }
            }
        }
    }
}
