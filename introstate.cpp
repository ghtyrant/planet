#include "introstate.h"
#include "gameengine.h"
#include "easylogging++.h"
#include "playstate.h"

IntroState::IntroState(GameEngine &game, std::string name)
  : GameState(game, name), angle_(0), scale_(0)
{
  if (!font_.loadFromFile("/usr/share/fonts/TTF/Vera.ttf"))
    LOG(FATAL) << "Could not load font!";

  text_.setFont(font_);
  text_.setString("PLANET INTRO");
  text_.setColor(sf::Color::Blue);
  text_.setCharacterSize(24);
  text_.setStyle(sf::Text::Style::Bold);

  sf::FloatRect text_rect = text_.getLocalBounds();
  sf::Vector2f screen_size = static_cast<sf::Vector2f>(game.screen.getSize());

  text_.setOrigin(text_rect.left + text_rect.width/2.0f,
                 text_rect.top  + text_rect.height/2.0f);
  text_.setPosition(sf::Vector2f(screen_size.x/2.0f,screen_size.y/2.0f));
}

void IntroState::draw()
{
  game_.screen.clear();
  game_.screen.draw(text_);
  game_.screen.display();
}

void IntroState::update(sf::Time delta)
{
  if (time_ >= sf::seconds(1))
    next_ = game_.build<PlayState>("PlayState");

  if (delta.asSeconds() > 0.00005f)
  {
    angle_ += 0.5f * (1.0f / delta.asSeconds());
    text_.setRotation(angle_);
    scale_ += 0.001f * (1.0f / delta.asSeconds());
    text_.setScale(scale_, scale_);

    if (angle_ >= 360.0f)
      angle_ = angle_ - 360.0f;
  }

  time_ += delta;
}
