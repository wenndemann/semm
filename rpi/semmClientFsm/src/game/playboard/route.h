#ifndef ROUTE_H
#define ROUTE_H

#include <vector>
#include <map>
#include <iostream>

class Node
{
public:
    Node();
    void init(std::multimap<float, Node*>* n, int x, int y);

    bool blocked;
    float distance;
    Node* predecessor;

    std::multimap<float, Node*>* neighbor;
    int posX, posY;
};

class WayPoint
{
public:
    WayPoint()
        : m_x(0), m_y(0), m_mag(false) { }
    WayPoint(int x, int y, bool mag)
        : m_x(x), m_y(y), m_mag(mag) { }

    int x() { return m_x; }
    int y() { return m_y; }
    bool mag() { return m_mag; }

private:
    int m_x, m_y;
    bool m_mag;
};


typedef std::vector<WayPoint> WaypointVec;
typedef WaypointVec::iterator WaypointVecIt;

class Route
{
public:
    Route(size_t x = 11, size_t y = 11);
    ~Route();

    void appendBack(int x, int y, bool mag);
    void appendFront(int x, int y, bool mag);
    int getCount() { return m_list.size(); }
    WayPoint& getElement(int num);
    int deleteElement(size_t num);
    void optimize();
    void calcPath(size_t startX, size_t startY, size_t tarX, size_t tarY);
    void block(size_t x, size_t y);
    bool pointsOnLine(WayPoint& a, WayPoint& b, WayPoint& c);

    Node* getNodes() { return nodes; }
    size_t getSize() { return size; }
    size_t getSizeX() { return sizeX; }

    WaypointVec& getList() { return m_list; }

private:
    WaypointVec m_list;

    size_t size, sizeX;
    Node* nodes;
};


#endif // ROUTE_H
