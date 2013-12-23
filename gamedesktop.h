#ifndef GAMEDESKTOP_H
#define GAMEDESKTOP_H

#include <SFGUI/SFGUI.hpp>

class GameDesktop
{
  public:
    GameDesktop();

    void addWindow(sfg::Window::Ptr window);
    void handleEvents(const sf::Event &event);
    void update(sf::Time delta);
    void draw(sf::RenderWindow &screen);

    void taskBarButtonClick(sfg::ToggleButton::Ptr button);
    void taskBarWindowFocus(sfg::ToggleButton::Ptr button);
  protected:
  private:
    sfg::SFGUI gui_;
    sfg::Desktop desktop_;
    sfg::Window::Ptr taskbar_;
    sfg::Box::Ptr box_;
    std::map<sfg::ToggleButton::Ptr, sfg::Window::Ptr> tasks_;
};

#endif // GAMEDESKTOP_H
