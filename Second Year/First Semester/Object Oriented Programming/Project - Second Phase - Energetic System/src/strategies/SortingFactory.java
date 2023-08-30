package strategies;

import common.Constants;

public final class SortingFactory {
    private static SortingFactory instance = null;
    private SortingFactory() { }

    /**
     * It does not allow more than one instances of this class to be created.
     * @return instance / new instance if there is a null instance
     */
    public static SortingFactory getInstance() {
        if (instance == null) {
            instance = new SortingFactory();
        }
        return instance;
    }

    /**
     * Based on the type string, it creates a new object of the specific strategy class.
     * @param type a string that points out the type of strategy a distributor has
     * @return a strategy to sort the producers.
     */
    public SortingStrategy create(final String type) {
        return switch (type) {
            case (Constants.GREEN_TP) -> new GreenSortingStrategy();
            case (Constants.PRICE_TP) -> new PriceSortingStrategy();
            case (Constants.QUANTITY_TP) -> new QuantitySortingStrategy();
            default ->  throw new IllegalStateException("Unexpected value: " + type);
        };
    }
}
