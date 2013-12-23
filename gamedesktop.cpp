#include "gamedesktop.h"
#include "easylogging++.h"

GameDesktop::GameDesktop()
{


  // Create our taskbar
  taskbar_ = sfg::Window::Create(0);
  box_ = sfg::Box::Create();
  box_->SetSpacing(0.2f);
  taskbar_->Add(box_);

  // Put the taskbar at the bottom of the screen
  sf::FloatRect rect = taskbar_->GetAllocation();
  taskbar_->SetPosition(sf::Vector2f(0, 600 - rect.height * 2));
  desktop_.Add(taskbar_);

}

void GameDesktop::addWindow(sfg::Window::Ptr window)
{
  auto button = sfg::ToggleButton::Create(window->GetTitle());
  tasks_[button] = window;
  button->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&GameDesktop::taskBarButtonClick, this, button));
  window->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&GameDesktop::taskBarWindowFocus, this, button));
  box_->Pack(button);
  desktop_.Add(window);
}

void GameDesktop::handleEvents(const sf::Event &event)
{
  desktop_.HandleEvent(event);
}

void GameDesktop::update(sf::Time delta)
{
  desktop_.Update(delta.asSeconds());
}

void GameDesktop::draw(sf::RenderWindow &screen)
{
  gui_.Display(screen);
}

void GameDesktop::taskBarButtonClick(sfg::ToggleButton::Ptr button)
{
  LOG(INFO) << "taskBarButtonClick got focus!";
  for (auto it = tasks_.begin(); it != tasks_.end(); it++)
    if (it->first != button)
      it->first->SetActive(false);

  desktop_.BringToFront(tasks_[button]);
}

void GameDesktop::taskBarWindowFocus(sfg::ToggleButton::Ptr button)
{
  if (!tasks_[button]->IsActiveWidget())
  {
    LOG(INFO) << "taskBarButtonClick got focus!";
    for (auto it = tasks_.begin(); it != tasks_.end(); it++)
      if (it->first != button)
        it->first->SetActive(false);
      else
        it->first->SetActive(true);
  }
}
