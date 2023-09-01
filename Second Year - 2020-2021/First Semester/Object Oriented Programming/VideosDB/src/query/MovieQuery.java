package query;

import fileio.ActionInputData;
import fileio.MovieInputData;
import fileio.UserInputData;

import java.util.List;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

public class MovieQuery extends Sort {
    /**
     * A method to check whether a movie respects the filters or not.
     * @param action the action
     * @param movie the movie
     * @return 1/0
     */
    public static int getOk(final ActionInputData action, final MovieInputData movie) {
        int ok = 0;
        if (action.getFilters().get(0).contains(null)) {
            if (action.getFilters().get(1).contains(null)) {
                ok = 1;
            } else {
                if (movie.getGenres().containsAll(action.getFilters().get(1))) {
                    ok = 1;
                }
            }
        } else {
            if (action.getFilters().get(1).contains(null)) {
                if (action.getFilters().get(0).contains("" + movie.getYear())) {
                    ok = 1;
                }
            } else {
                if (action.getFilters().get(0).contains("" + movie.getYear())) {
                    if (movie.getGenres().containsAll(action.getFilters().get(1))) {
                        ok = 1;
                    }
                }
            }
        }
        return ok;
    }

    /**
     * A method that calculates how many times a movie that respects the filters
     * appears in the favorite list of users. Then it sorts the movies based on that
     * number. If they have the same number of views, they are sorted by their
     * names.
     * @param users the list of users
     * @param action the action
     * @param movies the list of movies
     * @return favorite query message
     */
    public static String getFavoriteMessage(final List<UserInputData> users,
                                            final ActionInputData action,
                                            final List<MovieInputData> movies) {
        String favoriteMessage;
        Map<String, Double> moviesUnsorted = new HashMap<>();
        List<String> moviesSorted = new ArrayList<>();
        for (MovieInputData movie : movies) {
            double nrAp = 0.0;
            if (getOk(action, movie) == 1) {
                for (UserInputData user : users) {
                    if (user.getFavoriteMovies().contains(movie.getTitle())) {
                        nrAp++;
                    }
                }
                if (nrAp != 0.0) {
                    moviesUnsorted.put(movie.getTitle(), nrAp);
                }
            }
        }
        getSortedList(moviesUnsorted, action, moviesSorted);
        favoriteMessage = "Query result: " + moviesSorted;
        return favoriteMessage;
    }

    /**
     * A method that calculates the length of every movie that respects the
     * filters, sorting them by that length. If they have the same length,
     * they are sorted by their names.
     * @param action the action
     * @param movies the list of movies
     * @return longest query message
     */
    public static String getLongestMessage(final ActionInputData action,
                                           final List<MovieInputData> movies) {
        String longestMessage;
        Map<String, Double> moviesUnsorted = new HashMap<>();
        List<String> moviesSorted = new ArrayList<>();
        for (MovieInputData movie : movies) {
            if (getOk(action, movie) == 1) {
                moviesUnsorted.put(movie.getTitle(), (double) movie.getDuration());
            }
        }
        getSortedList(moviesUnsorted, action, moviesSorted);
        longestMessage = "Query result: " + moviesSorted;
        return longestMessage;
    }

    /**
     * A method that calculates how many views does a movie that respects
     * the filters have, sorting them by that number of views. If they have
     * the same number of views, the movies are sorted by their names.
     * @param users the list of users
     * @param action the action
     * @param movies the list of movies
     * @return most viewed query message
     */
    public static String getMvMessage(final List<UserInputData> users,
                                      final ActionInputData action,
                                      final List<MovieInputData> movies) {
        String mvMessage;
        Map<String, Double> moviesUnsorted = new HashMap<>();
        List<String> moviesSorted = new ArrayList<>();
        for (MovieInputData movie : movies) {
            double nrViews = 0.0;
            if (getOk(action, movie) == 1) {
                for (UserInputData user : users) {
                    if (user.getHistory().containsKey(movie.getTitle())) {
                        for (Map.Entry<String, Integer> entry : user.getHistory().entrySet()) {
                            if (entry.getKey().equals(movie.getTitle())) {
                                nrViews += entry.getValue();
                                break;
                            }
                        }
                    }
                }
                if (nrViews != 0.0) {
                    moviesUnsorted.put(movie.getTitle(), nrViews);
                }
            }
        }
        getSortedList(moviesUnsorted, action, moviesSorted);
        mvMessage = "Query result: " + moviesSorted;
        return mvMessage;
    }

    /**
     * A method that calculates the rating of every movie that respects the filters,
     * sorting them by their rating. If they have the same rating, the movies are
     * sorted by their names.
     * @param users the list of users
     * @param action the action
     * @param movies the list of movies
     * @return rating query message
     */
    public static String getRatingMessage(final List<UserInputData> users,
                                          final ActionInputData action,
                                          final List<MovieInputData> movies) {
        String ratingMessage;
        Map<String, Double> moviesUnsorted = new HashMap<>();
        List<String> moviesSorted = new ArrayList<>();
        for (MovieInputData movie : movies) {
            if (getOk(action, movie) == 1) {
                double count = 0.0;
                double sum = 0.0;
                double rating;
                for (UserInputData user : users) {
                    if (user.getMoviesRating().containsKey(movie.getTitle())) {
                        for (Map.Entry<String, Double> entry : user.getMoviesRating().entrySet()) {
                            if (entry.getKey().equals(movie.getTitle())) {
                                sum += entry.getValue();
                                count++;
                                break;
                            }
                        }
                    }
                }
                if (count != 0) {
                    rating = sum / count;
                } else {
                    rating = 0.0;
                }
                if (rating != 0.0) {
                    moviesUnsorted.put(movie.getTitle(), rating);
                }
            }
        }
        getSortedList(moviesUnsorted, action, moviesSorted);
        ratingMessage = "Query result: " + moviesSorted;
        return ratingMessage;
    }
}
