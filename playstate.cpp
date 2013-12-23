#include "playstate.h"
#include "gameengine.h"
#include "easylogging++.h"

PlayState::PlayState(GameEngine &game, std::string name)
  : GameState(game, name)//, fps_(0)
{
  button_ = sfg::Button::Create("Testing");
  button_->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&PlayState::onButtonClick, this));

  window_ = sfg::Window::Create();
  window_->SetTitle("Hello World!");
  window_->Add(button_);

  desktop_.addWindow(window_);
  desktop_.addWindow(dbg_window_.getWindow());
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
        game_.quit();
      else if (event.type == sf::Event::KeyPressed)
        if (event.key.code == sf::Keyboard::Escape)
          game_.quit();
    }
  }

  fps_ = 1.0f / (last_fps_time_.asSeconds());
  last_fps_time_ = delta;
  dbg_window_.setFPS(fps_);
  desktop_.update(delta);
}

void PlayState::draw()
{
  game_.screen.clear();
  desktop_.draw(game_.screen);
  game_.screen.display();
}

void PlayState::onButtonClick()
{
  button_->SetLabel("Hihihihi!");
  game_.quit();
}
