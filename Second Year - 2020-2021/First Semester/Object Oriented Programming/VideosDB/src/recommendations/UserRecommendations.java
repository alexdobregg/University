package recommendations;

import fileio.ActionInputData;
import fileio.MovieInputData;
import fileio.SerialInputData;
import fileio.UserInputData;

import java.util.List;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.LinkedHashMap;
import java.util.Comparator;

public final class UserRecommendations {
    private UserRecommendations() {
    }

    /**
     * A method that checks whether a user has a premium subscription or not.
     * @param users the list of users
     * @param action the action
     * @return 1/0
     */
    public static int getOk(final List<UserInputData> users, final ActionInputData action) {
        int ok = 0;
        for (UserInputData user : users) {
            if (user.getUsername().equals(action.getUsername())) {
                if (user.getSubscriptionType().equals("PREMIUM")) {
                    ok = 1;
                    break;
                }
            }
        }
        return ok;
    }

    /**
     * A method that obtains the list of unseen movies and serials
     * of a particular user, as well as a concatenated list of these two.
     * @param users the list of users
     * @param action the action
     * @param movies the list of movies
     * @param serials the serials
     * @param unseenMovies to store the unseen movies
     * @param unseenSerials to store the unseen serials
     * @param unseenVideos to store the unseen videos
     */
    public static void getUnseenVideos(final List<UserInputData> users,
                                       final ActionInputData action,
                                       final List<MovieInputData> movies,
                                       final List<SerialInputData> serials,
                                       final List<String> unseenMovies,
                                       final List<String> unseenSerials,
                                       final List<String> unseenVideos) {
        for (MovieInputData movie : movies) {
            for (UserInputData user : users) {
                if (user.getUsername().equals(action.getUsername())) {
                    if (!user.getHistory().containsKey(movie.getTitle())) {
                        unseenMovies.add(movie.getTitle());
                        unseenVideos.add(movie.getTitle());
                    }
                    break;
                }
            }
        }
        for (SerialInputData serial : serials) {
            for (UserInputData user : users) {
                if (user.getUsername().equals(action.getUsername())) {
                    if (!user.getHistory().containsKey(serial.getTitle())) {
                        unseenSerials.add(serial.getTitle());
                        unseenVideos.add(serial.getTitle());
                    }
                    break;
                }
            }
        }
    }

