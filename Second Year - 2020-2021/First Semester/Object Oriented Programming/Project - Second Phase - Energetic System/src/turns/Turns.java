package turns;

import common.Constants;
import entities.Consumer;
import input.DistributorChanges;
import entities.Distributor;
import input.ProducerChanges;
import entities.Producer;
import strategies.SortingFactory;
import strategies.SortingStrategy;

import java.util.Comparator;
import java.util.List;

public interface Turns {
    /**
     * A method for the changes that occur during every month.
     * @param numberOfTurns the number of turns to be made.
     * @param consumers the list of consumers
     * @param distributors the list of distributors
     * @param producers the list of producers
     * @param newConsumers a list of list of new consumers for every month
     * @param distributorChanges a list of list of distributor changes for every month
     * @param producerChanges a list of list of producer changes for every month
     */
    void computeTurns(long numberOfTurns, List<Consumer> consumers,
                       List<Distributor> distributors,
                       List<Producer> producers,
                       List<List<Consumer>> newConsumers,
                       List<List<DistributorChanges>> distributorChanges,
                       List<List<ProducerChanges>> producerChanges);

    /**
     * A method to update the producers of a distributor and calculates the new production
     * cost.
     * @param distributor one of the distributors
     * @param producers the list of producers
     */
    default void updateProducers(Distributor distributor, List<Producer> producers) {
        // Creating a strategy and sorting the producers.
        SortingStrategy sortingStrategy = SortingFactory.getInstance().create(
                distributor.getProducerStrategy().getLabel());
        sortingStrategy.sort(producers);
        // Updating the list of producers for a distributor.
        for (Producer producer : producers) {
            if (producer.getDistributorsIds().size() == producer.getMaxDistributors()) {
                continue;
            }
            distributor.getProducers().add(producer);
            producer.getDistributorsIds().add(distributor.getId());
            producer.addObserver(distributor);
            int sum = 0;
            for (Producer prod : distributor.getProducers()) {
                sum += prod.getEnergyPerDistributor();
            }
            if (sum > distributor.getEnergyNeededKW()) {
                break;
            }
        }
        // Calculating the cost of the contract for a distributor.
        double cost = 0;
        for (Producer producer : distributor.getProducers()) {
            cost += producer.getEnergyPerDistributor() * producer.getPriceKW();
        }
        distributor.setProductionCost((int) Math.round(Math.floor(cost / Constants.COST_DIV)));
        // Sorting back the producers after their Id.
        producers.sort(Comparator.comparingInt(Producer::getId));
    }

    /**
     * A method for when a distributor goes Bankrupt and the contracts and producers need
     * to be cleared, clearing the consumer's contract and deleting him from the producers'
     * list of distributors.
     * @param distributor one of the distributors
     * @param consumers the list of consumers
     * @param producers the list of producers
     */
    default void setDistributorBankrupt(Distributor distributor, List<Consumer> consumers,
                                        List<Producer> producers) {
        if (distributor.getBudget() < 0) {
            distributor.setIsBankrupt();
            for (Consumer consumer : consumers) {
                if (distributor.getContracts().contains(consumer.getContract())) {
                    // Clearing the debt and the contract.
                    consumer.setToPayDebt(0);
                    consumer.setInDebt(false);
                    consumer.setMonthsInDebt(0);
                    distributor.getContracts().remove(consumer.getContract());
                    consumer.setContract(null);
                }
            }
            for (Producer producer : producers) {
                if (distributor.getProducers().contains(producer)) {
                    // Removing the distributor from the producer's list
                    distributor.getProducers().remove(producer);
                    producer.getDistributorsIds().remove((Integer) distributor.getId());
                }
            }
        }
    }
}
