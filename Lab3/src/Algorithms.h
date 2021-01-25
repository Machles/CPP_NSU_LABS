#ifndef LAB3_ALGORITHMS_H
#define LAB3_ALGORITHMS_H

#include <queue>
#include <cstdlib>
#include "Robot.h"

class Algorithms {
public:
    bool dijikstra_algorithm(Robot* robot, const Coordinates& startPoint, Coordinates& endPoint, CELL goalCell, std::unordered_map<Coordinates, Coordinates>& came_from, std::unordered_map<Coordinates, long>& cost_so_far);
    bool astar_algorithm(Robot* robot, const Coordinates& startPoint, const Coordinates& endPoint, std::unordered_map<Coordinates, Coordinates>& came_from, std::unordered_map<Coordinates, long>& cost_so_far);

    inline long heuristic(const Coordinates& a, const Coordinates& b);  
    vector<Coordinates> getCellNeighbours(Map * robotInternalMap, const Coordinates& cellCoordinates);
    int getCostForCell(Map* robotInternalMap, const Coordinates& cellCoordinates, Robot * robot);
    std::vector<Coordinates> getPath(const Coordinates& startPoint,const Coordinates& endPoint, std::unordered_map<Coordinates, Coordinates> came_from, CELL type);
};

namespace std {
    template <> 
    struct hash<Coordinates> {
        typedef Coordinates argument_type;
        typedef std::size_t result_type;
        std::size_t operator()(const Coordinates& id) const noexcept {
            return std::hash<int>()(id.x ^ (id.y << 4));
        }
    };
}

template<typename T, typename priority_t>
struct PriorityQueue {
    typedef std::pair<priority_t, T> PQElement;

    std::priority_queue<PQElement, std::vector<PQElement>, std::greater<PQElement>> elements;

    inline void put(T item, priority_t priority) {
        elements.emplace(priority, item);
    }
    
    inline bool empty() const {
        return elements.empty();
    }

    T get() {
        T box = elements.top().second;
        elements.pop();
        return box;
    }
};

#endif