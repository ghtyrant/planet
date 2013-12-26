#include "gamedesktop.h"
#include "easylogging++.h"
#include "consts.h"
#include "celestialobject.h"
#include "space.h"

GameDesktop::GameDesktop(sf::RenderWindow &screen, Space &space)
  : screen_(screen),
    space_(space),
    zoom_(1.0f),
    mouse_left_down_(false),
    active_object_flash_(50),
    active_object_flash_dir_(1)
{
  // Create our taskbar
  taskbar_ = sfg::Window::Create(0);
  box_ = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 0.2f);
  taskbar_->Add(box_);

  screen_size_ = screen.getSize();

  // Put the taskbar at the bottom of the screen
  sf::FloatRect rect = taskbar_->GetAllocation();
  taskbar_->SetPosition(sf::Vector2f(0, screen_size_.y - rect.height * 2));
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
  if (event.type == sf::Event::MouseWheelMoved)
  {
    sf::View tmp = screen_.getView();
    if (event.mouseWheel.delta > 0)
    {
      tmp.zoom(0.9f);
      zoom_ *= 0.9f;
    }
    else
    {
      tmp.zoom(1.1f);
      zoom_ *= 1.1f;
    }
    screen_.setView(tmp);
  }
  else
    desktop_.HandleEvent(event);
}

void GameDesktop::update(sf::Time delta)
{
  // Single Click
  if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && !mouse_left_down_)
  {
    mouse_left_down_ = true;
    mouse_drag_last_position_ = sf::Mouse::getPosition(screen_);
    mouse_drag_start_position_ = sf::Mouse::getPosition(screen_);
  }

  // Release after single click
  if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && mouse_left_down_)
  {
    sf::Vector2i window_pos = sf::Mouse::getPosition(screen_);
    sf::Vector2f world_pos = screen_.mapPixelToCoords(window_pos);

    std::shared_ptr<CelestialObject> tmp = space_.coordsOverObject(world_pos);
    if (tmp != nullptr)
    {
      LOG(INFO) << "Selected " << tmp->getName();
      if (active_object_ != nullptr)
        active_object_->setColor(sf::Color(255, 255, 255));

      active_object_ = tmp;
      active_object_flash_ = 50;
      active_object_flash_dir_ = 5;
    }

    mouse_left_down_ = false;
  }

  // Long click (for dragging)
  if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && mouse_left_down_)
  {
    sf::Vector2i drag_delta = sf::Mouse::getPosition(screen_) - mouse_drag_start_position_;
    if (drag_delta.x != 0 || drag_delta.y != 0)
    {
      sf::View tmp = screen_.getView();
      tmp.move(-drag_delta.x, -drag_delta.y);
      screen_.setView(tmp);
    }
    sf::Mouse::setPosition(mouse_drag_start_position_, screen_);
  }

  // Update selected object
  if (active_object_ != nullptr)
  {
    active_object_flash_ += active_object_flash_dir_;
    if (active_object_flash_ >= 255)
      active_object_flash_dir_ = -5;
    if (active_object_flash_ <= 100)
      active_object_flash_dir_ = 5;

    active_object_->setColor(sf::Color(50, active_object_flash_, 50));
  }

  desktop_.Update(delta.asSeconds());
}

void GameDesktop::draw()
{
  gui_.Display(screen_);
}

void GameDesktop::taskBarButtonClick(sfg::ToggleButton::Ptr button)
{
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
