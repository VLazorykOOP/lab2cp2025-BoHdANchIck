#include <iostream>
#include <vector>
#include <thread>
#include <random>
#include <chrono>
#include <mutex>

const int w = 800;
const int h = 600;
const float V = 2.0f; 

std::mutex coutMutex; 

struct Point {
    float x, y;

    bool isCloseTo(const Point& other, float epsilon = 1.0f) const {
        return (std::abs(x - other.x) < epsilon && std::abs(y - other.y) < epsilon);
    }
};

class House {
protected:
    Point pos;
    Point target;
    bool moving = true;
    std::string type;

public:
    House(Point p, std::string t) : pos(p), type(t) {}

    virtual void move() = 0;

    virtual ~House() {}
};

std::random_device rd;
std::mt19937 gen(rd());

float getRandom(float min, float max) {
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

class CapitalHouse : public House {
public:
    CapitalHouse(Point p) : House(p, "Capital") {
        if (p.x <= w / 2 && p.y <= h / 2)
            moving = false;
        else {
            target.x = getRandom(0, w / 2);
            target.y = getRandom(0, h / 2);
        }
    }

    void move() override {
        while (moving) {
            float dx = target.x - pos.x;
            float dy = target.y - pos.y;
            float dist = std::sqrt(dx * dx + dy * dy);

            if (dist < V) {
                pos = target;
                moving = false;
                break;
            }

            pos.x += V * dx / dist;
            pos.y += V * dy / dist;

            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        std::lock_guard<std::mutex> lock(coutMutex);
        std::cout << "CapitalHouse at point (" << pos.x << ", " << pos.y << ")\n";
    }
};

class WoodenHouse : public House {
public:
    WoodenHouse(Point p) : House(p, "Wooden") {
        if (p.x >= w / 2 && p.y >= h / 2)
            moving = false;
        else {
            target.x = getRandom(w / 2, w);
            target.y = getRandom(h / 2, h);
        }
    }

    void move() override {
        while (moving) {
            float dx = target.x - pos.x;
            float dy = target.y - pos.y;
            float dist = std::sqrt(dx * dx + dy * dy);

            if (dist < V) {
                pos = target;
                moving = false;
                break;
            }

            pos.x += V * dx / dist;
            pos.y += V * dy / dist;

            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        std::lock_guard<std::mutex> lock(coutMutex);
        std::cout << "WoodenHouse at point (" << pos.x << ", " << pos.y << ")\n";
    }
};

int main() {
    std::vector<std::thread> threads;
    std::vector<House*> houses;

    for (int i = 0; i < 5; ++i) {
        Point p = { getRandom(0, w), getRandom(0, h) };
        houses.push_back(new CapitalHouse(p));
    }

    for (int i = 0; i < 5; ++i) {
        Point p = { getRandom(0, w), getRandom(0, h) };
        houses.push_back(new WoodenHouse(p));
    }

    for (House* house : houses) {
        threads.emplace_back(&House::move, house);
    }

    for (std::thread& t : threads)
        t.join();

    for (House* house : houses)
        delete house;

    return 0;
}