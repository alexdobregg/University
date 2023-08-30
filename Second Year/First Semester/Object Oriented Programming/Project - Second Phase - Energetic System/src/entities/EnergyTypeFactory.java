package entities;

import common.Constants;

public final class EnergyTypeFactory {
    private static EnergyTypeFactory instance = null;
    private EnergyTypeFactory() { }

    /**
     * It does not allow more than one instances of this class to be created.
     * @return instance / new instance if there is a null instance
     */
    public static EnergyTypeFactory getInstance() {
        if (instance == null) {
            instance = new EnergyTypeFactory();
        }
        return instance;
    }

    /**
     * Depending on the value of the string, it returns the energy type for a producer.
     * @param energyType a string that points out the energy type
     * @return the energy type a producer has to offer / error
     */
    public EnergyType create(final String energyType) {
        return switch (energyType) {
            case Constants.WIND_EN -> EnergyType.WIND;
            case Constants.SOLAR_EN -> EnergyType.SOLAR;
            case Constants.HYDRO_EN -> EnergyType.HYDRO;
            case Constants.COAL_EN -> EnergyType.COAL;
            case Constants.NUCLEAR_EN -> EnergyType.NUCLEAR;
            default -> throw new IllegalStateException("Unexpected value: " + energyType);
        };
    }
}
