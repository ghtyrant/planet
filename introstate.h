#ifndef INTROSTATE_H
#define INTROSTATE_H

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include "gamestate.h"

class GameEngine;

class IntroState : public GameState
{
  public:
    IntroState(GameEngine &game, std::string name);

    void update(sf::Time delta);
    void draw();

  private:
    sf::Font font_;
    sf::Text text_;
    sf::Time time_;
    float angle_;
    float scale_;
};

#endif // INTROSTATE_H
