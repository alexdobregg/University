package input;

import java.util.List;

public final class Input {
    private final long numberOfTurns;
    private final List<ConsumerInput> consumers;
    private final List<DistributorInput> distributors;
    private final List<List<ConsumerInput>> newConsumers;
    private final List<List<CostsChanges>> costsChanges;

    public Input(final long numberOfTurns, final List<ConsumerInput> consumers,
                 final List<DistributorInput> distributors,
                 final List<List<ConsumerInput>> newConsumers,
                 final List<List<CostsChanges>> costsChanges) {
        this.numberOfTurns = numberOfTurns;
        this.consumers = consumers;
        this.distributors = distributors;
        this.newConsumers = newConsumers;
        this.costsChanges = costsChanges;
    }

    public long getNumberOfTurns() {
        return numberOfTurns;
    }

    public List<ConsumerInput> getConsumers() {
        return consumers;
    }

    public List<DistributorInput> getDistributors() {
        return distributors;
    }

    public List<List<ConsumerInput>> getNewConsumers() {
        return newConsumers;
    }

    public List<List<CostsChanges>> getCostsChanges() {
        return costsChanges;
    }
}
