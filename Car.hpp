// Car.hpp

#ifndef CAR_HPP
#define CAR_HPP

#include <SFML/Graphics.hpp>
#include "NeuralNetwork.hpp"
#include <vector>
#include <cmath>

struct Ray {
    sf::VertexArray line;
    float angleOffset;
    float length = 0;
    Ray(float offset) : angleOffset(offset) {
        line.setPrimitiveType(sf::PrimitiveType::Lines);
        line.resize(2);
    }
};

class Car {
public:
    sf::RectangleShape shape;
    NeuralNetwork brain;
    std::vector<Ray> sensors;
    float fitness = 0;
    bool isCrashed = false;
    int nextCheckpoint = 1;

    Car() : brain(5, 8, 2) {
        shape.setSize({40.f, 20.f});
        shape.setFillColor(sf::Color::Red);
        shape.setOrigin({shape.getSize().x / 2, shape.getSize().y / 2});

        sensors.emplace_back(-90.f); sensors.emplace_back(-45.f);
        sensors.emplace_back(0.f);   sensors.emplace_back(45.f);
        sensors.emplace_back(90.f);
    }

    void reset() {
        // DÜZELTME: Yeni, daha büyük pistin başlangıç noktası
        shape.setPosition({800.f, 150.f});
        shape.setRotation(sf::degrees(0.f));
        shape.setFillColor(sf::Color::Red);
        isCrashed = false;
        fitness = 0;
        nextCheckpoint = 1;
    }
    
    // ... (dosyanın geri kalanı aynı) ...
    void update(const std::vector<sf::CircleShape>& checkpoints, float time) {
        if (isCrashed) return;

        fitness = (nextCheckpoint - 1) * 1000 + (20.f - time);

        std::vector<float> inputs;
        for (auto& sensor : sensors) { inputs.push_back(sensor.length / 200.f); }
        std::vector<float> outputs = brain.feedForward(inputs);
        float speedControl = outputs[0];
        float turnControl = outputs[1];
        float speed = (speedControl > 0) ? 5.f : 0.f;
        shape.rotate(sf::degrees(turnControl * 3.f));
        const float PI = 3.14159265f;
        float angle = shape.getRotation().asDegrees();
        float radyanAngle = angle * PI / 180.f;
        shape.move({speed * std::cos(radyanAngle), speed * std::sin(radyanAngle)});

        sf::Vector2f carPos = shape.getPosition();
        sf::Vector2f checkpointPos = checkpoints[nextCheckpoint].getPosition();
        float dist = std::sqrt(std::pow(carPos.x - checkpointPos.x, 2) + std::pow(carPos.y - checkpointPos.y, 2));
        if (dist < checkpoints[nextCheckpoint].getRadius()) {
            nextCheckpoint = (nextCheckpoint + 1) % checkpoints.size();
        }
    }
    
    void checkCollision(const sf::Vector2f& trackCenter, float outerRadius, float innerRadius) {
        if(isCrashed) return;
        sf::Vector2f carPos = shape.getPosition();
        float dist = std::sqrt(std::pow(carPos.x - trackCenter.x, 2) + std::pow(carPos.y - trackCenter.y, 2));
        if (dist > outerRadius - 10.f || dist < innerRadius + 10.f) {
            isCrashed = true;
            shape.setFillColor(sf::Color(50, 50, 150));
        }
    }

    void updateSensors(const sf::Vector2f& trackCenter, float outerRadius, float innerRadius) {
        if(isCrashed) return;
        sf::Vector2f carPos = shape.getPosition();
        const float PI = 3.14159265f;
        for (auto& ray : sensors) {
            float carAngle = shape.getRotation().asDegrees();
            float rayAngleRadian = (carAngle + ray.angleOffset) * PI / 180.f;
            ray.line[0].position = carPos;
            ray.length = 200.f;
            for (float i = 0; i < 200; i += 5) {
                sf::Vector2f point = {carPos.x + i * std::cos(rayAngleRadian), carPos.y + i * std::sin(rayAngleRadian)};
                float dist = std::sqrt(std::pow(point.x - trackCenter.x, 2) + std::pow(point.y - trackCenter.y, 2));
                if (dist > outerRadius - 10.f || dist < innerRadius + 10.f) {
                    ray.length = i;
                    break;
                }
            }
            ray.line[1].position = {carPos.x + ray.length * std::cos(rayAngleRadian), carPos.y + ray.length * std::sin(rayAngleRadian)};
        }
    }
};

#endif // CAR_HPP