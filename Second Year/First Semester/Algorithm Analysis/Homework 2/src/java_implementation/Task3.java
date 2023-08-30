// Copyright 2020
// Author: Matei Simtinică

import java.io.*;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
import java.util.WeakHashMap;

/**
 * Task3
 * This being an optimization problem, the solve method's logic has to work differently.
 * You have to search for the minimum number of arrests by successively querying the oracle.
 * Hint: it might be easier to reduce the current task to a previously solved task
 */
public class Task3 extends Task {
    String task2InFilename;
    String task2OutFilename;

    Task2 task2Solver = new Task2();
    // 0 means we didn't find the families that need to be
    // arrested. 1 means we did.
    int ok = 0;
    // This list contains the families that do not need to be arrested.
    List<Integer> familiesNotToArrest = new ArrayList<>();

    @Override
    public void solve() throws IOException, InterruptedException {
        task2InFilename = inFilename + "_t2";
        task2OutFilename = outFilename + "_t2";
        task2Solver.addFiles(task2InFilename, oracleInFilename, oracleOutFilename, task2OutFilename);
        readProblemData();

        while (ok == 0 && K > 0) {
            reduceToTask2();
            task2Solver.solve();
            extractAnswerFromTask2();
        }

        writeAnswer();
    }

    @Override
    public void readProblemData() throws IOException {
        Scanner scanner = new Scanner(new File(inFilename));

        // Obtaining the number of families (vertexes), the number of relations
        // between families (edges).
        N = scanner.nextInt();
        M = scanner.nextInt();

        // The dimension of the group of families that needs to be arrested.
        // It starts with N - 1, and it is decreasing until we find a group
        // that can be arrested.
        K = N - 1;

        // Obtaining the relations between families.
        for (int i = 0; i < N; i++) {
            edges.add(new ArrayList<>());
        }
        for (int i = 0; i < M; i++) {
            edges.get(scanner.nextInt() - 1).add(scanner.nextInt());
        }

        scanner.close();
    }

    public void reduceToTask2() throws IOException {
        // Obtaining the file that will be considered input for the second
        // task, reducing the current problem to task 2.
        StringBuilder toWrite = new StringBuilder();

        toWrite.append(N)
                .append(" ")
                .append(N * (N - 1) / 2 - M)
                .append(" ")
                .append(K)
                .append("\n");

        for (int i = 1; i < N; i++) {
            for (int j = i + 1; j <= N; j++) {
                if (!edges.get(i - 1).contains(j)) {
                    toWrite.append(i)
                            .append(" ")
                            .append(j)
                            .append("\n");
                }
            }
        }

        BufferedWriter writer = new BufferedWriter(
                new FileWriter(task2InFilename));
        writer.write(toWrite.toString());
        writer.close();
    }

    public void extractAnswerFromTask2() throws IOException {
        Scanner scanner = new Scanner(new File(task2OutFilename));
        String truthValue = scanner.nextLine();

        if (truthValue.equals("True")) {
            ok = 1;
            // We found the biggest group of families that do not need to
            // be arrested.
            for (int i = 0; i < K; i++) {
                familiesNotToArrest.add(scanner.nextInt());
            }
        } else {
            K--;
        }

        scanner.close();
    }

    @Override
    public void writeAnswer() throws IOException {
        StringBuilder toWrite = new StringBuilder();

        for (int i = 1; i <= N; i++) {
            // Writing the families that need to be arrested.
            if (!familiesNotToArrest.contains(i)) {
                toWrite.append(i)
                        .append(" ");
            }
        }

        BufferedWriter writer = new BufferedWriter(
                new FileWriter(outFilename));
        writer.write(toWrite.toString());
        writer.close();
    }
}
