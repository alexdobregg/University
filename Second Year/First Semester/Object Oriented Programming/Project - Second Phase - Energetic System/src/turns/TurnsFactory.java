package turns;
import common.Constants;

public final class TurnsFactory {
    private static TurnsFactory instance = null;
    private TurnsFactory() { }

    /**
     * It does not allow more than one instances of this class to be created.
     * @return instance / new instance if there is a null instance
     */
    public static TurnsFactory getInstance() {
        if (instance == null) {
            instance = new TurnsFactory();
        }
        return instance;
    }

    /**
     * Based on the turn type (first turn or other turns), it creates
     * a new object specific to that class.
     * @param turnType the type of turn
     * @return a new Turns object
     */
    public Turns create(final String turnType) {
        return switch (turnType) {
            case Constants.FIRST_TURN -> new FirstTurn();
            case Constants.OTHER_TURNS -> new OtherTurns();
            default -> throw new IllegalStateException("Unexpected value: " + turnType);
        };
    }
}
