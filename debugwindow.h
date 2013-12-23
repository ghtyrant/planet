#ifndef DEBUGWINDOW_H
#define DEBUGWINDOW_H

#include <SFGUI/SFGUI.hpp>


class DebugWindow
{
  public:
    DebugWindow();

    sfg::Window::Ptr getWindow() { return window_; }
    void setFPS(float fps);
  protected:
  private:
    sfg::Window::Ptr window_;
    sfg::Label::Ptr label_;
    float fps_;
};

#endif // DEBUGWINDOW_H
