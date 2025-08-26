#ifndef NODE_H
#define NODE_H

#include <vector>
#include <atomic>

template <typename T>
class Node {
private:
    static std::atomic<int> global_id;
    int id;
    std::vector<T> body;

public:
    // Constructor
    Node(const std::vector<T>& b = {}) : body(b) {
        id = global_id++;
    }

    // Accessors
    std::vector<T> get() const { return body; }
    int getId() const { return id; }

    // Mutators
    void set(const std::vector<T>& b) { body = b; }
};

// Initialize static ID counter
template <typename T>
std::atomic<int> Node<T>::global_id{0};

// Factory function
template <typename T>
Node<T> createNode(const std::vector<T>& body = {}) {
    return Node<T>(body);
}

#endif // NODE_H
