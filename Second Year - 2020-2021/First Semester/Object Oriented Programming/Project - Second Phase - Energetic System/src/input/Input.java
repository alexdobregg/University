package input;

import entities.Consumer;
import entities.Distributor;
import entities.Producer;

import java.util.List;

public final class Input {
    private final long numberOfTurns;
    private final List<Consumer> consumers;
    private final List<Distributor> distributors;
    private final List<Producer> producers;
    private final List<List<Consumer>> newConsumers;
    private final List<List<DistributorChanges>> distributorChanges;
    private final List<List<ProducerChanges>> producerChanges;

    public Input(final long numberOfTurns, final List<Consumer> consumers,
                 final List<Distributor> distributors,
                 final List<Producer> producers,
                 final List<List<Consumer>> newConsumers,
                 final List<List<DistributorChanges>> distributorChanges,
                 final List<List<ProducerChanges>> producerChanges) {
        this.numberOfTurns = numberOfTurns;
        this.consumers = consumers;
        this.distributors = distributors;
        this.producers = producers;
        this.newConsumers = newConsumers;
        this.distributorChanges = distributorChanges;
        this.producerChanges = producerChanges;
    }

    public long getNumberOfTurns() {
        return numberOfTurns;
    }

    public List<Consumer> getConsumers() {
        return consumers;
    }

    public List<Distributor> getDistributors() {
        return distributors;
    }

    public List<Producer> getProducers() {
        return producers;
    }

    public List<List<Consumer>> getNewConsumers() {
        return newConsumers;
    }

    public List<List<DistributorChanges>> getDistributorChanges() {
        return distributorChanges;
    }

    public List<List<ProducerChanges>> getProducerChanges() {
        return producerChanges;
    }
}
