#include <iostream>
#include <cmath>
#include <random>

const int w = 1000; 
const int h = 800;  
const float V = 2.0f; 

struct Point {
    float x, y;
};

class House {
protected:
    Point pos;      
    Point target;    
    bool moving;    
public:
    House(float x, float y) : pos{ x, y }, moving(true) {}

    virtual void generateTarget() = 0;

    virtual void update() {
        if (!moving) return;

        float dx = target.x - pos.x;
        float dy = target.y - pos.y;
        float dist = std::sqrt(dx * dx + dy * dy);

        if (dist < V) {
            pos = target;
            moving = false;
        }
        else {
            pos.x += V * dx / dist;
            pos.y += V * dy / dist;
        }
    }

    bool isMoving() const { return moving; }
    Point getPosition() const { return pos; }
};

float randomFloat(float min, float max) {
    static std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}
class CapitalHouse : public House {
public:
    CapitalHouse(float x, float y) : House(x, y) {
        if (x <= w / 2 && y <= h / 2) {
            moving = false;
        }
        else {
            generateTarget();
        }
    }

    void generateTarget() override {
        target.x = randomFloat(0, w / 2);
        target.y = randomFloat(0, h / 2);
    }
};
class WoodenHouse : public House {
public:
    WoodenHouse(float x, float y) : House(x, y) {
        if (x >= w / 2 && y >= h / 2) {
            moving = false;
        }
        else {
            generateTarget();
        }
    }

    void generateTarget() override {
        target.x = randomFloat(w / 2, w);
        target.y = randomFloat(h / 2, h);
    }
};
int main() {
    CapitalHouse ch(800, 600);    
    WoodenHouse wh(100, 100);     

    for (int t = 0; t < 1000; ++t) {
        if (ch.isMoving()) ch.update();
        if (wh.isMoving()) wh.update();

        std::cout << "CapitalHouse: (" << ch.getPosition().x << ", " << ch.getPosition().y << ")\n";
        std::cout << "WoodenHouse: (" << wh.getPosition().x << ", " << wh.getPosition().y << ")\n";

        if (!ch.isMoving() && !wh.isMoving()) break;
    }

    return 0;
}
