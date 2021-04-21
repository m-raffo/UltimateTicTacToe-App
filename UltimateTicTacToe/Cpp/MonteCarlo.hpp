#pragma once

#include <bitset>
#include <vector>
#include <iostream>
#include "GameState.hpp"
#include "Minimax.hpp"
#include <limits>
#include "dirichlet.hpp"


struct trainingExample {
    std::bitset<199> canonicalBoard;
    float result;
    float pi[81];
    float q;

    std::vector<int> validMoves;
};

struct trainingExampleVector {
    std::vector<int> canonicalBoard;
    float result;
    float q;
    std::vector<float> pi;
    int timesSeen = 1;

    std::vector<int> validMoves;
};



class MCTS {
    float cpuct = 1;
    double dirichlet_a = 0.8;
    float percent_q = 0.5;

    std::vector<trainingExample> trainingPositions;

    std::mt19937 gen;
    dirichlet_distribution<std::mt19937> dirichlet;

    public:
    Node rootNode;
    Node *currentNode;


        MCTS();
        MCTS(float _cupct, double _dirichlet, float _percent_q);

        bool gameOver = false;


        void startNewSearch(GameState position);

        void backpropagate(Node *finalNode, float result);

    std::vector<int> searchPreNN();
    std::vector<int> getAllPossibleMovesVector();
        void searchPostNN(std::vector<float> policy, float v);

        nnInput searchPreNNforTfLite();
        void searchPostNNTfLite(nnOutput result);

        bool evaluationNeeded;

    std::vector<float> getActionProb();
    std::vector<float> getQProb();
    std::vector<float> getPProb();
    std::vector<float> getVProb();

        int maxActionProb();

        void takeAction(int actionIndex);
        int getStatus();
        void displayGame();
    std::string gameToString();

        void saveTrainingExample(std::vector<float> pi, float q);
    std::vector<trainingExample> getTrainingExamples(int result);
    std::vector<trainingExampleVector> getTrainingExamplesVector(int result);
        void purgeTrainingExamples();

    std::vector<double> dir(double a, int dim);

};

std::vector<std::vector<int>> getSymmetriesBoard(std::vector<int> board);
std::vector<std::vector<float>> getSymmetriesPi(std::vector<float> pi);
std::vector<std::vector<int>> getSymmetriesMoves(std::vector<int> moves);
std::vector<trainingExampleVector> getSymmetries(trainingExampleVector position);


int findCanonicalRotation(std::vector<int> board);

std::vector<int> getCanonicalBoardRotation(std::vector<int> board);
trainingExampleVector getCanonicalTrainingExampleRotation(trainingExampleVector ex);
