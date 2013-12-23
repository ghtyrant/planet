#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <memory>
#include <string>
#include <SFML/System/Time.hpp>

#include "easylogging++.h"

class GameEngine;

class GameState
{
public:
  GameState(GameEngine &game, std::string name) : game_(game), name_(name) {LOG(INFO) << "Starting up state '" << name_ << "'!";}

  virtual void update(sf::Time delta) = 0;
  virtual void draw() = 0;

  std::unique_ptr<GameState> next() { return std::move(next_); }

protected:
  GameEngine& game_;
  std::string name_;
  std::unique_ptr<GameState> next_;
};

#endif // GAMESTATE_H
