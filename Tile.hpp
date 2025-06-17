//Abstract class, pure virtual function definition Tile
#ifndef TILE_HPP
#define TILE_HPP

#include <string>
#include <memory>

class Tile {
protected:
    int id;
    std::string name;

public:
    Tile(int id, const std::string& name) : id(id), name(name) {}
    virtual ~Tile() = default;

    virtual void displayInfo() const = 0;

    int getId() const { return id; }
    std::string getName() const { return name; }
    virtual std::string getTypeString() const=0; 
};

#endif