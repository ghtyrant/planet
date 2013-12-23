#include "debugwindow.h"

DebugWindow::DebugWindow()
{
  window_ = sfg::Window::Create();
  window_->SetTitle("Debug");
  label_ = sfg::Label::Create("Testing");

  window_->Add(label_);
}

void DebugWindow::setFPS(float fps)
{
  fps_ = fps;
  std::ostringstream out;
  out << "FPS: " << fps_;
  std::string str(out.str());
  label_->SetText(str);
}
