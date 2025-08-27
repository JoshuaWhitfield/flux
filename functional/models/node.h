#ifndef NODE_H
#define NODE_H

#include <map>
#include <string>
#include <any>
#include <atomic>
#include <stdexcept>

class Node {
private:
    static std::atomic<int> global_id;
    int id;
    std::map<std::string, std::any> body;

public:
    // Constructor
    Node() {
        id = global_id++;
    }

    // Accessors
    std::map<std::string, std::any> get() const { return body; }
    int getId() const { return id; }

    // Mutators
    void set(const std::map<std::string, std::any>& b) { body = b; }

    // Indexing by string
    std::any& operator[](const std::string& key) { return body[key]; }
    const std::any& operator[](const std::string& key) const { 
        auto it = body.find(key);
        if (it == body.end()) throw std::runtime_error("Key not found: " + key);
        return it->second;
    }

    // Typed access helper
    template <typename T>
    T& getAs(const std::string& key) {
        return *std::any_cast<T>(&body[key]);
    }

    template <typename T>
    const T& getAs(const std::string& key) const {
        return *std::any_cast<const T>(&body.at(key));
    }
};

// Initialize static ID counter
std::atomic<int> Node::global_id{0};

// Factory function to create a new Node
inline Node createNode() {
    return Node();
}

#endif // NODE_H
