package turns;

import input.ConsumerInput;
import input.CostsChanges;
import input.DistributorInput;

import java.util.List;

public interface Turns {
    /**
     * A method for the changes that occur during every month.
     * @param numberOfTurns the number of turns to be made.
     * @param consumers the list of consumers
     * @param distributors the list of distributors
     * @param newConsumers a list of list of new consumers for every month
     * @param costsChanges a list of list of cost changes for every month
     */
    void computeTurns(long numberOfTurns, List<ConsumerInput> consumers,
                       List<DistributorInput> distributors,
                       List<List<ConsumerInput>> newConsumers,
                       List<List<CostsChanges>> costsChanges);
}
