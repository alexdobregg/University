package input;

public final class ConsumerInput {
    private final int id;
    private boolean isBankrupt;
    private int budget;
    private final int monthlyIncome;
    private boolean inDebt;
    private int monthsInDebt;
    private int toPayDebt;
    private Contract contract;

    public ConsumerInput(final int id, final int initialBudget, final int monthlyIncome) {
        this.id = id;
        this.budget = initialBudget;
        this.monthlyIncome = monthlyIncome;
        this.isBankrupt = false;
        this.inDebt = false;
        this.monthsInDebt = 0;
        this.contract = null;
        this.toPayDebt = 0;
    }

    public void setBudget(final int budget) {
        this.budget = budget;
    }

    /**
     * A method to set the isBankrupt field to true;
     */
    public void setIsBankrupt() {
        this.isBankrupt = true;
    }

    public int getId() {
        return this.id;
    }

    public int getBudget() {
        return this.budget;
    }

    public int getMonthlyIncome() {
        return this.monthlyIncome;
    }

    public boolean getIsBankrupt() {
        return this.isBankrupt;
    }

    public boolean isInDebt() {
        return inDebt;
    }

    public void setInDebt(final boolean inDebt) {
        this.inDebt = inDebt;
    }

    public int getMonthsInDebt() {
        return monthsInDebt;
    }

    public void setMonthsInDebt(final int monthsInDebt) {
        this.monthsInDebt = monthsInDebt;
    }

    public Contract getContract() {
        return contract;
    }

    public void setContract(final Contract contract) {
        this.contract = contract;
    }

    public int getToPayDebt() {
        return toPayDebt;
    }

    public void setToPayDebt(final int toPayDebt) {
        this.toPayDebt = toPayDebt;
    }

}
