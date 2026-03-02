/*
** EPITECH PROJECT, 2025
** bsNeuronalNetwork
** File description:
** main
*/

#include "NeuronalNetwork.hpp"
#include <iostream>

bool test_Neurone(NeuralNetwork p, const std::vector<std::pair<std::vector<float>, float>> &test_data)
{
    for (const auto &data : test_data) {
        float output = (p.activateNeurone(data.first) >= 0.5f) ? 1.0f : 0.0f;
        if (output != data.second) {
            return false;
        }
    }
    return true;
}

int main()
{
    std::vector<std::pair<std::vector<float>, float>> training_data = {
        {{0.0f, 0.0f}, 0.0f},
        {{0.0f, 1.0f}, 1.0f},
        {{1.0f, 0.0f}, 1.0f},
        {{1.0f, 1.0f}, 1.0f}
    };
    int nombre1;
    int nombre2;
    // perceptron_t p = create_perceptron(2, 0.1f);
    // std::vector<perceptron_t> p = create_layer(2, 2, 0.1f);
    NeuralNetwork p(2, 0.1f);
    p.addLayer(4);
    // p.addLayer(2);
    int random_index = 0;
    for (int a = 0; a < 100000; a++) {
        std::cout << "Training iteration: " << a << std::endl;
        random_index = getRandomInt(0, 3);
        p.back_propagation(training_data[random_index].first, training_data[random_index].second);
        if (test_Neurone(p, training_data)) {
            std::cout << "Training complete after " << a + 1 << " iterations." << std::endl;
            break;
        }
    }

    while (true) {
        
        std::cout << "Entrez le premier nombre entier : ";
        
        if (!(std::cin >> nombre1)) {
            break; 
        }

        std::cout << "Entrez le deuxieme nombre entier : ";
        if (!(std::cin >> nombre2)) {
            break;
        }
        std::vector<float> inputs = {static_cast<float>(nombre1), static_cast<float>(nombre2)};
        float output = (p.activateNeurone(inputs) >= 0.5f) ? 1.0f : 0.0f;
        std::cout << "Output: " << output << std::endl;
    }

    return 0;
}
