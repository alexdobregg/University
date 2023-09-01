package input;

import common.Constants;
import entities.Consumer;
import entities.Distributor;
import entities.Producer;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public final class InputLoader {
    private final String inputPath;
    public InputLoader(final String inputPath) {
        this.inputPath = inputPath;
    }

    /**
     * A method to obtain number of turns, input data and monthly updates
     * from the input file.
     * @return a new input object
     */
    public Input readData() {
        JSONParser jsonParser = new JSONParser();
        long numberOfTurns = 0;
        List<Consumer> consumers = new ArrayList<>();
        List<Distributor> distributors = new ArrayList<>();
        List<Producer> producers = new ArrayList<>();
        List<List<Consumer>> newConsumers = new ArrayList<>();
        List<List<DistributorChanges>> distributorChanges = new ArrayList<>();
        List<List<ProducerChanges>> producerChanges = new ArrayList<>();
        try {
            // Parsing the contents of the JSON file.
            JSONObject jsonObject = (JSONObject) jsonParser.
                            parse(new FileReader(inputPath));
            // Obtaining the number of turns from the file.
            numberOfTurns =
                    (long) jsonObject.get(Constants.NUMBER_OF_TURNS);
            // Obtaining the initial data from the file.
            readInitialData(jsonObject, consumers, distributors, producers);

            // Obtaining the monthly updates from the file.
            readMonthlyUpdates(jsonObject, newConsumers, distributorChanges, producerChanges);
        } catch (ParseException | IOException e) {
            e.printStackTrace();
        }
        return new Input(numberOfTurns, consumers, distributors, producers,
                        newConsumers, distributorChanges, producerChanges);
    }

    /**
     * It reads the initial data.
     * @param jsonObject the json object used to read the data from the file
     * @param consumers the list of consumers from the file
     * @param distributors the list of distributors from the file
     * @param producers the list of producers from the file
     */
    public void readInitialData(JSONObject jsonObject, List<Consumer> consumers,
                                List<Distributor> distributors, List<Producer> producers) {

        // Obtaining the initial data from the file: consumers, distributors and producers.
        JSONObject jsonInitialData =
                (JSONObject) jsonObject.get(Constants.INITIAL_DATA);

        JSONArray jsonConsumers = (JSONArray) jsonInitialData.
                get(Constants.CONSUMERS);

        JSONArray jsonDistributors = (JSONArray) jsonInitialData.
                get(Constants.DISTRIBUTORS);

        JSONArray jsonProducers = (JSONArray) jsonInitialData.
                get(Constants.PRODUCERS);
        // Consumers.
        if (jsonConsumers != null) {
            for (Object jsonIter : jsonConsumers) {
                consumers.add(new Consumer(
                        Integer.parseInt(((JSONObject) jsonIter).
                                get(Constants.ID).toString()),
                        Integer.parseInt(((JSONObject) jsonIter).
                                get(Constants.INITIAL_BUDGET).toString()),
                        Integer.parseInt(((JSONObject) jsonIter).
                                get(Constants.MONTHLY_INCOME).toString())));
            }
        } else {
            System.out.println("THERE ARE NO CONSUMERS");
        }
        // Distributors.
        if (jsonDistributors != null) {
            for (Object jsonIter : jsonDistributors) {
                distributors.add(new Distributor(
                        Integer.parseInt(((JSONObject) jsonIter).
                                get(Constants.ID).toString()),
                        Integer.parseInt(((JSONObject) jsonIter).
                                get(Constants.CONTRACT_LENGTH).toString()),
                        Integer.parseInt(((JSONObject) jsonIter).
                                get(Constants.INITIAL_BUDGET).toString()),
                        Integer.parseInt(((JSONObject) jsonIter).
                                get(Constants.INITIAL_INFRASTRUCTURE_COST).toString()),
                        Integer.parseInt(((JSONObject) jsonIter).
                                get(Constants.ENERGY_NEEDED_KW).toString()),
                        (((JSONObject) jsonIter).get(Constants.PRODUCER_STRATEGY)).
                                toString()));
            }
        } else {
            System.out.println("THERE ARE NO DISTRIBUTORS");
        }
        // Producers.
        if (jsonProducers != null) {
            for (Object jsonIter : jsonProducers) {
                producers.add(new Producer(
                        Integer.parseInt(((JSONObject) jsonIter).
                                get(Constants.ID).toString()),
                        (((JSONObject) jsonIter).get(Constants.ENERGY_TYPE).
                                toString()),
                        Integer.parseInt(((JSONObject) jsonIter).
                                get(Constants.MAX_DISTRIBUTORS).toString()),
                        Double.parseDouble(((JSONObject) jsonIter).
                                get(Constants.PRICE_KW).toString()),
                        Integer.parseInt(((JSONObject) jsonIter).
                                get(Constants.ENERGY_PER_DISTRIBUTOR).toString())));
            }
        } else {
            System.out.println("THERE ARE NO PRODUCERS");
        }
    }

    /**
     * It reads the monthly updates.
     * @param jsonObject the json object used to read the data from the file
     * @param newConsumers a list of list containing new consumers for every month
     * @param distributorChanges a list of list containing distributors changes for every month
     * @param producerChanges a list of list containing producers changes for every month
     */
    public void readMonthlyUpdates(JSONObject jsonObject, List<List<Consumer>> newConsumers,
                                   List<List<DistributorChanges>> distributorChanges,
                                   List<List<ProducerChanges>> producerChanges) {
        JSONArray jsonMonthlyUpdates = (JSONArray)
                jsonObject.get(Constants.MONTHLY_UPDATES);
        if (jsonMonthlyUpdates != null) {
            for (Object jsonIter : jsonMonthlyUpdates) {
                // It adds null if there are no new consumers in that month,
                // or a list of consumers.
                if (((JSONArray) ((JSONObject) jsonIter).get(Constants.NEW_CONSUMERS)).
                        isEmpty()) {
                    newConsumers.add(null);
                } else {
                    List<Consumer> newCons = new ArrayList<>();
                    for (Object jsonCons : ((JSONArray) ((JSONObject) jsonIter).
                            get(Constants.NEW_CONSUMERS))) {
                        newCons.add(new Consumer(
                                Integer.parseInt(((JSONObject) jsonCons).
                                        get(Constants.ID).toString()),
                                Integer.parseInt(((JSONObject) jsonCons).
                                        get(Constants.INITIAL_BUDGET).toString()),
                                Integer.parseInt(((JSONObject) jsonCons).
                                        get(Constants.MONTHLY_INCOME).toString())));
                    }
                    newConsumers.add(newCons);
                }
                // It adds null if there are no new distributor changes in that month,
                // or a list of distributor changes.
                if ((((JSONArray) ((JSONObject) jsonIter).
                        get(Constants.DISTRIBUTOR_CHANGES)).isEmpty())) {
                    distributorChanges.add(null);
                } else {
                    List<DistributorChanges> newDbChanges = new ArrayList<>();
                    for (Object jsonCons : ((JSONArray) ((JSONObject) jsonIter).
                            get(Constants.DISTRIBUTOR_CHANGES))) {
                        newDbChanges.add(new DistributorChanges(
                                Integer.parseInt(((JSONObject) jsonCons).
                                        get(Constants.ID).toString()),
                                Integer.parseInt(((JSONObject) jsonCons).
                                        get(Constants.INFRASTRUCTURE_COST).toString())));
                    }
                    distributorChanges.add(newDbChanges);
                }
                // It adds null if there are no new producer changes in that month,
                // or a list of producer changes.
                if ((((JSONArray) ((JSONObject) jsonIter).
                        get(Constants.PRODUCER_CHANGES)).isEmpty())) {
                    producerChanges.add(null);
                } else {
                    List<ProducerChanges> newPdChanges = new ArrayList<>();
                    for (Object jsonCons : ((JSONArray) ((JSONObject) jsonIter).
                            get(Constants.PRODUCER_CHANGES))) {
                        newPdChanges.add(new ProducerChanges(
                                Integer.parseInt(((JSONObject) jsonCons).
                                        get(Constants.ID).toString()),
                                Integer.parseInt(((JSONObject) jsonCons).
                                        get(Constants.ENERGY_PER_DISTRIBUTOR).toString())));
                    }
                    producerChanges.add(newPdChanges);
                }
            }
        } else {
            System.out.println("THERE ARE NO MONTHLY UPDATES");
        }
    }
}
