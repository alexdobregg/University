package query;

import actor.ActorsAwards;
import fileio.ActionInputData;
import fileio.ActorInputData;
import fileio.UserInputData;

import java.util.List;
import java.util.ArrayList;
import java.util.Map;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Collections;

import static common.Constants.FILTER_AW;

public class ActorQuery extends Sort {
    /**
     * A method to check whether an actor has the awards from the action or not.
     * @param action the action
     * @param actor the actor
     * @return 1/0
     */
    public static int getOkAw(final ActionInputData action, final ActorInputData actor) {
        int ok = 0;
        if (!action.getFilters().get(FILTER_AW).contains(null)) {
            List<String> awards = new ArrayList<>();
            for (Map.Entry<ActorsAwards, Integer> entry : actor.getAwards().entrySet()) {
                awards.add(entry.getKey().toString());
            }
            if (awards.containsAll(action.getFilters().get(FILTER_AW))) {
                ok = 1;
            }
        } else {
            ok = 1;
        }
        return ok;
    }

    /**
     * A method to check if the career description of an actor contains the filter words.
     * @param action the action
     * @param actor the actor
     * @return 1/0
     */
    public static int getOkFd(final ActionInputData action, final ActorInputData actor) {
        int ok = 0;
        String[] arrOfStr = actor.getCareerDescription().split("\\W+");
        List<String> arr = new ArrayList<>(Arrays.asList(arrOfStr));
        List<String> arrayActor = new ArrayList<>();
        List<String> arrayAction = new ArrayList<>();
        for (String ar : arr) {
            arrayActor.add(ar.toUpperCase());
        }
        for (String ar2 : action.getFilters().get(2)) {
            arrayAction.add(ar2.toUpperCase());
        }
        if (arrayActor.containsAll(arrayAction)) {
            ok = 1;
        }
        return ok;
    }

    /**
     * A method to sort the actors that have all the awards by their number of awards.
     * If they have the same amount of awards, they are sorted by their names.
     * @param action the action
     * @param actors the list of actors
     * @return awards query message
     */
    public static String getAwardsMessage(final ActionInputData action,
                                          final List<ActorInputData> actors) {
        String awardsMessage;
        Map<String, Double> actorsUnsorted = new HashMap<>();
        List<String> actorsSorted = new ArrayList<>();
        for (ActorInputData actor : actors) {
            int ok = getOkAw(action, actor);
            if (ok == 1) {
                double nrAwards = 0.0;
                for (Map.Entry<ActorsAwards, Integer> entry : actor.getAwards().entrySet()) {
                    nrAwards += entry.getValue();
                }
                if (nrAwards != 0) {
                    actorsUnsorted.put(actor.getName(), nrAwards);
                }

            }
        }
        getSortedList(actorsUnsorted, action, actorsSorted);
        awardsMessage = "Query result: " + actorsSorted;
        return awardsMessage;
    }

    /**
     * A method that calculates the movies/serials' rating in which an actor have played,
     * using a map to store the name of the actor and his rating. The map is sorted
     * by the rating. If they have the same rating, it is sorted by their name.
     * @param users the list of users
     * @param action the action
     * @param actors the list of actors
     * @return average query message
     */
    public static String getAverageMessage(final List<UserInputData> users,
                                           final ActionInputData action,
                                           final List<ActorInputData> actors) {
        String averageMessage;
        Map<String, Double> actorsUnsorted = new HashMap<>();
        List<String> actorsSorted = new ArrayList<>();
        for (ActorInputData actor : actors) {
            double rating;
            double sum = 0.0;
            double count = 0.0;
            for (String video : actor.getFilmography()) {
                for (UserInputData user : users) {
                    if (user.getMoviesRating().containsKey(video)) {
                        for (Map.Entry<String, Double> entry : user.getMoviesRating().entrySet()) {
                            if (entry.getKey().equals(video)) {
                                sum += entry.getValue();
                                count++;
                                break;
                            }
                        }
                    } else if (user.getSerialRating().containsKey(video)) {
                        for (Map.Entry<String, ArrayList<Double>> entry
                                                            : user.getSerialRating().entrySet()) {
                            if (entry.getKey().equals(video)) {
                                double sum2 = 0.0;
                                double ratingSer;
                                for (Double sezRat : entry.getValue()) {
                                    sum2 += sezRat;
                                }
                                if (!entry.getValue().isEmpty()) {
                                    ratingSer = sum2 / entry.getValue().size();
                                } else {
                                    ratingSer = 0.0;
                                }
                                sum += ratingSer;
                                count++;
                                break;
                            }
                        }
                    }
                }
            }
            if (count == 0) {
                rating = 0.0;
            } else {
                rating = sum / count;
            }
            if (rating != 0) {
                actorsUnsorted.put(actor.getName(), rating);
            }
        }
        getSortedList(actorsUnsorted, action, actorsSorted);
        averageMessage = "Query result: " + actorsSorted;
        return averageMessage;
    }

    /**
     * A method that builds the list of actors if their career description contains
     * the key words from the filter. The list is sorted by their name.
     * @param action the action
     * @param actors the list of actors
     * @return filter description query message
     */
    public static String getFdMessage(final ActionInputData action,
                                      final List<ActorInputData> actors) {
        String fdMessage;
        List<String> actorsSorted = new ArrayList<>();
        for (ActorInputData actor : actors) {
            int ok = getOkFd(action, actor);
            if (ok == 1) {
                actorsSorted.add(actor.getName());
            }
        }
        if (action.getSortType().equals("asc")) {
            Collections.sort(actorsSorted);
        } else {
            Collections.sort(actorsSorted, Collections.reverseOrder());
        }
        fdMessage = "Query result: " + actorsSorted;
        return fdMessage;
    }
}
