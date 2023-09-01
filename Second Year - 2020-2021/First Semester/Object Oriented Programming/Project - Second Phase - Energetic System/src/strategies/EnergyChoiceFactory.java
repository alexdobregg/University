package strategies;

import common.Constants;

public final class EnergyChoiceFactory {
    private static EnergyChoiceFactory instance = null;
    private EnergyChoiceFactory() { }

    /**
     * It does not allow more than one instances of this class to be created.
     * @return instance / new instance if there is a null instance
     */
    public static EnergyChoiceFactory getInstance() {
        if (instance == null) {
            instance = new EnergyChoiceFactory();
        }
        return instance;
    }

    /**
     * Depending on the value of the string, it returns the type of strategy for a distributor.
     * @param strategyType a string that points out the strategy type
     * @return the strategy type a distributor has / error
     */
    public EnergyChoiceStrategyType create(String strategyType) {
        return switch (strategyType) {
            case Constants.GREEN_TP -> EnergyChoiceStrategyType.GREEN;
            case Constants.PRICE_TP -> EnergyChoiceStrategyType.PRICE;
            case Constants.QUANTITY_TP -> EnergyChoiceStrategyType.QUANTITY;
            default -> throw new IllegalStateException("Unexpected value: " + strategyType);
        };
    }
}
