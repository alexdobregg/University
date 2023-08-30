// Copyright 2020
// Author: Matei Simtinică

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

/**
 * Bonus Task
 * You have to implement 4 methods:
 * readProblemData         - read the problem input and store it however you see fit
 * formulateOracleQuestion - transform the current problem instance into a SAT instance and write the oracle input
 * decipherOracleAnswer    - transform the SAT answer back to the current problem's answer
 * writeAnswer             - write the current problem's answer
 */
public class BonusTask extends Task {

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
        // between families (edges).
        N = scanner.nextInt();
        M = scanner.nextInt();
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
        // Minimum Vertex Cover -> Weighted Partial MaxSAT
        int numberOfVariables = N;

        // The number of clauses, calculated using the two FORs.
        int numberOfClauses = M + N;

        // The sum of the weights of the soft clauses + 1.
        int top = M * (N - 1) + 1;

        toWrite.append("p wcnf ")
                .append(numberOfVariables)
                .append(" ")
                .append(numberOfClauses)
                .append(" ")
                .append(top)
                .append("\n");

        // The first set of clauses
        for (int i = 1; i <= N; i++) {
            for (int j : edges.get(i - 1)) {
                toWrite.append(top)
                        .append(" ")
                        .append(i)
                        .append(" ")
                        .append(j)
                        .append(" 0\n");
            }
        }

        // The second set of clauses.
        for (int i = 1; i <= N; i++) {
            toWrite.append(M - edges.get(i - 1).size())
                    .append(" -")
                    .append(i)
                    .append(" 0\n");
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

        scanner.nextLine();

        for (int i = 1; i <= N; i++) {
            // If the value is greater than 0, it means it is part of the group
            // of families that need to be arrested.
            int variable = scanner.nextInt();
            if (variable > 0) {
                toWrite.append(variable)
                        .append(" ");
            }
        }

        BufferedWriter writer = new BufferedWriter(
                new FileWriter(outFilename));
        writer.write(toWrite.toString());
        writer.close();
        scanner.close();
    }
}
