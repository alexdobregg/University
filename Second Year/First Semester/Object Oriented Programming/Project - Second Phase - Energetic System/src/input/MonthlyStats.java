package input;

import java.util.List;

public final class MonthlyStats {
    private final int month;
    private final List<Integer> distributorsIds;

    public MonthlyStats(final int month, final List<Integer> distributorsIds) {
        this.month = month;
        this.distributorsIds = distributorsIds;
    }

    public int getMonth() {
        return month;
    }

    public List<Integer> getDistributorsIds() {
        return distributorsIds;
    }
}
