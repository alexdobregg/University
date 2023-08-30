package query;

import fileio.ActionInputData;

import java.util.List;
import java.util.Map;
import java.util.LinkedHashMap;
import java.util.Comparator;

public abstract class Sort {
    /**
     * A method that sorts by key and value, based on the type of sorting from the action.
     * @param unsortedMap the map that needs to be sorted
     * @param action the action
     * @param sortedEnt to store the sorted List.
     */
    public static void getSortedList(final Map<String, Double> unsortedMap,
                                     final ActionInputData action,
                                     final List<String> sortedEnt) {
        if (action.getSortType().equals("asc")) {
            LinkedHashMap<String, Double> sortedMap = new LinkedHashMap<>();
            LinkedHashMap<String, Double> sortedMap2 = new LinkedHashMap<>();
            unsortedMap.entrySet()
                    .stream()
                    .sorted(Map.Entry.comparingByKey())
                    .forEachOrdered(x -> sortedMap.put(x.getKey(), x.getValue()));
            sortedMap.entrySet()
                    .stream()
                    .sorted(Map.Entry.comparingByValue())
                    .forEachOrdered(x -> sortedMap2.put(x.getKey(), x.getValue()));
            int count = 0;
            for (Map.Entry<String, Double> sort : sortedMap2.entrySet()) {
                if (action.getNumber() == 0) {
                    sortedEnt.add(sort.getKey());
                } else {
                    if (count < action.getNumber()) {
                        sortedEnt.add(sort.getKey());
                        count++;
                    }
                }
            }
        } else {
            LinkedHashMap<String, Double> reverseSortedMap = new LinkedHashMap<>();
            LinkedHashMap<String, Double> reverseSortedMap2 = new LinkedHashMap<>();
            unsortedMap.entrySet()
                    .stream()
                    .sorted(Map.Entry.comparingByKey(Comparator.reverseOrder()))
                    .forEachOrdered(x -> reverseSortedMap.put(x.getKey(), x.getValue()));
            reverseSortedMap.entrySet()
                    .stream()
                    .sorted(Map.Entry.comparingByValue(Comparator.reverseOrder()))
                    .forEachOrdered(x -> reverseSortedMap2.put(x.getKey(), x.getValue()));
            int count = 0;
            for (Map.Entry<String, Double> sort : reverseSortedMap2.entrySet()) {
                if (action.getNumber() == 0) {
                    sortedEnt.add(sort.getKey());
                } else {
                    if (count < action.getNumber()) {
                        sortedEnt.add(sort.getKey());
                        count++;
                    }
                }
            }
        }
    }
}
