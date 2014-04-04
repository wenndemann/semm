#ifndef PIMPL_H
#define PIMPL_H

#include <iostream>
#include <map>

class MainWindow;
class PlayboardView;
class PlayboardScene;

namespace boost
{
    class thread;
}

class pimpl
{
public:
    pimpl();
    virtual ~pimpl();

    void addMeeples( uint8_t color, std::map< uint16_t, uint8_t > tagsFields );

    void setMeeplePos( uint8_t color, uint16_t tag, uint8_t toFieldId );

private:
    void worker( );

    MainWindow* _mainWindow;

    boost::thread* _thread;
};

#endif // PIMPL_H
