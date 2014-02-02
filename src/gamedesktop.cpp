#include <SFML/Window/Event.hpp>

#include "gamedesktop.h"
#include "easylogging++.h"
#include "consts.h"
#include "celestialobject.h"
#include "space.h"

GameDesktop::GameDesktop(sf::RenderWindow &screen, Space &space)
  : screen_(screen),
    space_(space),
    renderer_(CEGUI::OpenGLRenderer::bootstrapSystem()),
    zoom_(1.0f),
    mouse_left_down_(false),
    gui_handled_click_(false),
    has_focus_(false),
    active_object_flash_(50),
    active_object_flash_dir_(1)
{
  screen_size_ = screen.getSize();

  // Set up default resource groups
  CEGUI::DefaultResourceProvider *rp = static_cast<CEGUI::DefaultResourceProvider*>(CEGUI::System::getSingleton().getResourceProvider());

  rp->setResourceGroupDirectory("schemes", "/usr/share/cegui-0/schemes/"); 
  rp->setResourceGroupDirectory("imagesets", "/usr/share/cegui-0/imagesets/");
  rp->setResourceGroupDirectory("fonts", "/usr/share/cegui-0/fonts/");
  rp->setResourceGroupDirectory("layouts", "/usr/share/cegui-0/layouts/");
  rp->setResourceGroupDirectory("looknfeels", "/usr/share/cegui-0/looknfeel");
  rp->setResourceGroupDirectory("lua_scripts", "/usr/share/cegui-0/lua_scripts/");

  CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
  CEGUI::Font::setDefaultResourceGroup("fonts");
  CEGUI::Scheme::setDefaultResourceGroup("schemes");
  CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
  CEGUI::WindowManager::setDefaultResourceGroup("layouts");
  CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");

  // Set up the GUI
  CEGUI::SchemeManager::getSingleton().createFromFile("WindowsLook.scheme");
  CEGUI::FontManager::getSingleton().createFromFile("DejaVuSans-10.font");

  CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("WindowsLook/MouseArrow");

  CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
  default_window_ = wmgr.createWindow("DefaultWindow", "root");
  default_window_->setProperty("MousePassThroughEnabled", "True");
  //root->setSize(CEGUI::USize(screen_size_.x, screen_size_.y));
  CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(default_window_);

  CEGUI::FrameWindow *fw = static_cast<CEGUI::FrameWindow*>(wmgr.createWindow("WindowsLook/FrameWindow", "testWindow"));

  default_window_->addChild(fw);
  fw->setText("Hallo Welt!");
  // Create our taskbar
  /*taskbar_ = sfg::Window::Create(0);
  box_ = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 0.2f);
  taskbar_->Add(box_);


  // Put the taskbar at the bottom of the screen
  sf::FloatRect rect = taskbar_->GetAllocation();
  taskbar_->SetPosition(sf::Vector2f(0, screen_size_.y - rect.height * 2));
  desktop_.Add(taskbar_);*/

  //addWindow(dbg_window_.getWindow());

  initializeKeyMap();
  LOG(INFO) << "Initialized GameDesktop";
}

/*void GameDesktop::addWindow(sfg::Window::Ptr window)
{
  auto button = sfg::ToggleButton::Create(window->GetTitle());
  button->SetRequisition(sf::Vector2f(150.0f, 0.0f));
  button->RequestResize();

  tasks_[button] = window;
  button->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&GameDesktop::taskBarButtonClick, this, button));
  window->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&GameDesktop::taskBarWindowFocus, this, button));
  box_->Pack(button);
  desktop_.Add(window);
}*/