    /**
     * A method that calculates the rating of the unseen movies and serials,
     * then storing the name and the rating in a map.
     * @param users the list of users
     * @param action the action
     * @param unseenMovies the list of unseen movies
     * @param unseenSerials the list of unseen serials
     * @param videosUnsorted a map to store the name and the rating
     */
    public static void getRatingVideos(final List<UserInputData> users,
                                       final ActionInputData action,
                                       final List<String> unseenMovies,
                                       final List<String> unseenSerials,
                                       final Map<String, Double> videosUnsorted) {
        for (String movie : unseenMovies) {
            double count = 0.0;
            double sum = 0.0;
            double rating;
            for (UserInputData user : users) {
                if (!user.getUsername().equals(action.getUsername())) {
                    if (user.getMoviesRating().containsKey(movie)) {
                        for (Map.Entry<String, Double> entry : user.getMoviesRating().entrySet()) {
                            if (entry.getKey().equals(movie)) {
                                sum += entry.getValue();
                                count++;
                                break;
                            }
                        }
                    }
                }
            }
            if (count != 0) {
                rating = sum / count;
            } else {
                rating = 0.0;
            }
            if (action.getType().equals("search")) {
                videosUnsorted.put(movie, rating);
            } else {
                if (rating != 0.0) {
                    videosUnsorted.put(movie, rating);
                }
            }
        }
        for (String serial : unseenSerials) {
            double ratingSerial;
            double rating;
            double count = 0.0;
            double sum2 = 0.0;
            for (UserInputData user : users) {
                if (!user.getUsername().equals(action.getUsername())) {
                    if (user.getSerialRating().containsKey(serial)) {
                        for (Map.Entry<String, ArrayList<Double>> entry
                                : user.getSerialRating().entrySet()) {
                            if (entry.getKey().equals(serial)) {
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
            }
            if (count == 0) {
                rating = 0.0;
            } else {
                rating = sum2 / count;
            }
            if (action.getType().equals("search")) {
                videosUnsorted.put(serial, rating);
            } else {
                if (rating != 0.0) {
                    videosUnsorted.put(serial, rating);
                }
            }
        }
    }

    /**
     * A method to obtain every genre and store them in a list.
     * @param movies the list of movies
     * @param serials the list of serials
     * @param genres to store the genres of the videos
     */
    public static void getGenres(final List<MovieInputData> movies,
                                 final List<SerialInputData> serials,
                                 final List<String> genres) {
        for (MovieInputData movie : movies) {
            for (String genre : movie.getGenres()) {
                if (!genres.contains(genre)) {
                    genres.add(genre);
                }
            }
        }
        for (SerialInputData serial : serials) {
            for (String genre : serial.getGenres()) {
                if (!genres.contains(genre)) {
                    genres.add(genre);
                }
            }
        }
    }

    /**
     * A method that calculates the number of views for a given video, whether it is
     * a serial or a movie.
     * @param users the list of users
     * @param action the action
     * @param video the video
     * @return number of views for a video
     */
    public static Double getNrViews(final List<UserInputData> users, final ActionInputData action,
                                    final String video) {
        double nrViews = 0.0;
        for (UserInputData user : users) {
            if (!user.getUsername().equals(action.getUsername())) {
                if (user.getHistory().containsKey(video)) {
                    for (Map.Entry<String, Integer> entry : user.getHistory().entrySet()) {
                        if (entry.getKey().equals(video)) {
                            nrViews += (double) entry.getValue();
                            break;
                        }
                    }
                }
            }
        }
        return nrViews;
    }

    /**
     * A method that calculates the total number of views from every movie/serial
     * and storing the genre as well as that number in a map.
     * @param users the list of users
     * @param action the action
     * @param movies the list of movies
     * @param serials the list of serials
     * @param unseenMovies the list of unseen movies
     * @param unseenSerials the list of unseen serials
     * @param genresPop a map to store the genres and their number of views
     * @param genres the list of genres
     */
    public static void getGenresPop(final List<UserInputData> users,
                                    final ActionInputData action,
                                    final List<MovieInputData> movies,
                                    final List<SerialInputData> serials,
                                    final List<String> unseenMovies,
                                    final List<String> unseenSerials,
                                    final Map<String, Double> genresPop,
                                    final List<String> genres) {
        for (String genre : genres) {
            double nrViews = 0.0;
            for (String unseenMovie : unseenMovies) {
                for (MovieInputData movie : movies) {
                    if (movie.getTitle().equals(unseenMovie)) {
                        if (movie.getGenres().contains(genre)) {
                            nrViews += getNrViews(users, action, unseenMovie);
                        }
                    }
                }
            }
            for (String unseenSerial : unseenSerials) {
                for (SerialInputData serial : serials) {
                    if (serial.getTitle().equals(unseenSerial)) {
                        if (serial.getGenres().contains(genre)) {
                            nrViews += getNrViews(users, action, unseenSerial);
                        }
                    }
                }
            }
            if (nrViews != 0.0) {
                genresPop.put(genre, nrViews);
            }
        }
    }

    /**
     * A method to sort a map.
     * @param unsortedMap the unsorted map
     * @param sortedMap to store the sorted map
     */
    public static void getSortedMap(final Map<String, Double> unsortedMap,
                                   final Map<String, Double> sortedMap) {
        unsortedMap.entrySet()
                .stream()
                .sorted(Map.Entry.comparingByValue(Comparator.reverseOrder()))
                .forEachOrdered(x -> sortedMap.put(x.getKey(), x.getValue()));
    }

    /**
     * A method that returns a message containing the first unseen video from
     * the data base. If it is not possible, then it returns an error message.
     * @param users the list of serials
     * @param action the action
     * @param movies the list of movies
     * @param serials the list of serials
     * @return standard recommendation message
     */
    public static String getStandardMessage(final List<UserInputData> users,
                                            final ActionInputData action,
                                            final List<MovieInputData> movies,
                                            final List<SerialInputData> serials) {
        String standardMessage = null;
        int ok = 0;
        for (MovieInputData movie : movies) {
            for (UserInputData user : users) {
                if (user.getUsername().equals(action.getUsername())) {
                    if (!user.getHistory().containsKey(movie.getTitle())) {
                        ok = 1;
                        standardMessage = "StandardRecommendation result: " + movie.getTitle();
                        break;
                    }
                }
            }
            if (ok == 1) {
                break;
            }
        }
        if (ok == 0) {
            for (SerialInputData serial : serials) {
                for (UserInputData user : users) {
                    if (user.getUsername().equals(action.getUsername())) {
                        if (!user.getHistory().containsKey(serial.getTitle())) {
                            ok = 1;
                            standardMessage = "StandardRecommendation result: "
                                    + serial.getTitle();
                            break;
                        }
                    }
                }
                if (ok == 1) {
                    break;
                }
            }
        }
        if (ok == 0) {
            standardMessage = "StandardRecommendation cannot be applied!";
        }
        return standardMessage;
    }

    /**
     * A method that returns a message containing the best unseen
     * video of an user based on the rating of every movie/serial. If it is
     * not possible, then it returns an error message.
     * @param users the list of users
     * @param action the action
     * @param movies the list of movies
     * @param serials the list of serials
     * @return best unseen recommendation message
     */
    public static String getBestUnseenMessage(final List<UserInputData> users,
                                              final ActionInputData action,
                                              final List<MovieInputData> movies,
                                              final List<SerialInputData> serials) {
        String bestUnseenMessage;
        Map<String, Double> videosUnsorted = new HashMap<>();
        List<String> unseenMovies = new ArrayList<>();
        List<String> unseenSerials = new ArrayList<>();
        List<String> unseenVideos = new ArrayList<>();
        getUnseenVideos(users, action, movies, serials, unseenMovies, unseenSerials, unseenVideos);
        getRatingVideos(users, action, unseenMovies, unseenSerials, videosUnsorted);
        LinkedHashMap<String, Double> videosSorted = new LinkedHashMap<>();
       getSortedMap(videosUnsorted, videosSorted);
        String bestUnseenVideo = null;
        for (Map.Entry<String, Double> entry : videosSorted.entrySet()) {
            bestUnseenVideo = entry.getKey();
            break;
        }
        if (bestUnseenVideo != null) {
            bestUnseenMessage = "BestRatedUnseenRecommendation result: " + bestUnseenVideo;
        } else {
            if (!unseenVideos.isEmpty()) {
                bestUnseenMessage = "BestRatedUnseenRecommendation result: " + unseenVideos.get(0);
            } else {
                bestUnseenMessage = "BestRatedUnseenRecommendation cannot be applied!";
            }
        }
        return bestUnseenMessage;
    }

    /**
     * A method that returns a message containing a video based on how often the video is
     * seen on the favorite list of every user, that has not been seen by the user specified
     * in the action. If it is not possible, then it returns an error message.
     * @param users the list of users
     * @param action the action
     * @param movies the list of movies
     * @param serials the list of serials
     * @return favorite recommendation message
     */
    public static String getFavoriteMessage(final List<UserInputData> users,
                                            final ActionInputData action,
                                            final List<MovieInputData> movies,
                                            final List<SerialInputData> serials) {
        String favoriteMessage;
        if (getOk(users, action) == 1) {
            Map<String, Double> videosUnsorted = new HashMap<>();
            List<String> unseenMovies = new ArrayList<>();
            List<String> unseenSerials = new ArrayList<>();
            List<String> unseenVideos = new ArrayList<>();
            getUnseenVideos(users, action, movies, serials, unseenMovies, unseenSerials,
                    unseenVideos);
            for (String movie : unseenMovies) {
                double nrFav = 0.0;
                for (UserInputData user : users) {
                    if (!user.getUsername().equals(action.getUsername())) {
                        if (user.getFavoriteMovies().contains(movie)) {
                            nrFav++;
                        }
                    }
                }
                if (nrFav != 0) {
                    videosUnsorted.put(movie, nrFav);
                }
            }
            for (String serial : unseenSerials) {
                double nrFav = 0.0;
                for (UserInputData user : users) {
                    if (!user.getUsername().equals(action.getUsername())) {
                        if (user.getFavoriteMovies().contains(serial)) {
                            nrFav++;
                        }
                    }
                }
                if (nrFav != 0) {
                    videosUnsorted.put(serial, nrFav);
                }
            }
            LinkedHashMap<String, Double> videosSorted = new LinkedHashMap<>();
            getSortedMap(videosUnsorted, videosSorted);
            String favVideo = null;
            for (Map.Entry<String, Double> entry : videosSorted.entrySet()) {
                favVideo = entry.getKey();
                break;
            }
            if (favVideo != null) {
                favoriteMessage = "FavoriteRecommendation result: " + favVideo;
            } else {
                favoriteMessage = "FavoriteRecommendation cannot be applied!";
            }
        } else {
            favoriteMessage = "FavoriteRecommendation cannot be applied!";
        }
        return favoriteMessage;
    }

    /**
     * A method that returns a message containing a list of unseen movies/serials
     * from a specific genre. If it is not possible, then it returns an error message.
     * @param users the list of users
     * @param action the action
     * @param movies the list of movies
     * @param serials the serials
     * @return search recommendation message
     */
    public static String getSearchMessage(final List<UserInputData> users,
                                          final ActionInputData action,
                                          final List<MovieInputData> movies,
                                          final List<SerialInputData> serials) {
        String searchMessage;
        if (getOk(users, action) == 1) {
            Map<String, Double> videosUnsorted = new HashMap<>();
            List<String> unseenMovies = new ArrayList<>();
            List<String> unseenSerials = new ArrayList<>();
            List<String> unseenVideos = new ArrayList<>();
            List<String> unseenMoviesGenre = new ArrayList<>();
            List<String> unseenSerialsGenre = new ArrayList<>();
            getUnseenVideos(users, action, movies, serials, unseenMovies, unseenSerials,
                    unseenVideos);
            for (MovieInputData movie : movies) {
                for (String unseenMovie : unseenMovies) {
                    if (movie.getTitle().equals(unseenMovie)) {
                        if (movie.getGenres().contains(action.getGenre())) {
                            unseenMoviesGenre.add(unseenMovie);
                        }
                    }
                }
            }
            for (SerialInputData serial : serials) {
                for (String unseenSerial : unseenSerials) {
                    if (serial.getTitle().equals(unseenSerial)) {
                        if (serial.getGenres().contains(action.getGenre())) {
                            unseenSerialsGenre.add(unseenSerial);
                        }
                    }
                }
            }
            LinkedHashMap<String, Double> sortedMap = new LinkedHashMap<>();
            LinkedHashMap<String, Double> sortedMap2 = new LinkedHashMap<>();
            getRatingVideos(users, action, unseenMoviesGenre, unseenSerialsGenre, videosUnsorted);
            videosUnsorted.entrySet()
                    .stream()
                    .sorted(Map.Entry.comparingByKey())
                    .forEachOrdered(x -> sortedMap.put(x.getKey(), x.getValue()));
            sortedMap.entrySet()
                    .stream()
                    .sorted(Map.Entry.comparingByValue())
                    .forEachOrdered(x -> sortedMap2.put(x.getKey(), x.getValue()));
            List<String> videosSorted = new ArrayList<>();
            for (Map.Entry<String, Double> sort : sortedMap2.entrySet()) {
                videosSorted.add(sort.getKey());
            }
            if (!videosSorted.isEmpty()) {
                searchMessage = "SearchRecommendation result: " + videosSorted;
            } else {
                searchMessage = "SearchRecommendation cannot be applied!";
            }
        } else {
            searchMessage = "SearchRecommendation cannot be applied!";
        }
        return searchMessage;
    }

    /**
     * A method that returns a message containing the first video
     * from the most popular genre that has not been seen by the user. If it
     * is not possible, then it returns an error message.
     * @param users the list of users
     * @param action the action
     * @param movies the list of movies
     * @param serials the list of serials
     * @return popular recommendation message
     */
    public static String getPopularMessage(final List<UserInputData> users,
                                           final ActionInputData action,
                                           final List<MovieInputData> movies,
                                           final List<SerialInputData> serials) {
        String popularMessage = null;
        if (getOk(users, action) == 1) {
            List<String> genres = new ArrayList<>();
            Map<String, Double> genresPop = new HashMap<>();
            List<String> genresSorted = new ArrayList<>();
            List<String> unseenMovies = new ArrayList<>();
            List<String> unseenSerials = new ArrayList<>();
            List<String> unseenVideos = new ArrayList<>();
            getUnseenVideos(users, action, movies, serials, unseenMovies, unseenSerials,
                    unseenVideos);
            getGenres(movies, serials, genres);
            getGenresPop(users, action, movies, serials, unseenMovies, unseenSerials,
                    genresPop, genres);
            LinkedHashMap<String, Double> genresMap = new LinkedHashMap<>();
            getSortedMap(genresPop, genresMap);
            for (Map.Entry<String, Double> sort : genresMap.entrySet()) {
                genresSorted.add(sort.getKey());
            }
            if (!genresSorted.isEmpty()) {
                int ok = 0;
                for (String unseenMovie : unseenMovies) {
                    for (MovieInputData movie : movies) {
                        if (movie.getTitle().equals(unseenMovie)) {
                            if (movie.getGenres().contains(genresSorted.get(0))) {
                                popularMessage = "PopularRecommendation result: "
                                        + movie.getTitle();
                                ok = 1;
                                break;
                            }
                        }
                    }
                    if (ok == 1) {
                        break;
                    }
                }
                if (ok == 0) {
                    for (String unseenSerial : unseenSerials) {
                        for (SerialInputData serial : serials) {
                            if (serial.getTitle().equals(unseenSerial)) {
                                if (serial.getGenres().contains(genresSorted.get(0))) {
                                    popularMessage = "PopularRecommendation result: "
                                            + serial.getTitle();
                                    ok = 1;
                                    break;
                                }
                            }
                        }
                        if (ok == 1) {
                            break;
                        }
                    }
                }
            } else {
                popularMessage = "PopularRecommendation cannot be applied!";
            }
        } else {
            popularMessage = "PopularRecommendation cannot be applied!";
        }
        return popularMessage;
    }
}
