#include <SFML/Window/Event.hpp>

#include "playstate.h"
#include "gameengine.h"
#include "easylogging++.h"

PlayState::PlayState(GameEngine &game, std::string name)
  : GameState(game, name), desktop_(game_.screen, space_)
{
  space_.generate();
}

void PlayState::update(sf::Time delta)
{
  sf::Event event;
  if (game_.screen.isOpen())
  {
    while (game_.screen.pollEvent(event))
    {
      desktop_.handleEvents(event);
      if (event.type == sf::Event::Closed)
      {
        game_.quit();
      }
      else if (event.type == sf::Event::KeyPressed)
      {
        if (event.key.code == sf::Keyboard::Escape)
        {
          game_.quit();
        }
      }
    }
  }

  desktop_.update(delta);
  space_.update(delta);
  fps_ = 1.0f / (last_fps_time_.asSeconds());
  last_fps_time_ = delta;
  //dbg_window_.setFPS(fps_);
}

void PlayState::draw()
{
  game_.screen.clear();
  space_.draw(game_.screen);
  desktop_.draw();
  game_.screen.display();
}
