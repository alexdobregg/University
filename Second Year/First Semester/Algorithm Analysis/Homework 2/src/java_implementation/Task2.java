// Copyright 2020
// Author: Matei Simtinică

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
import java.util.stream.IntStream;

/**
 * Task2
 * You have to implement 4 methods:
 * readProblemData         - read the problem input and store it however you see fit
 * formulateOracleQuestion - transform the current problem instance into a SAT instance and write the oracle input
 * decipherOracleAnswer    - transform the SAT answer back to the current problem's answer
 * writeAnswer             - write the current problem's answer
 */
public class Task2 extends Task {
    @Override
    public void solve() throws IOException, InterruptedException {
        readProblemData();
        formulateOracleQuestion();
        askOracle();
        decipherOracleAnswer();
        writeAnswer();
    }

    @Override
    public void readProblemData() throws IOException {
        Scanner scanner = new Scanner(new File(inFilename));

        // Obtaining the number of families (vertexes), the number of relations
        // between families (edges) and dimension of the extended family.
        N = scanner.nextInt();
        M = scanner.nextInt();
        K = scanner.nextInt();

        // Obtaining the relations between families.
        for (int i = 0; i < N; i++) {
            edges.add(new ArrayList<>());
        }
        for (int i = 0; i < M; i++) {
            edges.get(scanner.nextInt() - 1).add(scanner.nextInt());
        }

        scanner.close();
    }


    @Override
    public void formulateOracleQuestion() throws IOException {
        StringBuilder toWrite = new StringBuilder();

        // The number of variables used, considering the reduction:
        // Clique -> SAT.
        int numberOfVariables = N * K;

        // The number of clauses, calculated using the three FORs.
        int numberOfClauses = K + N * ((K * (K - 1)) / 2)
                        + (K * (K - 1) / 2) * (N * (N - 1) - M);

        List<int[]> listVar = new ArrayList<>();
        for (int i = 0; i < K; i++) {
            listVar.add(IntStream
                        .rangeClosed(N * i + 1, N * (i + 1))
                        .toArray());
        }

        toWrite.append("p cnf ")
                .append(numberOfVariables)
                .append(" ")
                .append(numberOfClauses)
                .append("\n");

        // The first set of clauses.
        for (int i = 1; i <= K; i++) {
            for (int v = 1; v <= N; v++) {
                toWrite.append(listVar.get(i - 1)[v - 1])
                        .append(" ");
            }
            toWrite.append("0\n");
        }

        // The second set of clauses.
        for (int i = 1; i < K; i++) {
            for (int j = i + 1; j <= K; j++) {
                for (int v = 1; v <= N; v++) {
                    toWrite.append("-")
                            .append(listVar.get(i - 1)[v - 1])
                            .append(" -")
                            .append(listVar.get(j - 1)[v - 1])
                            .append(" 0\n");
                }
            }
        }

        // The third set of clauses
        for (int i = 1; i < K; i++) {
            for (int j = i + 1; j <= K; j++) {
                for (int v = 1; v <= N; v++) {
                    for (int w = 1; w <= N; w++) {
                        if (v != w && !edges.get(v - 1).contains(w)) {
                            toWrite.append("-")
                                    .append(listVar.get(i - 1)[v - 1])
                                    .append(" -")
                                    .append(listVar.get(j - 1)[w - 1])
                                    .append(" 0\n");
                        }
                    }
                }
            }
        }

        BufferedWriter writer = new BufferedWriter(
                new FileWriter(oracleInFilename));
        writer.write(toWrite.toString());
        writer.close();
    }

    @Override
    public void decipherOracleAnswer() throws IOException {
        try {
            askOracle();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void writeAnswer() throws IOException {
        Scanner scanner = new Scanner(new File(oracleOutFilename));
        StringBuilder toWrite = new StringBuilder();

        String truthValue = scanner.nextLine();

        if (truthValue.equals("True")) {
            // Writes True/False depending on the truth value obtained after
            // the oracle was asked. If it's true, it also writes the list of
            // the extended family.
            toWrite.append("True\n");
            // Skipping the number of variables.
            scanner.nextInt();
            for (int i = 1; i <= K; i++) {
                for (int j = 1; j <= N; j++) {
                    // Reading the variables from the file. Some of them are
                    // positive, and the other are negatives, so we can know
                    // which families form the extended family.
                    if (scanner.nextInt() > 0) {
                        toWrite.append(j)
                                .append(" ");
                    }
                }
            }
        } else {
            toWrite.append("False");
        }

        BufferedWriter writer = new BufferedWriter(
                new FileWriter(outFilename));
        writer.write(toWrite.toString());
        writer.close();
        scanner.close();
    }
}
