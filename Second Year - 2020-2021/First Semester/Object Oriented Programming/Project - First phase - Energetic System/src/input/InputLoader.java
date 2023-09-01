package input;

import common.Constants;
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
        List<ConsumerInput> consumers = new ArrayList<>();
        List<DistributorInput> distributors = new ArrayList<>();
        List<List<ConsumerInput>> newConsumers = new ArrayList<>();
        List<List<CostsChanges>> costsChanges = new ArrayList<>();
        try {
            // Parsing the contents of the JSON file.
            JSONObject jsonObject = (JSONObject) jsonParser.
                            parse(new FileReader(inputPath));
            // Obtaining the number of turns from the file.
            numberOfTurns =
                    (long) jsonObject.get(Constants.NUMBER_OF_TURNS);
            // Obtaining the initial data from the file: consumers and distributors.
            JSONObject jsonInitialData =
                    (JSONObject) jsonObject.get(Constants.INITIAL_DATA);

            JSONArray jsonConsumers = (JSONArray) jsonInitialData.
                                    get(Constants.CONSUMERS);

            JSONArray jsonDistributors = (JSONArray) jsonInitialData.
                                    get(Constants.DISTRIBUTORS);
            if (jsonConsumers != null) {
                for (Object jsonIter : jsonConsumers) {
                    consumers.add(new ConsumerInput(
                            Integer.parseInt(((JSONObject) jsonIter).
                                    get(Constants.ID).toString()),
                            Integer.parseInt(((JSONObject) jsonIter).
                                    get(Constants.INITIAL_BUDGET).toString()),
                            Integer.parseInt(((JSONObject) jsonIter).
                                    get(Constants.MONTHLY_INCOME).toString())));
                }
            } else {
                System.out.printf("THERE ARE NO CONSUMERS");
            }

            if (jsonDistributors != null) {
                for (Object jsonIter : jsonDistributors) {
                    distributors.add(new DistributorInput(
                            Integer.parseInt(((JSONObject) jsonIter).
                                    get(Constants.ID).toString()),
                            Integer.parseInt(((JSONObject) jsonIter).
                                    get(Constants.CONTRACT_LENGTH).toString()),
                            Integer.parseInt(((JSONObject) jsonIter).
                                    get(Constants.INITIAL_BUDGET).toString()),
                            Integer.parseInt(((JSONObject) jsonIter).
                                    get(Constants.INITIAL_INFRASTRUCTURE_COST).toString()),
                            Integer.parseInt(((JSONObject) jsonIter).
                                    get(Constants.INITIAL_PRODUCTION_COST).toString())));
                }
            } else {
                System.out.printf("THERE ARE NO DISTRIBUTORS");
            }
            // Obtaining the monthly updates from the file.
            JSONArray jsonMonthlyUpdates = (JSONArray)
                    jsonObject.get(Constants.MONTHLY_UPDATES);
            if (jsonMonthlyUpdates != null) {
                for (Object jsonIter : jsonMonthlyUpdates) {
                    if (((JSONArray) ((JSONObject) jsonIter).get(Constants.NEW_CONSUMERS)).
                                                                                isEmpty()) {
                        newConsumers.add(null);
                    } else {
                        List<ConsumerInput> newCons = new ArrayList<>();
                        for (Object jsonCons : ((JSONArray) ((JSONObject) jsonIter).
                                get(Constants.NEW_CONSUMERS))) {
                            newCons.add(new ConsumerInput(
                                    Integer.parseInt(((JSONObject) jsonCons).
                                            get(Constants.ID).toString()),
                                    Integer.parseInt(((JSONObject) jsonCons).
                                            get(Constants.INITIAL_BUDGET).toString()),
                                    Integer.parseInt(((JSONObject) jsonCons).
                                            get(Constants.MONTHLY_INCOME).toString())));
                        }
                        newConsumers.add(newCons);
                    }
                    if ((((JSONArray) ((JSONObject) jsonIter).
                            get(Constants.COSTS_CHANGES)).isEmpty())) {
                        costsChanges.add(null);
                    } else {
                        List<CostsChanges> newCosts = new ArrayList<>();
                        for (Object jsonCons : ((JSONArray) ((JSONObject) jsonIter).
                                get(Constants.COSTS_CHANGES))) {
                            newCosts.add(new CostsChanges(
                                    Integer.parseInt(((JSONObject) jsonCons).
                                            get(Constants.ID).toString()),
                                    Integer.parseInt(((JSONObject) jsonCons).
                                            get(Constants.INFRASTRUCTURE_COST).toString()),
                                    Integer.parseInt(((JSONObject) jsonCons).
                                            get(Constants.PRODUCTION_COST).toString())));
                        }
                        costsChanges.add(newCosts);
                    }
                }
            } else {
                System.out.println("THERE ARE NO MONTHLY UPDATES");
            }
        } catch (ParseException | IOException e) {
            e.printStackTrace();
        }
        return new Input(numberOfTurns, consumers, distributors, newConsumers, costsChanges);
    }
}
