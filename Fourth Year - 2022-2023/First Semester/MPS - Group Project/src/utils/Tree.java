package utils;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class Tree {
    private final List<List<Integer>> threshIdx;
    private final List<List<Integer>> operIdx;

    public Tree() {
        this.threshIdx = new ArrayList<>();
        this.operIdx = new ArrayList<>();
    }

    public List<List<Integer>> getThreshIdx() {
        return threshIdx;
    }

    public List<List<Integer>> getOperIdx() {
        return operIdx;
    }

    /**
     * Method to shuffle the indexes of both operators and thresholds.
     * @param length1 length of the operators
     * @param length2 length of the thresholds
     */
    public void ourShuffle(int length1, int length2) {
        List<Integer> oper = Stream.iterate(0, n -> n + 1)
                .limit(length1).collect(Collectors.toList());
        List<Integer> thresh = Stream.iterate(0, n -> n + 1)
                .limit(length2).collect(Collectors.toList());
        Collections.shuffle(oper);
        Collections.shuffle(thresh);
        this.threshIdx.add(thresh);
        this.operIdx.add(oper);
    }
}
