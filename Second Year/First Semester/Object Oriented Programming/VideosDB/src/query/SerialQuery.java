package query;

import entertainment.Season;
import fileio.ActionInputData;
import fileio.SerialInputData;
import fileio.UserInputData;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class SerialQuery extends Sort {
    /**
     * A method to check whether a serial respects the filters or not.
     * @param action the action
     * @param serial the serial
     * @return 1/0
     */
    public static int getOk(final ActionInputData action, final SerialInputData serial) {
        int ok = 0;
        if (action.getFilters().get(0).contains(null)) {
            if (action.getFilters().get(1).contains(null)) {
                ok = 1;
            } else {
                if (serial.getGenres().containsAll(action.getFilters().get(1))) {
                    ok = 1;
                }
            }
        } else {
            if (action.getFilters().get(1).contains(null)) {
                if (action.getFilters().get(0).contains("" + serial.getYear())) {
                    ok = 1;
                }
            } else {
                if (action.getFilters().get(0).contains("" + serial.getYear())) {
                    if (serial.getGenres().containsAll(action.getFilters().get(1))) {
                        ok = 1;
                    }
                }
            }
        }
        return ok;
    }

    /**
     * A method that calculates how many times a serial that respects the filters
     * appears in the favorite list of users. Then it sorts the serials based on that
     * number. If they have the same number of views, they are sorted by their
     * names.
     * @param users the list of users
     * @param action the action
     * @param serials the list of serials
     * @return favorite query message
     */
    public static String getFavoriteMessage(final List<UserInputData> users,
                                            final ActionInputData action,
                                            final List<SerialInputData> serials) {
        String favoriteMessage;
        Map<String, Double> serialsUnsorted = new HashMap<>();
        List<String> serialsSorted = new ArrayList<>();
        for (SerialInputData serial : serials) {
            double nrAp = 0.0;
            if (getOk(action, serial) == 1) {
                for (UserInputData user : users) {
                    if (user.getFavoriteMovies().contains(serial.getTitle())) {
                        nrAp++;
                    }
                }
                if (nrAp != 0.0) {
                    serialsUnsorted.put(serial.getTitle(), nrAp);
                }
            }
        }
        getSortedList(serialsUnsorted, action, serialsSorted);
        favoriteMessage = "Query result: " + serialsSorted;
        return favoriteMessage;
    }

    /**
     * A method that calculates the length of every serial that respects the
     * filters, sorting them by that length. If they have the same length,
     * they are sorted by their names.
     * @param action the action
     * @param serials the list of movies
     * @return longest query message
     */
    public static String getLongestMessage(final ActionInputData action,
                                           final List<SerialInputData> serials) {
        String longestMessage;
        Map<String, Double> serialsUnsorted = new HashMap<>();
        List<String> serialsSorted = new ArrayList<>();
        for (SerialInputData serial : serials) {
            if (getOk(action, serial) == 1) {
                double dur = 0.0;
                for (Season season : serial.getSeasons()) {
                    dur += season.getDuration();
                }
                serialsUnsorted.put(serial.getTitle(), dur);
            }
        }
        getSortedList(serialsUnsorted, action, serialsSorted);
        longestMessage = "Query result: " + serialsSorted;
        return longestMessage;
    }

    /**
     * A method that calculates how many views does a serial that respects
     * the filters have, sorting them by that number of views. If they have
     * the same number of views, the serials are sorted by their names.
     * @param users the list of users
     * @param action the action
     * @param serials the list of movies
     * @return most viewed query message
     */
    public static String getMvMessage(final List<UserInputData> users,
                                      final ActionInputData action,
                                      final List<SerialInputData> serials) {
        String mvMessage;
        Map<String, Double> serialsUnsorted = new HashMap<>();
        List<String> serialsSorted = new ArrayList<>();
        for (SerialInputData serial : serials) {
            double nrViews = 0.0;
            if (getOk(action, serial) == 1) {
                for (UserInputData user : users) {
                    if (user.getHistory().containsKey(serial.getTitle())) {
                        for (Map.Entry<String, Integer> entry : user.getHistory().entrySet()) {
                            if (entry.getKey().equals(serial.getTitle())) {
                                nrViews += entry.getValue();
                                break;
                            }
                        }
                    }
                }
                if (nrViews != 0) {
                    serialsUnsorted.put(serial.getTitle(), nrViews);
                }
            }
        }
        getSortedList(serialsUnsorted, action, serialsSorted);
        mvMessage = "Query result: " + serialsSorted;
        return mvMessage;
    }

    /**
     * A method that calculates the rating of every serial that respects the filters,
     * sorting them by their rating. If they have the same rating, the serials are
     * sorted by their names.
     * @param users the list of users
     * @param action the action
     * @param serials the list of movies
     * @return rating query message
     */
    public static String getRatingMessage(final List<UserInputData> users,
                                          final ActionInputData action,
                                          final List<SerialInputData> serials) {
        String ratingMessage;
        Map<String, Double> serialsUnsorted = new HashMap<>();
        List<String> serialsSorted = new ArrayList<>();
        for (SerialInputData serial : serials) {
            if (getOk(action, serial) == 1) {
                double ratingSerial;
                double rating;
                double count = 0.0;
                double sum2 = 0.0;
                for (UserInputData user : users) {
                    if (user.getSerialRating().containsKey(serial.getTitle())) {
                        for (Map.Entry<String, ArrayList<Double>> entry
                                                        : user.getSerialRating().entrySet()) {
                            if (entry.getKey().equals(serial.getTitle())) {
                                double sum = 0;
                                for (Double sezRat : entry.getValue()) {
                                    sum += sezRat;
                                }
                                if (!entry.getValue().isEmpty()) {
                                    ratingSerial = sum / entry.getValue().size();
                                } else {
                                    ratingSerial = 0.0;
                                }
                                sum2 += ratingSerial;
                                count++;
                                break;
                            }
                        }
                    }
                }
                if (count == 0) {
                    rating = 0.0;
                } else {
                    rating = sum2 / count;
                }
                if (rating != 0.0) {
                    serialsUnsorted.put(serial.getTitle(), rating);
                }
            }
        }
        getSortedList(serialsUnsorted, action, serialsSorted);
        ratingMessage = "Query result: " + serialsSorted;
        return ratingMessage;
    }
}
