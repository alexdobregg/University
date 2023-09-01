package main;

import checker.Checkstyle;
import checker.Checker;

import fileio.ActionInputData;
import fileio.UserInputData;
import fileio.SerialInputData;
import fileio.MovieInputData;
import fileio.Writer;
import fileio.InputLoader;
import fileio.Input;
import fileio.ActorInputData;

import common.Constants;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import query.ActorQuery;
import query.MovieQuery;
import query.UserQuery;
import query.SerialQuery;
import recommendations.UserRecommendations;
import commands.Command;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.List;
import java.util.Objects;

/**
 * The entry point to this homework. It runs the checker that tests your implementation.
 */
public final class Main {
    /**
     * for coding style
     */
    private Main() {
    }

    /**
     * Call the main checker and the coding style checker
     * @param args from command line
     * @throws IOException in case of exceptions to reading / writing
     */
    public static void main(final String[] args) throws IOException {
        File directory = new File(Constants.TESTS_PATH);
        Path path = Paths.get(Constants.RESULT_PATH);
        if (!Files.exists(path)) {
            Files.createDirectories(path);
        }

        File outputDirectory = new File(Constants.RESULT_PATH);

        Checker checker = new Checker();
        checker.deleteFiles(outputDirectory.listFiles());

        for (File file : Objects.requireNonNull(directory.listFiles())) {

            String filepath = Constants.OUT_PATH + file.getName();
            File out = new File(filepath);
            boolean isCreated = out.createNewFile();
            if (isCreated) {
                action(file.getAbsolutePath(), filepath);
            }
        }

        checker.iterateFiles(Constants.RESULT_PATH, Constants.REF_PATH, Constants.TESTS_PATH);
        Checkstyle test = new Checkstyle();
        test.testCheckstyle();
    }

