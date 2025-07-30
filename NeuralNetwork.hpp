// NeuralNetwork.hpp

#ifndef NEURAL_NETWORK_HPP
#define NEURAL_NETWORK_HPP

#include <vector>
#include <cmath>
#include <random>
#include <algorithm> // std::sort için

class NeuralNetwork {
public:
    std::vector<std::vector<float>> weights_ih;
    std::vector<std::vector<float>> weights_ho;

    NeuralNetwork(int inputNodes, int hiddenNodes, int outputNodes)
        : input_nodes(inputNodes), hidden_nodes(hiddenNodes), output_nodes(outputNodes) {
        
        weights_ih.resize(hidden_nodes, std::vector<float>(input_nodes));
        weights_ho.resize(output_nodes, std::vector<float>(hidden_nodes));

        randomizeWeights();
    }

    void randomizeWeights() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> distr(-1.0, 1.0);
        for (auto& row : weights_ih) for (auto& weight : row) weight = distr(gen);
        for (auto& row : weights_ho) for (auto& weight : row) weight = distr(gen);
    }

    std::vector<float> feedForward(const std::vector<float>& inputs) {
        std::vector<float> hidden(hidden_nodes);
        for (int i = 0; i < hidden_nodes; ++i) {
            float sum = 0;
            for (int j = 0; j < input_nodes; ++j) {
                sum += inputs[j] * weights_ih[i][j];
            }
            hidden[i] = tanh(sum);
        }

        std::vector<float> outputs(output_nodes);
        for (int i = 0; i < output_nodes; ++i) {
            float sum = 0;
            for (int j = 0; j < hidden_nodes; ++j) {
                sum += hidden[j] * weights_ho[i][j];
            }
            outputs[i] = tanh(sum);
        }
        return outputs;
    }

    static NeuralNetwork crossover(const NeuralNetwork& parentA, const NeuralNetwork& parentB) {
        NeuralNetwork child(parentA.input_nodes, parentA.hidden_nodes, parentA.output_nodes);
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(0, 1);

        for (size_t i = 0; i < child.weights_ih.size(); ++i) {
            for (size_t j = 0; j < child.weights_ih[i].size(); ++j) {
                child.weights_ih[i][j] = (distr(gen) == 0) ? parentA.weights_ih[i][j] : parentB.weights_ih[i][j];
            }
        }
        for (size_t i = 0; i < child.weights_ho.size(); ++i) {
            for (size_t j = 0; j < child.weights_ho[i].size(); ++j) {
                child.weights_ho[i][j] = (distr(gen) == 0) ? parentA.weights_ho[i][j] : parentB.weights_ho[i][j];
            }
        }
        return child;
    }

    void mutate(float mutationRate) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> distr(0.0, 1.0);
        std::uniform_real_distribution<> mutation_distr(-0.1, 0.1);

        for (auto& row : weights_ih) {
            for (auto& weight : row) {
                if (distr(gen) < mutationRate) {
                    weight += mutation_distr(gen);
                }
            }
        }
        for (auto& row : weights_ho) {
            for (auto& weight : row) {
                if (distr(gen) < mutationRate) {
                    weight += mutation_distr(gen);
                }
            }
        }
    }

private:
    int input_nodes, hidden_nodes, output_nodes;
};

#endif // NEURAL_NETWORK_HPP