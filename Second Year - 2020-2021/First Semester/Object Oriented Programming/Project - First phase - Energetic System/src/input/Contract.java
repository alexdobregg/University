package input;

public final class Contract {
    private final int consumerId;
    private final int price;
    private int remainedContractMonths;

    public Contract(final int consumerId, final int price, final int contractMonths) {
        this.consumerId = consumerId;
        this.price = price;
        this.remainedContractMonths = contractMonths;
    }

    public int getConsumerId() {
        return consumerId;
    }

    public int getPrice() {
        return price;
    }

    public int getRemainedContractMonths() {
        return remainedContractMonths;
    }

    public void setRemainedContractMonths(final int remainedContractMonths) {
        this.remainedContractMonths = remainedContractMonths;
    }
}
