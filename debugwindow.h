#ifndef DEBUGWINDOW_H
#define DEBUGWINDOW_H

#include <SFGUI/SFGUI.hpp>


class DebugWindow
{
  public:
    DebugWindow(sf::RenderTarget &target);

    sfg::Window::Ptr getWindow() { return window_; }
    void setFPS(float fps);

    void zoomIn();
    void zoomOut();
  protected:
  private:
    sf::RenderTarget &screen_;
    sfg::Window::Ptr window_;
    sfg::Label::Ptr fps_label_;
    sfg::Label::Ptr mouse_label_;
    sfg::Box::Ptr box_;

    sfg::Button::Ptr zoom_in_;
    sfg::Button::Ptr zoom_out_;

    float fps_;
};

#endif // DEBUGWINDOW_H
