#ifndef GAMEDESKTOP_H
#define GAMEDESKTOP_H

#include <memory>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GLRenderer.h>

class Space;
class CelestialObject;

class GameDesktop
{
  public:
    GameDesktop(sf::RenderWindow &screen, Space &space);

    //void addWindow(sfg::Window::Ptr window);
    void handleEvents(const sf::Event &event);
    void update(sf::Time delta);
    void draw();

  protected:
  private:

    void initializeKeyMap();
    CEGUI::Key::Scan toCEGUIKey(sf::Keyboard::Key code);
    CEGUI::MouseButton toCEGUIMouseButton(sf::Mouse::Button button);

    bool isMouseInRenderWindow() const;

    sf::Vector2u screen_size_;
    sf::RenderWindow& screen_;
    Space &space_;
    CEGUI::OpenGLRenderer& renderer_;
    CEGUI::WindowManager* wm_;

    //std::map<sfg::ToggleButton::Ptr, sfg::Window::Ptr> tasks_;
    float zoom_;
    bool mouse_left_down_;
    bool gui_handled_click_;
    sf::Vector2i mouse_drag_start_position_;

    std::shared_ptr<CelestialObject> active_object_;
    unsigned char active_object_flash_;
    char active_object_flash_dir_;
    std::map<sf::Keyboard::Key, CEGUI::Key::Scan> key_map_;
    std::map<sf::Mouse::Button, CEGUI::MouseButton> mouse_map_;
};

#endif // GAMEDESKTOP_H
