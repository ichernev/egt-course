#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include <exception>
// #include <unistd.h>

struct RoadSegment {
    double begining; // meters
    int minSpeed; // km/h
    int maxSpeed; // km/h
};

class Road {
public:
    Road(double meters, int numSegments) {
        double segLen = meters / numSegments;
        for (int i = 0; i < numSegments; ++i) {
            int maxSpeed = 50 + rand() % 50;
            segments.push_back(RoadSegment {
                    i * segLen,
                    maxSpeed - 10,
                    maxSpeed,
            });
        }
        segments.push_back(RoadSegment { meters, 0, 0 });
    }
    int minSpeedAt(double meters) const {
        return getSegmentAt(meters)->minSpeed;
    }
    int maxSpeedAt(double meters) const {
        return getSegmentAt(meters)->maxSpeed;
    }
    int totalLength() const {
        return segments[segments.size()-1].begining;
    }
private:
    const RoadSegment *getSegmentAt(double meters) const {
        if (meters >= totalLength()) {
            return NULL;
        }
        for (int i = 0; i < segments.size(); ++i) {
            if (segments[i].begining <= meters &&
                    segments[i+1].begining > meters) {
                return &segments[i];
            }
        }
        return NULL;
    }

    std::vector<RoadSegment> segments;
};

class Car {
public:
    Car(std::string name) : road(NULL), name(name), meters(0) {}
    void assignRoad(Road *road) {
        if (this->road != NULL) {
            throw "can't override road";
        }
        this->road = road;
    }
    bool hasFinished() {
        return road == NULL;
    }
    void tick(double sec) {
        if (hasFinished()) {
            return;
        }

        int maxSpeed = road->maxSpeedAt(meters);
        int roadLen = road->totalLength();
        double dist = maxSpeed * (sec / 3600) * 1000;
        std::cout << "Car " << this->name << " traveled " << dist <<
            " with " << maxSpeed << std::endl;
        meters += dist;
        if (meters >= roadLen) {
            std::cout << "Car " << this->name << " reached end of road"
                << std::endl;
            road = NULL;
            meters = 0;
        } else {
            std::cout << "Car " << this->name << " at "
                << meters << "/" << roadLen << " on road" << std::endl;
        }
    }
private:
    std::string name;
    Road *road;
    double meters;
};

class Controller {
public:
    void addCar() {
        // Car *p = new Car(unique_name());
        // cars.emplace_back(p);
        std::unique_ptr<Car> p = std::make_unique<Car>(unique_name());
        cars.push_back(std::move(p));
    }
    void addRoad() {
        roads.emplace_back(std::make_unique<Road>(1000, 2));
    }

    void loop() {
        for (auto &car : cars) {
            if (car->hasFinished()) {
                car->assignRoad(roads[rand() % roads.size()].get());
            }
        }
        for (auto &car : cars) {
            car->tick(10);
        }
    }
private:
    std::string unique_name() {
        std::ostringstream os;
        os << "car-id-" << carId++;
        return os.str();
    }

    int carId;

    double timerInterval;
    std::vector<std::unique_ptr<Car>> cars;
    std::vector<std::unique_ptr<Road>> roads;
};

int main() {
    Controller ctl;
    ctl.addCar();
    ctl.addRoad();

    for (int i = 0; i < 10; ++i) {
        ctl.loop();
        // sleep(1);
    }

    return 0;
}