void GameDesktop::handleEvents(const sf::Event &event)
{
  bool handled = false;
  CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();

  // Inject CEGUI events
  switch (event.type)
  {
  case sf::Event::TextEntered:
    handled = context.injectChar(event.text.unicode); break;
  case sf::Event::KeyPressed:
    handled = context.injectKeyDown(toCEGUIKey(event.key.code)); break;
  case sf::Event::KeyReleased:
    handled = context.injectKeyUp(toCEGUIKey(event.key.code)); break;
  case sf::Event::MouseMoved:
    handled = context.injectMousePosition(static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y)); break;
  case sf::Event::MouseButtonPressed:
    handled = context.injectMouseButtonDown(toCEGUIMouseButton(event.mouseButton.button)); break;
  case sf::Event::MouseButtonReleased:
    handled = context.injectMouseButtonUp(toCEGUIMouseButton(event.mouseButton.button)); break;
  case sf::Event::MouseWheelMoved:
    handled = context.injectMouseWheelChange(static_cast<float>(event.mouseWheel.delta)); break;
  case sf::Event::Resized: CEGUI::System::getSingleton().notifyDisplaySizeChanged(CEGUI::Sizef(event.size.width, event.size.height)); handled = true; break;
  default: handled = false;
  }

  if (event.type == sf::Event::LostFocus || event.type == sf::Event::GainedFocus)
  {
    mouse_left_down_ = false; 

    has_focus_ = (event.type == sf::Event::GainedFocus ? true : false);

    if (has_focus_)
      LOG(INFO) << "App gained focus!";
    else
      LOG(INFO) << "App lost focus!";
  }

  // If mouse is over an UI element, we do not initiate drag scrolling
  if (!handled && isMouseInRenderWindow() && has_focus_)
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

    if (event.type == sf::Event::MouseButtonReleased)
    {
      if (event.mouseButton.button == sf::Mouse::Left)
      {
        // Release after single click, selecting stuff
        if (mouse_left_down_)
        {
          sf::Vector2i window_pos = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
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
      }
    }
  }
}

void GameDesktop::update(sf::Time delta)
{
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

  CEGUI::System::getSingleton().getDefaultGUIContext().injectTimePulse(delta.asSeconds());
 
  // update scrolling
  if (isMouseInRenderWindow() && CEGUI::System::getSingleton().getDefaultGUIContext().getWindowContainingMouse() == default_window_)
    updateScrolling();

  //desktop_.Update(delta.asSeconds());
}

void GameDesktop::updateScrolling()
{
  if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && !mouse_left_down_)
  {
    mouse_left_down_ = true;
    mouse_drag_start_position_ = sf::Mouse::getPosition(screen_);
  }

  if (mouse_left_down_)
  {
    sf::Vector2i mouse_pos = sf::Mouse::getPosition(screen_);
    sf::Vector2i drag_delta = mouse_pos - mouse_drag_start_position_;
    if (drag_delta.x != 0 || drag_delta.y != 0)
    {
      LOG(INFO) << "Updateing view ...";
      sf::View tmp = screen_.getView();
      tmp.move(-drag_delta.x, -drag_delta.y);
      screen_.setView(tmp);
    }
    sf::Mouse::setPosition(mouse_drag_start_position_, screen_);
  }
}

void GameDesktop::draw()
{
  // resetGLStates to not get CEGUI windows transformed by SFML
  screen_.resetGLStates();
  CEGUI::System::getSingleton().renderAllGUIContexts();
}

/*void GameDesktop::taskBarButtonClick(sfg::ToggleButton::Ptr button)
{
  for (auto& it : tasks_)
    if (it.first != button)
      it.first->SetActive(false);

  desktop_.BringToFront(tasks_[button]);
}

void GameDesktop::taskBarWindowFocus(sfg::ToggleButton::Ptr button)
{
  if (!tasks_[button]->IsActiveWidget())
  {
    LOG(INFO) << "taskBarButtonClick got focus!";
    for (auto& it : tasks_)
      if (it.first != button)
        it.first->SetActive(false);
      else
        it.first->SetActive(true);
  }
}*/

