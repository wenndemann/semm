#include "route.h"

Route::Route(size_t x, size_t y)
    : size(x*y),
      sizeX(x),
      nodes(new Node[size]) {

    //initialize WayPoints with neighbours and distances
    for(size_t i = 0; i < x; i++) {
        for(size_t j = 0; j < y; j++) {
            std::multimap<float, Node*>* n = new std::multimap<float, Node*>;
            int actNum = (j*x) + i;
            if(i >    0 ) n->insert(std::pair<float, Node*>(30.0f, &nodes[actNum - 1]));
            if(i < (x-1)) n->insert(std::pair<float, Node*>(30.0f, &nodes[actNum + 1]));
            if(j >    0 ) n->insert(std::pair<float, Node*>(30.0f, &nodes[actNum - x]));
            if(j < (y-1)) n->insert(std::pair<float, Node*>(30.0f, &nodes[actNum + x]));

            if(i >    0  && j >     0 ) n->insert(std::pair<float, Node*>(42.4f, &nodes[actNum - x-1]));
            if(i < (x-1) && j >     0 ) n->insert(std::pair<float, Node*>(42.4f, &nodes[actNum - x+1]));
            if(i >    0  && j < (y-1) ) n->insert(std::pair<float, Node*>(42.4f, &nodes[actNum + x-1]));
            if(i < (x-1) && j < (y-1) ) n->insert(std::pair<float, Node*>(42.4f, &nodes[actNum + x+1]));
            nodes[actNum].init(n, i, j);
        }
    }
}

Route::~Route() {
	delete[] nodes;
}

void Route::appendBack(int x, int y, bool mag) {
    m_list.push_back(WayPoint(x, y, mag));
}

void Route::appendFront(int x, int y, bool mag) {
    m_list.insert(m_list.begin(), WayPoint(x, y, mag));
}

WayPoint& Route::getElement(int num) {
    return m_list[num];
}

int Route::deleteElement(size_t num) {
    if(num > m_list.size()) return -1;
    m_list.erase(m_list.begin() + num);
    return m_list.size();
}

void Route::optimize() {
    if(this->getCount() < 2) return;

    int a = 0, b = 1;
    bool li, magEqual;

    while(b < (this->getCount() -1)) {
        magEqual = m_list[b].mag() == m_list[b+1].mag();
        li = pointsOnLine(m_list[a], m_list[b], m_list[b+1]);
        if(li && magEqual) {
            this->deleteElement(b);
        }
        else {
            b++;
            a=b-1;
        }
    }
}

void Route::calcPath(size_t startX, size_t startY, size_t tarX, size_t tarY) {
    size_t s = startY * sizeX + startX;
    size_t t = tarY * sizeX + tarX;

    //init
    nodes[s].distance = 0;
    nodes[s].blocked = false;
    nodes[t].blocked = false;

    //algorithm
    for(size_t n = 0; n < (size-1); n++) {
        for(size_t i = 0; i < size; i++) {
            Node* u = &nodes[i];

            std::multimap<float, Node*>::iterator n;
            for(n = nodes[i].neighbor->begin(); n != nodes[i].neighbor->end(); ++n) {
                Node* v = n->second;
                float weightUV = n-> first;
                if(u->distance + weightUV < v->distance && !v->blocked) {
                    v->distance = u->distance + weightUV;
                    v->predecessor = u;
                }
            }
        }
    }

    //add waypoints to list
    Node* act = &nodes[t];
    appendFront(act->posX, act->posY, true);
    while(act != &nodes[s]) {
        act = act->predecessor;
        appendFront(act->posX, act->posY, true);
    }
}

void Route::block(size_t x, size_t y) {
    //block special fields
    size_t s = y * sizeX + x;
    nodes[s].blocked = true;
}

bool Route::pointsOnLine(WayPoint &a, WayPoint &b, WayPoint &c) {
    //return true if points are in a line
    if(a.x() == b.x() && b.x() == c.x()) return true;
    if(a.y() == b.y() && b.y() == c.y()) return true;

    float temp = (a.x() - b.x());
    if(temp == 0.0f) return false;
    float m = (a.y() - b.y()) / temp;
    float n = a.y() - (m*a.x());
    return c.y() == (m * c.x() + n);
}

Node::Node()
    : blocked(false),
      distance(9999.0f),
      predecessor(NULL),
      posX(0),
      posY(0),
      neighbor(NULL){

}

void Node::init(std::multimap<float, Node*>* n, int x, int y) {
    neighbor = n;
    posX = x;
    posY = y;
}
