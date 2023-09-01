package output;

import com.fasterxml.jackson.core.util.DefaultPrettyPrinter;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.ObjectWriter;
import java.io.IOException;
import java.nio.file.Paths;
import java.util.List;

public final class Writer {
    private final List<OutputConsumer> consumers;
    private final List<OutputDistributor> distributors;
    private final List<OutputProducer> energyProducers;

    public Writer(final List<OutputConsumer> consumersOut,
                  final List<OutputDistributor> distributorOut,
                  final List<OutputProducer> energyProducers) throws IOException {
        this.consumers = consumersOut;
        this.distributors = distributorOut;
        this.energyProducers = energyProducers;
    }

    public List<OutputConsumer> getConsumers() {
        return consumers;
    }

    public List<OutputDistributor> getDistributors() {
        return distributors;
    }

    public List<OutputProducer> getEnergyProducers() {
        return energyProducers;
    }

    /**
     * A method that writes the output in a file.
     * @param filePath the path to the output file
     */
    public void closeJSON(final String filePath) {
        try {
            ObjectMapper mapper = new ObjectMapper();
            ObjectWriter objectWriter = mapper.writer(new DefaultPrettyPrinter());
            objectWriter.writeValue(Paths.get(filePath).toFile(), this);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
