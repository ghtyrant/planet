#include "gameengine.h"
#include "gamestate.h"
#include "easylogging++.h"

GameEngine::GameEngine(const std::string& title, const unsigned int width, const unsigned int height)
  : running_(false)
{
  LOG(INFO) << "Creating rendering environment";
  screen.create(sf::VideoMode(width, height), title);
  screen.setFramerateLimit(40);
}

bool GameEngine::running() const
{
  return running_;
}

void GameEngine::quit()
{
  LOG(INFO) << "Shutting down engine";
  running_ = false;
}

void GameEngine::draw()
{
  if (!states_.empty() && running_)
    states_.top()->draw();
}

void GameEngine::update(sf::Time delta)
{
  std::unique_ptr<GameState> temp = states_.top()->next();
  if (temp != nullptr)
  {
    states_.pop();
    states_.push(std::move(temp));
  }

  if (!states_.empty() && running_)
    states_.top()->update(delta);
}

void GameEngine::run(std::unique_ptr<GameState> state)
{
  running_ = true;
  states_.push(std::move(state));
}