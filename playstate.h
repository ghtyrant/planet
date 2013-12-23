#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include <memory>
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Button.hpp>

#include "gamestate.h"
#include "debugwindow.h"
#include "gamedesktop.h"

class PlayState : public GameState
{
  public:
    PlayState(GameEngine &game, std::string name);

    void update(sf::Time delta);
    void draw();

    void onButtonClick();
  protected:
  private:
    sfg::Button::Ptr button_;
    sfg::Window::Ptr window_;
    GameDesktop desktop_;
    DebugWindow dbg_window_;
    float fps_;
    sf::Time last_fps_time_;

};

#endif // PLAYSTATE_H
