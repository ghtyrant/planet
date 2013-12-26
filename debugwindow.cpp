#include "debugwindow.h"

DebugWindow::DebugWindow(sf::RenderTarget &target)
  : screen_(target)
{
  window_ = sfg::Window::Create();
  window_->SetTitle("Debug");
  box_ = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 0.2f);

  fps_label_ = sfg::Label::Create("Testing");
  mouse_label_ = sfg::Label::Create("Mousepos");

  zoom_in_ = sfg::Button::Create("Zoom Out");
  zoom_out_ = sfg::Button::Create("Zoom In");

  box_->Pack(fps_label_);
  box_->Pack(mouse_label_);
  box_->Pack(zoom_in_);
  box_->Pack(zoom_out_);

  window_->Add(box_);

  zoom_in_->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&DebugWindow::zoomIn, this));
  zoom_out_->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&DebugWindow::zoomOut, this));
}

void DebugWindow::setFPS(float fps)
{
  fps_ = fps;
  std::ostringstream out;
  out << "FPS: " << fps_;
  std::string str(out.str());
  fps_label_->SetText(str);

  sf::Vector2i pos = sf::Mouse::getPosition();
  std::ostringstream out2;
  out2 << "X: " << pos.x << ", " << pos.y;
  std::string str2(out2.str());
  mouse_label_->SetText(str2);
}

void DebugWindow::zoomIn()
{
  sf::View tmp = screen_.getView();
  tmp.zoom(0.9f);
  screen_.setView(tmp);
}

void DebugWindow::zoomOut()
{
  sf::View tmp = screen_.getView();
  tmp.zoom(1.1f);
  screen_.setView(tmp);
}
