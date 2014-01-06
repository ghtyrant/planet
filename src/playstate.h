#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include <memory>

#include "gamestate.h"
#include "gamedesktop.h"
#include "space.h"

class PlayState : public GameState
{
  public:
    PlayState(GameEngine &game, std::string name);

    void update(sf::Time delta);
    void draw();
  protected:
  private:
    GameDesktop desktop_;
    float fps_;
    sf::Time last_fps_time_;
    Space space_; // Spaaaaceee!

};

#endif // PLAYSTATE_H
