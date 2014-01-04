#include "playstate.h"
#include "gameengine.h"
#include "easylogging++.h"

PlayState::PlayState(GameEngine &game, std::string name)
  : GameState(game, name), desktop_(game_.screen, space_), dbg_window_(game_.screen)
{
  button_ = sfg::Button::Create("Testing");
  button_->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&PlayState::onButtonClick, this));

  sim_button_ = sfg::Button::Create("Sim. Step");
  sim_button_->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&PlayState::onSimButtonClick, this));

  window_ = sfg::Window::Create();
  window_->SetTitle("Hello World!");
  //window_->Add(button_);
  window_->Add(sim_button_);

  desktop_.addWindow(window_);
  desktop_.addWindow(dbg_window_.getWindow());

  space_.generate();

  // Simulate the universe for 100.000 years
  /*for (unsigned int i = 0; i < 100; i++)
  {
    if (i % 1000 == 0)
      LOG(INFO) << "Simulated for " << i << " years ...";
    space_.simulate();
  }*/
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
  dbg_window_.setFPS(fps_);
}

void PlayState::draw()
{
  game_.screen.clear();
  space_.draw(game_.screen);
  desktop_.draw();
  game_.screen.display();
}

void PlayState::onButtonClick()
{
  button_->SetLabel("Hihihihi!");
  game_.quit();
}

void PlayState::onSimButtonClick()
{
  space_.simulate();
}
