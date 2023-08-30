package turns;

import common.Constants;

import entities.Consumer;
import input.DistributorChanges;
import entities.Distributor;
import input.ProducerChanges;
import entities.Producer;
import input.Contract;
import input.MonthlyStats;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public final class OtherTurns implements Turns {
    /**
     * A method to make the changes that occur every month until it reaches
     * number of turns.
     * @param numberOfTurns the number of turns to be made.
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

        for (long i = 0; i < numberOfTurns; i++) {
            updateDistributorChanges(distributors, distributorChanges.get((int) i));

            // We update the prices of a distributor and find out who is the distributorMin.
            Distributor distributorMin = updateDistributorsPrices(distributors);

            // We make the updates for every consumer, even the new ones.
            updateConsumers(consumers, distributorMin, distributors, newConsumers.get((int) i));

            // We compute the total cost for every distributor, based on their number of contracts.
            for (Distributor distributor : distributors) {
                if (!distributor.getIsBankrupt()) {
                    int costTotal = distributor.getInfrastructureCost()
                            + distributor.getProductionCost() * distributor.getContracts().size();
                    distributor.setTotalCost(costTotal);
                    distributor.setBudget(distributor.getBudget() - distributor.getTotalCost());
                    for (Consumer consumer : consumers) {
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
                    setDistributorBankrupt(distributor, consumers, producers);
                }
            }

            // Decreasing the months in the contract.
            for (Consumer consumer : consumers) {
                if (!consumer.getIsBankrupt()) {
                    int months = consumer.getContract().getRemainedContractMonths();
                    consumer.getContract().setRemainedContractMonths(months - 1);
                }
            }

            updateProducerChanges(producers, producerChanges.get((int) i));
            updateDistributorsProducers(distributors, producers);

            // Adding to the monthlyStats the stats of the current month.
            for (Producer producer : producers) {
                List<Integer> distributorsIds = new ArrayList<>(producer.getDistributorsIds());
                Collections.sort(distributorsIds);
                producer.getMonthlyStats().add(new MonthlyStats((int) i + 1, distributorsIds));
            }
        }
    }

    /**
     * It updates everything related to old or new consumers.
     * @param consumers the list of consumers
     * @param distributorMin the distributor with the lowest price
     * @param distributors the list of distributors
     * @param newConsumers the list of newConsumers in a month
     */
    public void updateConsumers(final List<Consumer> consumers, final Distributor distributorMin,
                                final List<Distributor> distributors,
                                final List<Consumer> newConsumers) {

        // We check if a consumer has an ending contract and we remove it if he does.
        for (Consumer consumer : consumers) {
            if (!consumer.getIsBankrupt()) {
                if (consumer.getContract().getRemainedContractMonths() == 0) {
                    for (Distributor distributor : distributors) {
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
        if (newConsumers != null) {
            consumers.addAll(newConsumers);
        }

        // If a consumer does not have a contract, we create a new one with the
        // distributorMin.
        for (Consumer consumer : consumers) {
            if (!consumer.getIsBankrupt()) {
                if (distributorMin != null) {
                    if (consumer.getContract() == null) {
                        Contract contract = new Contract(consumer.getId(),
                                distributorMin.getContractCost(),
                                distributorMin.getContractLength());
                        consumer.setContract(contract);
                        int idx = distributors.indexOf(distributorMin);
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
                        // If he does not have enough money to pay the debt, he will go
                        // bankrupt.
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
        }
    }

    /**
     * A method to update the prices of every distributor. It also returns
     * the distributor with the lowest price.
     * @param distributors the list of distributors
     * @return the distributorMin / it also updates the prices
     */
    public Distributor updateDistributorsPrices(final List<Distributor> distributors) {
        // distributorMin is the distributor with the lowest price.
        Distributor distributorMin = null;
        int minPrice = Integer.MAX_VALUE;

        for (Distributor distributor : distributors) {
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
                distributor.setContractCost(finPrice);
                if (finPrice < minPrice) {
                    minPrice = finPrice;
                    distributorMin = distributor;
                }
            }
        }
        return distributorMin;
    }

    /**
     * A method to remove the producers from a distributor's list and the distributor from
     * every list of distributors that producers have, updating the new producers for every
     * distributor that needs an update.
     * @param distributors the list of distributors
     * @param producers the list of producers
     */
    public void updateDistributorsProducers(final List<Distributor> distributors,
                                            final List<Producer> producers) {
        for (Distributor distributor : distributors) {
            if (!distributor.getIsBankrupt()) {
                if (distributor.isChangingProducers()) {
                    for (Producer producer : producers) {
                        if (producer.getDistributorsIds().contains(distributor.getId())) {
                            // Deleting the distributor from every producer that was giving him
                            // energy, as well as removing the producer from his list of producers.
                            producer.getDistributorsIds().remove((Integer) distributor.getId());
                            distributor.getProducers().remove(producer);
                            producer.deleteObserver(distributor);
                        }
                    }
                    updateProducers(distributor, producers);
                    distributor.setChangingProducers(false);
                }
            }
        }
    }

    /**
     * A method to update the infrastructure cost of a distributor if there is any
     * change in that month for him.
     * @param distributors the list of distributors
     * @param distributorChanges the list of distributor changes that occur in a month
     */
    public void updateDistributorChanges(final List<Distributor> distributors,
                                          final List<DistributorChanges> distributorChanges) {
        if (distributorChanges != null) {
            for (DistributorChanges distributorChange : distributorChanges) {
                for (Distributor distributor : distributors) {
                    if (!distributor.getIsBankrupt()) {
                        if (distributor.getId() == distributorChange.getId()) {
                            distributor.setInfrastructureCost(distributorChange.
                                    getInfrastructureCost());
                        }
                    }
                }
            }
        }
    }

    /**
     * A method to update the energy per distributor of a producer if there is any
     * change in that month for him.
     * @param producers the list of producers
     * @param producerChanges the list of producer changes that occur in a month
     */
    public void updateProducerChanges(List<Producer> producers,
                                      List<ProducerChanges> producerChanges) {
        if (producerChanges != null) {
            for (ProducerChanges producerChange : producerChanges) {
                for (Producer producer : producers) {
                    if (producer.getId() == producerChange.getId()) {
                        producer.setEnergyPerDistributor(producerChange.
                                getEnergyPerDistributor());
                    }
                }
            }
        }
    }
}
