#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include "Car.hpp"
#include <algorithm>
#include <iostream>

void createNextGeneration(std::vector<Car>& cars, float mutationRate, float& bestFitness) {
    std::sort(cars.begin(), cars.end(), [](const Car& a, const Car& b) {
        return a.fitness > b.fitness;
    });

    if (cars[0].fitness > bestFitness) {
        bestFitness = cars[0].fitness;
    }
    std::cout << "Nesil Bitti. En Iyi Skor: " << cars[0].fitness << " | Genel Rekor: " << bestFitness << std::endl;

    std::vector<Car> parents;
    int parentCount = cars.size() * 0.2;
    for (int i = 0; i < parentCount; ++i) {
        parents.push_back(cars[i]);
    }

    std::vector<Car> newGeneration;
    newGeneration.push_back(cars[0]);
    newGeneration.back().brain.mutate(0);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, parents.size() - 1);

    for (size_t i = 1; i < cars.size(); ++i) {
        const Car& parentA = parents[distr(gen)];
        const Car& parentB = parents[distr(gen)];
        NeuralNetwork childBrain = NeuralNetwork::crossover(parentA.brain, parentB.brain);
        childBrain.mutate(mutationRate);
        Car newCar;
        newCar.brain = childBrain;
        newGeneration.push_back(newCar);
    }
    
    cars = newGeneration;
    for (auto& car : cars) {
        car.reset();
    }
}

int main()
{
    // DÜZELTME: Ekran boyutunu büyüt
    sf::RenderWindow window(sf::VideoMode({1600, 900}), "Genetik Algoritma - Araba Yarisi");
    window.setFramerateLimit(60);

    // DÜZELTME: Pist sabitlerini yeni ekran boyutuna göre güncelle
    const sf::Vector2f PIST_MERKEZI = {800.f, 450.f}; // Ekranın yeni merkezi
    const float DIS_YARICAP = 400.f;                 // Dış daire daha büyük
    const float IC_YARICAP = 200.f;                  // İç daire daha küçük (yol daha geniş)
    
    sf::CircleShape disPist(DIS_YARICAP);
    disPist.setFillColor(sf::Color(128, 128, 128));
    disPist.setOrigin({disPist.getRadius(), disPist.getRadius()});
    disPist.setPosition(PIST_MERKEZI);

    sf::CircleShape icPist(IC_YARICAP);
    icPist.setFillColor(sf::Color::Black);
    icPist.setOrigin({icPist.getRadius(), icPist.getRadius()});
    icPist.setPosition(PIST_MERKEZI);

    // Kontrol Noktaları
    std::vector<sf::CircleShape> checkpoints;
    const int checkpointSayisi = 12; // Daha fazla checkpoint
    for (int i = 0; i < checkpointSayisi; ++i) {
        float angle = (360.f / checkpointSayisi) * i;
        float radyan = angle * 3.14159265f / 180.f;
        float radius = (DIS_YARICAP + IC_YARICAP) / 2;
        sf::Vector2f pos = {PIST_MERKEZI.x + radius * std::cos(radyan), PIST_MERKEZI.y + radius * std::sin(radyan)};
        
        sf::CircleShape cp(30.f); // Checkpointler de biraz büyüdü
        cp.setOrigin({cp.getRadius(), cp.getRadius()});
        cp.setPosition(pos);
        cp.setFillColor(sf::Color(0, 255, 0, 50));
        checkpoints.push_back(cp);
    }

    const int POPULATION_SIZE = 500;
    std::vector<Car> cars(POPULATION_SIZE);
    for (auto& car : cars) {
        car.reset();
    }

    sf::Font font;
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) return -1;
    sf::Text infoText(font, "Yukleniyor...", 20);
    infoText.setFillColor(sf::Color::White);
    infoText.setPosition({10.f, 10.f});
    
    int generation = 1;
    float bestFitness = 0;
    float mutationRate = 0.05f;

    sf::Clock generationClock; 
    const float GENERATION_TIME_LIMIT = 30.f; // Süreyi biraz uzattık

    while (window.isOpen())
    {
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
        }

        int activeCars = 0;
        for (auto& car : cars) {
            if (!car.isCrashed) {
                car.updateSensors(PIST_MERKEZI, DIS_YARICAP, IC_YARICAP);
                car.update(checkpoints, generationClock.getElapsedTime().asSeconds());
                car.checkCollision(PIST_MERKEZI, DIS_YARICAP, IC_YARICAP);
                activeCars++;
            }
        }
        
        float elapsedTime = generationClock.getElapsedTime().asSeconds();
        infoText.setString("Nesil: " + std::to_string(generation) + 
                         "\nKalan Sure: " + std::to_string(int(GENERATION_TIME_LIMIT - elapsedTime)) +
                         "\nHayattaki Araba: " + std::to_string(activeCars) +
                         "\nGenel Rekor: " + std::to_string(bestFitness));

        if (elapsedTime >= GENERATION_TIME_LIMIT || activeCars == 0) {
            createNextGeneration(cars, mutationRate, bestFitness); 
            generation++;
            generationClock.restart();
        }

        window.clear(sf::Color::Black);
        window.draw(disPist);
        window.draw(icPist);
        for(const auto& cp : checkpoints) window.draw(cp);
        for (auto& car : cars) {
            if (!car.isCrashed) window.draw(car.shape);
        }
        window.draw(infoText);
        window.display();
    }

    return 0;
}