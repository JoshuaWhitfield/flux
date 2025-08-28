#ifndef EDGE_H
#define EDGE_H

#include <vector>
#include <string>
#include <sstream>
#include <atomic>

template <typename T>
class Edge {
private:
    static std::atomic<int> global_id;
    int id;
    T value;
    T origin;
    std::vector<T> history;

public:
    // Constructor
    Edge(const T& val) : value(val), origin(val) {
        id = global_id++;
        history.push_back(val);
    }

    // Accessors
    const T& get() const { return value; }
    T getOrigin() const { return origin; }
    std::vector<T> getHistory() const { return history; }
    size_t getLength() const { return history.size(); }
    int getId() const { return id; }

    // Mutators
    void set(const T& val) {
        value = val;
        history.push_back(val);
    }

    void reset() {
        value = history.front();
        origin = history.front();
        history.clear();
        history.push_back(value);
    }
};

// Initialize static ID counter
template <typename T>
std::atomic<int> Edge<T>::global_id{0};

// Factory function
template <typename T>
Edge<T> createEdge(const T& value) {
    return Edge<T>(value);
}

#endif // EDGE_H