    /**
     * @param filePath1 for input file
     * @param filePath2 for output file
     * @throws IOException in case of exceptions to reading / writing
     */
    public static void action(final String filePath1,
                              final String filePath2) throws IOException {
        InputLoader inputLoader = new InputLoader(filePath1);
        Input input = inputLoader.readData();

        Writer fileWriter = new Writer(filePath2);
        JSONArray arrayResult = new JSONArray();
        //TODO add here the entry point to your implementation
        List<ActionInputData> actions = input.getCommands();
        List<UserInputData> users = input.getUsers();
        List<MovieInputData> movies = input.getMovies();
        List<SerialInputData> serials = input.getSerials();
        List<ActorInputData> actors = input.getActors();
        for (ActionInputData action : actions) {
            if (action.getActionType().equals("command")) {
                if (action.getType().equals("favorite")) {
                    String message = Command.getFavMessage(users, action);
                    JSONObject obj = fileWriter.writeFile(action.getActionId(), null, message);
                    arrayResult.add(obj);
                }
                if (action.getType().equals("view")) {
                    String message = Command.getViewMessage(users, action);
                    JSONObject obj = fileWriter.writeFile(action.getActionId(), null, message);
                    arrayResult.add(obj);
                }
                if (action.getType().equals("rating")) {
                    if (action.getSeasonNumber() == 0) {
                        String message = Command.getRatingMovMessage(users, action);
                        JSONObject obj = fileWriter.writeFile(action.getActionId(), null, message);
                        arrayResult.add(obj);
                    } else {
                        String message = Command.getRatingSeasMessage(users, action, serials);

                        JSONObject obj = fileWriter.writeFile(action.getActionId(), null, message);
                        arrayResult.add(obj);
                    }
                }
            } else if (action.getActionType().equals("query")) {
                if (action.getObjectType().equals("users")) {
                    String message = UserQuery.getUsQMessage(users, action);
                    JSONObject obj = fileWriter.writeFile(action.getActionId(), null, message);
                    arrayResult.add(obj);
                }
                if (action.getObjectType().equals("movies")) {
                    if (action.getCriteria().equals("favorite")) {
                        String message = MovieQuery.getFavoriteMessage(users, action, movies);
                        JSONObject obj = fileWriter.writeFile(action.getActionId(), null, message);
                        arrayResult.add(obj);
                    }
                    if (action.getCriteria().equals("longest")) {
                        String message = MovieQuery.getLongestMessage(action, movies);
                        JSONObject obj = fileWriter.writeFile(action.getActionId(), null, message);
                        arrayResult.add(obj);
                    }
                    if (action.getCriteria().equals("most_viewed")) {
                        String message = MovieQuery.getMvMessage(users, action, movies);
                        JSONObject obj = fileWriter.writeFile(action.getActionId(), null, message);
                        arrayResult.add(obj);
                    }
                    if (action.getCriteria().equals("ratings")) {
                        String message = MovieQuery.getRatingMessage(users, action, movies);
                        JSONObject obj = fileWriter.writeFile(action.getActionId(), null, message);
                        arrayResult.add(obj);
                    }
                }
                if (action.getObjectType().equals("shows")) {
                    if (action.getCriteria().equals("favorite")) {
                        String message = SerialQuery.getFavoriteMessage(users, action, serials);
                        JSONObject obj = fileWriter.writeFile(action.getActionId(), null, message);
                        arrayResult.add(obj);
                    }
                    if (action.getCriteria().equals("longest")) {
                        String message = SerialQuery.getLongestMessage(action, serials);
                        JSONObject obj = fileWriter.writeFile(action.getActionId(), null, message);
                        arrayResult.add(obj);
                    }
                    if (action.getCriteria().equals("most_viewed")) {
                        String message = SerialQuery.getMvMessage(users, action, serials);
                        JSONObject obj = fileWriter.writeFile(action.getActionId(), null, message);
                        arrayResult.add(obj);
                    }
                    if (action.getCriteria().equals("ratings")) {
                        String message = SerialQuery.getRatingMessage(users, action, serials);
                        JSONObject obj = fileWriter.writeFile(action.getActionId(), null, message);
                        arrayResult.add(obj);
                    }
                }
                if (action.getObjectType().equals("actors")) {
                    if (action.getCriteria().equals("awards")) {
                        String message = ActorQuery.getAwardsMessage(action, actors);
                        JSONObject obj = fileWriter.writeFile(action.getActionId(), null, message);
                        arrayResult.add(obj);
                    }
                    if (action.getCriteria().equals("average")) {
                        String message = ActorQuery.getAverageMessage(users, action, actors);
                        JSONObject obj = fileWriter.writeFile(action.getActionId(), null, message);
                        arrayResult.add(obj);
                    }
                    if (action.getCriteria().equals("filter_description")) {
                        String message = ActorQuery.getFdMessage(action, actors);
                        JSONObject obj = fileWriter.writeFile(action.getActionId(), null, message);
                        arrayResult.add(obj);
                    }
                }
            } else if (action.getActionType().equals("recommendation")) {

                if (action.getType().equals("standard")) {
                    String message = UserRecommendations.getStandardMessage(users, action,
                            movies, serials);
                    JSONObject obj = fileWriter.writeFile(action.getActionId(), null, message);
                    arrayResult.add(obj);
                }
                if (action.getType().equals("best_unseen")) {
                    String message = UserRecommendations.getBestUnseenMessage(users, action,
                            movies, serials);
                    JSONObject obj = fileWriter.writeFile(action.getActionId(), null, message);
                    arrayResult.add(obj);
                }
                if (action.getType().equals("favorite")) {
                    String message = UserRecommendations.getFavoriteMessage(users, action,
                            movies, serials);
                    JSONObject obj = fileWriter.writeFile(action.getActionId(), null, message);
                    arrayResult.add(obj);
                }
                if (action.getType().equals("search")) {
                    String message = UserRecommendations.getSearchMessage(users, action,
                            movies, serials);
                    JSONObject obj = fileWriter.writeFile(action.getActionId(), null, message);
                    arrayResult.add(obj);
                }
                if (action.getType().equals("popular")) {
                    String message = UserRecommendations.getPopularMessage(users, action,
                            movies, serials);
                    JSONObject obj = fileWriter.writeFile(action.getActionId(), null, message);
                    arrayResult.add(obj);
                }
            }
        }
        fileWriter.closeJSON(arrayResult);
    }
}