void GameDesktop::initializeKeyMap()
{
  key_map_[sf::Keyboard::A]         = CEGUI::Key::A;
  key_map_[sf::Keyboard::B]         = CEGUI::Key::B;
  key_map_[sf::Keyboard::C]         = CEGUI::Key::C;
  key_map_[sf::Keyboard::D]         = CEGUI::Key::D;
  key_map_[sf::Keyboard::E]         = CEGUI::Key::E;
  key_map_[sf::Keyboard::F]         = CEGUI::Key::F;
  key_map_[sf::Keyboard::G]         = CEGUI::Key::G;
  key_map_[sf::Keyboard::H]         = CEGUI::Key::H;
  key_map_[sf::Keyboard::I]         = CEGUI::Key::I;
  key_map_[sf::Keyboard::J]         = CEGUI::Key::J;
  key_map_[sf::Keyboard::K]         = CEGUI::Key::K;
  key_map_[sf::Keyboard::L]         = CEGUI::Key::L;
  key_map_[sf::Keyboard::M]         = CEGUI::Key::M;
  key_map_[sf::Keyboard::N]         = CEGUI::Key::N;
  key_map_[sf::Keyboard::O]         = CEGUI::Key::O;
  key_map_[sf::Keyboard::P]         = CEGUI::Key::P;
  key_map_[sf::Keyboard::Q]         = CEGUI::Key::Q;
  key_map_[sf::Keyboard::R]         = CEGUI::Key::R;
  key_map_[sf::Keyboard::S]         = CEGUI::Key::S;
  key_map_[sf::Keyboard::T]         = CEGUI::Key::T;
  key_map_[sf::Keyboard::U]         = CEGUI::Key::U;
  key_map_[sf::Keyboard::V]         = CEGUI::Key::V;
  key_map_[sf::Keyboard::W]         = CEGUI::Key::W;
  key_map_[sf::Keyboard::X]         = CEGUI::Key::X;
  key_map_[sf::Keyboard::Y]         = CEGUI::Key::Y;
  key_map_[sf::Keyboard::Z]         = CEGUI::Key::Z;
  key_map_[sf::Keyboard::Num0]      = CEGUI::Key::Zero;
  key_map_[sf::Keyboard::Num1]      = CEGUI::Key::One;
  key_map_[sf::Keyboard::Num2]      = CEGUI::Key::Two;
  key_map_[sf::Keyboard::Num3]      = CEGUI::Key::Three;
  key_map_[sf::Keyboard::Num4]      = CEGUI::Key::Four;
  key_map_[sf::Keyboard::Num5]      = CEGUI::Key::Five;
  key_map_[sf::Keyboard::Num6]      = CEGUI::Key::Six;
  key_map_[sf::Keyboard::Num7]      = CEGUI::Key::Seven;
  key_map_[sf::Keyboard::Num8]      = CEGUI::Key::Eight;
  key_map_[sf::Keyboard::Num9]      = CEGUI::Key::Nine;
  key_map_[sf::Keyboard::Escape]    = CEGUI::Key::Escape;
  key_map_[sf::Keyboard::LControl]  = CEGUI::Key::LeftControl;
  key_map_[sf::Keyboard::LShift]    = CEGUI::Key::LeftShift;
  key_map_[sf::Keyboard::LAlt]      = CEGUI::Key::LeftAlt;
  key_map_[sf::Keyboard::LSystem]   = CEGUI::Key::LeftWindows;
  key_map_[sf::Keyboard::RControl]  = CEGUI::Key::RightControl;
  key_map_[sf::Keyboard::RShift]    = CEGUI::Key::RightShift;
  key_map_[sf::Keyboard::RAlt]      = CEGUI::Key::RightAlt;
  key_map_[sf::Keyboard::RSystem]   = CEGUI::Key::RightWindows;
  key_map_[sf::Keyboard::LBracket]  = CEGUI::Key::LeftBracket;
  key_map_[sf::Keyboard::RBracket]  = CEGUI::Key::RightBracket;
  key_map_[sf::Keyboard::SemiColon] = CEGUI::Key::Semicolon;
  key_map_[sf::Keyboard::Comma]     = CEGUI::Key::Comma;
  key_map_[sf::Keyboard::Period]    = CEGUI::Key::Period;
  key_map_[sf::Keyboard::Quote]     = CEGUI::Key::Apostrophe;
  key_map_[sf::Keyboard::Slash]     = CEGUI::Key::Slash;
  key_map_[sf::Keyboard::BackSlash] = CEGUI::Key::Backslash;
  key_map_[sf::Keyboard::Tilde]     = CEGUI::Key::Grave;
  key_map_[sf::Keyboard::Equal]     = CEGUI::Key::Equals;
  key_map_[sf::Keyboard::Dash]      = CEGUI::Key::Minus;
  key_map_[sf::Keyboard::Space]     = CEGUI::Key::Space;
  key_map_[sf::Keyboard::Return]    = CEGUI::Key::Return;
  key_map_[sf::Keyboard::BackSpace] = CEGUI::Key::Backspace;
  key_map_[sf::Keyboard::Tab]       = CEGUI::Key::Tab;
  key_map_[sf::Keyboard::PageUp]    = CEGUI::Key::PageUp;
  key_map_[sf::Keyboard::PageDown]  = CEGUI::Key::PageDown;
  key_map_[sf::Keyboard::End]       = CEGUI::Key::End;
  key_map_[sf::Keyboard::Home]      = CEGUI::Key::Home;
  key_map_[sf::Keyboard::Insert]    = CEGUI::Key::Insert;
  key_map_[sf::Keyboard::Delete]    = CEGUI::Key::Delete;
  key_map_[sf::Keyboard::Add]       = CEGUI::Key::Add;
  key_map_[sf::Keyboard::Subtract]  = CEGUI::Key::Subtract;
  key_map_[sf::Keyboard::Multiply]  = CEGUI::Key::Multiply;
  key_map_[sf::Keyboard::Divide]    = CEGUI::Key::Divide;
  key_map_[sf::Keyboard::Left]      = CEGUI::Key::ArrowLeft;
  key_map_[sf::Keyboard::Right]     = CEGUI::Key::ArrowRight;
  key_map_[sf::Keyboard::Up]        = CEGUI::Key::ArrowUp;
  key_map_[sf::Keyboard::Down]      = CEGUI::Key::ArrowDown;
  key_map_[sf::Keyboard::Numpad0]   = CEGUI::Key::Numpad0;
  key_map_[sf::Keyboard::Numpad1]   = CEGUI::Key::Numpad1;
  key_map_[sf::Keyboard::Numpad2]   = CEGUI::Key::Numpad2;
  key_map_[sf::Keyboard::Numpad3]   = CEGUI::Key::Numpad3;
  key_map_[sf::Keyboard::Numpad4]   = CEGUI::Key::Numpad4;
  key_map_[sf::Keyboard::Numpad5]   = CEGUI::Key::Numpad5;
  key_map_[sf::Keyboard::Numpad6]   = CEGUI::Key::Numpad6;
  key_map_[sf::Keyboard::Numpad7]   = CEGUI::Key::Numpad7;
  key_map_[sf::Keyboard::Numpad8]   = CEGUI::Key::Numpad8;
  key_map_[sf::Keyboard::Numpad9]   = CEGUI::Key::Numpad9;
  key_map_[sf::Keyboard::F1]        = CEGUI::Key::F1;
  key_map_[sf::Keyboard::F2]        = CEGUI::Key::F2;
  key_map_[sf::Keyboard::F3]        = CEGUI::Key::F3;
  key_map_[sf::Keyboard::F4]        = CEGUI::Key::F4;
  key_map_[sf::Keyboard::F5]        = CEGUI::Key::F5;
  key_map_[sf::Keyboard::F6]        = CEGUI::Key::F6;
  key_map_[sf::Keyboard::F7]        = CEGUI::Key::F7;
  key_map_[sf::Keyboard::F8]        = CEGUI::Key::F8;
  key_map_[sf::Keyboard::F9]        = CEGUI::Key::F9;
  key_map_[sf::Keyboard::F10]       = CEGUI::Key::F10;
  key_map_[sf::Keyboard::F11]       = CEGUI::Key::F11;
  key_map_[sf::Keyboard::F12]       = CEGUI::Key::F12;
  key_map_[sf::Keyboard::F13]       = CEGUI::Key::F13;
  key_map_[sf::Keyboard::F14]       = CEGUI::Key::F14;
  key_map_[sf::Keyboard::F15]       = CEGUI::Key::F15;
  key_map_[sf::Keyboard::Pause]     = CEGUI::Key::Pause;
  
  mouse_map_[sf::Mouse::Left]       = CEGUI::LeftButton;
  mouse_map_[sf::Mouse::Right]      = CEGUI::RightButton;
  mouse_map_[sf::Mouse::Middle]     = CEGUI::MiddleButton;
  mouse_map_[sf::Mouse::XButton1]   = CEGUI::X1Button;
  mouse_map_[sf::Mouse::XButton2]   = CEGUI::X2Button;
}

CEGUI::Key::Scan GameDesktop::toCEGUIKey(sf::Keyboard::Key code)
{
  return key_map_[code];
}

CEGUI::MouseButton GameDesktop::toCEGUIMouseButton(sf::Mouse::Button button)
{
  return mouse_map_[button];
}


bool GameDesktop::isMouseInRenderWindow() const
{
  sf::Vector2i mouse_pos = sf::Mouse::getPosition();
  sf::IntRect window_rect(screen_.getPosition(), static_cast<sf::Vector2i>(screen_.getSize()));

  return window_rect.contains(mouse_pos);
}
