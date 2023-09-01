package input;

public final class ProducerChanges {
    private final int id;
    private final int energyPerDistributor;

    public ProducerChanges(final int id, final int energyPerDistributor) {
        this.id = id;
        this.energyPerDistributor = energyPerDistributor;
    }

    public int getId() {
        return id;
    }

    public int getEnergyPerDistributor() {
        return energyPerDistributor;
    }
}
