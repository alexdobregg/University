import common.Constants;

import entities.Consumer;
import input.DistributorChanges;
import entities.Distributor;
import input.Input;
import input.InputLoader;
import input.ProducerChanges;
import entities.Producer;

import output.OutputConsumer;
import output.OutputDistributor;
import output.OutputProducer;
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
        List<Consumer> consumers = input.getConsumers();
        List<Distributor> distributors = input.getDistributors();
        List<Producer> producers = input.getProducers();
        List<List<Consumer>> newConsumers = input.getNewConsumers();
        List<List<DistributorChanges>> distributorChanges = input.getDistributorChanges();
        List<List<ProducerChanges>> producerChanges = input.getProducerChanges();

        // THE FIRST TURN.
        Turns turns = TurnsFactory.getInstance().create(Constants.FIRST_TURN);
        turns.computeTurns(numberOfTurns, consumers, distributors, producers,
                            newConsumers, distributorChanges, producerChanges);

        // THE OTHER TURNS.
        turns = TurnsFactory.getInstance().create(Constants.OTHER_TURNS);
        turns.computeTurns(numberOfTurns, consumers, distributors, producers,
                            newConsumers, distributorChanges, producerChanges);

        // Saving the fields we need in another classes.
        List<OutputConsumer> outConsumers = new ArrayList<>();
        List<OutputDistributor> outDistributors = new ArrayList<>();
        List<OutputProducer> outProducers = new ArrayList<>();

        for (Consumer consumer : consumers) {
            outConsumers.add(new OutputConsumer(consumer.getId(), consumer.getIsBankrupt(),
                    consumer.getBudget()));
        }

        for (Distributor distributor : distributors) {
            outDistributors.add(new OutputDistributor(distributor.getId(),
                    distributor.getEnergyNeededKW(), distributor.getContractCost(),
                    distributor.getBudget(), distributor.getProducerStrategy(),
                    distributor.getIsBankrupt(), distributor.getContracts()));
        }

        for (Producer producer : producers) {
            outProducers.add(new OutputProducer(producer.getId(),
                    producer.getMaxDistributors(), producer.getPriceKW(),
                    producer.getEnergyType(), producer.getEnergyPerDistributor(),
                    producer.getMonthlyStats()));
        }

        // Writing to the output file.
        Writer fileWriter = new Writer(outConsumers, outDistributors, outProducers);
        fileWriter.closeJSON(args[1]);

    }
}

