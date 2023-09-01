package query;

import fileio.ActionInputData;
import fileio.UserInputData;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;


public class UserQuery extends Sort {
    /**
     * A method that returns the message containing the list of users, sorted
     * by the number of ratings each one has given.
     * @param users the list of users
     * @param action the action
     * @return user query message
     */
    public static String getUsQMessage(final List<UserInputData> users,
                                       final ActionInputData action) {
        String usQMessage;
        List<String> usersSorted = new ArrayList<>();
        Map<String, Double> usersUnsorted = new HashMap<>();
        for (UserInputData user : users) {
            if ((user.getMoviesRating().size() + user.getSerialRating().size()) != 0) {
                usersUnsorted.put(user.getUsername(), (double) (user.getMoviesRating().size()
                                                        + user.getSerialRating().size()));
            }
        }
        getSortedList(usersUnsorted, action, usersSorted);
        usQMessage = "Query result: " + usersSorted;
        return usQMessage;
    }
}
