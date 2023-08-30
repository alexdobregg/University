import common.Constants;
import input.ConsumerInput;
import input.CostsChanges;
import input.DistributorInput;
import input.InputLoader;
import input.Input;
import output.OutputConsumer;
import output.OutputDistributor;
import output.Writer;
import turns.Turns;
import turns.TurnsFactory;


import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public final class Main {
    private Main() {
    }

    /**
     * Used in the main checker.
     * @param args from command line
     * @throws IOException in case of exceptions to reading / writing
     */
    public static void main(final String[] args) throws IOException {
        String filepath = args[0];
        // Reading from the input file.
        InputLoader inputLoader = new InputLoader(filepath);
        Input input = inputLoader.readData();
        long numberOfTurns = input.getNumberOfTurns();
        List<ConsumerInput> consumers = input.getConsumers();
        List<DistributorInput> distributors = input.getDistributors();
        List<List<ConsumerInput>> newConsumers = input.getNewConsumers();
        List<List<CostsChanges>> costsChanges = input.getCostsChanges();
        // THE FIRST TURN
        Turns turns = TurnsFactory.getInstance().create(Constants.FIRST_TURN);
        turns.computeTurns(numberOfTurns, consumers, distributors, newConsumers, costsChanges);
        // THE OTHER TURNS.
        turns = TurnsFactory.getInstance().create(Constants.OTHER_TURNS);
        turns.computeTurns(numberOfTurns, consumers, distributors, newConsumers, costsChanges);
        // Saving the fields we need in another classes.
        List<OutputConsumer> outConsumers = new ArrayList<>();
        List<OutputDistributor> outDistributors = new ArrayList<>();
        for (ConsumerInput consumer : consumers) {
            outConsumers.add(new OutputConsumer(consumer.getId(), consumer.getIsBankrupt(),
                                                                    consumer.getBudget()));
        }
        for (DistributorInput distributor : distributors) {
            outDistributors.add(new OutputDistributor(distributor.getId(),
                    distributor.getBudget(), distributor.getIsBankrupt(),
                                            distributor.getContracts()));
        }
        // Writing to the output file.
        Writer fileWriter = new Writer(outConsumers, outDistributors);
        fileWriter.closeJSON(args[1]);
    }
}
