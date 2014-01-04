#ifndef GAMEDESKTOP_H
#define GAMEDESKTOP_H

#include <SFGUI/SFGUI.hpp>

class Space;
class CelestialObject;

class GameDesktop
{
  public:
    GameDesktop(sf::RenderWindow &screen, Space &space);

    void addWindow(sfg::Window::Ptr window);
    void handleEvents(const sf::Event &event);
    void update(sf::Time delta);
    void draw();

    void taskBarButtonClick(sfg::ToggleButton::Ptr button);
    void taskBarWindowFocus(sfg::ToggleButton::Ptr button);
  protected:
  private:

    bool isMouseOverUI() const;
    bool isMouseInRenderWindow() const;

    sf::Vector2u screen_size_;
    sf::RenderWindow& screen_;
    Space &space_;
    sfg::SFGUI gui_;
    sfg::Desktop desktop_;
    sfg::Window::Ptr taskbar_;
    sfg::Box::Ptr box_;
    std::map<sfg::ToggleButton::Ptr, sfg::Window::Ptr> tasks_;
    float zoom_;
    bool mouse_left_down_;
    bool gui_handled_click_;
    sf::Vector2i mouse_drag_start_position_;

    std::shared_ptr<CelestialObject> active_object_;
    unsigned char active_object_flash_;
    char active_object_flash_dir_;
};

#endif // GAMEDESKTOP_H
