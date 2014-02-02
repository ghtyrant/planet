#include <SFML/Graphics/RenderTarget.hpp>

#include "celestialobject.h"
#include "consts.h"
#include "easylogging++.h"

CelestialObject::CelestialObject(const std::string &name, const sf::Texture &tex, unsigned int radius, unsigned int age, unsigned int mass, const std::shared_ptr<CelestialObject> parent)
  : GameObject(name),
    sf::Drawable(),
    sf::Transformable(),
    radius_(radius),
    age_(age),
    mass_(mass),
    parent_(parent)
{
  velocity_.x = 0;
  velocity_.y = 0;

  sprite_.setTexture(tex);

  sf::FloatRect bounds = sprite_.getGlobalBounds();
  //LOG(INFO) << "Width: " << bounds.width << " Height: " << bounds.height;
  origin_.x = bounds.width / 2.0f;
  origin_.y = bounds.height / 2.0f;
  setOrigin(origin_);

  float size = ((float)radius_ / (float)RADIUS_MAX) * 0.5f;
  setScale(size, size);

  if (parent_)
    last_parent_pos_ = parent_->getPosition();
}

CelestialObject::~CelestialObject()
{
  //dtor
}

void CelestialObject::update(sf::Time delta)
{
  if (parent_)
  {
    sf::Transform t;
    sf::Vector2f parent_pos = parent_->getPosition();
    sf::Vector2f translation = parent_pos - last_parent_pos_;
    sf::Vector2f pos = getPosition();
    sf::Vector2f result_pos = sf::Vector2f(parent_pos.x - pos.x, parent_pos.y - pos.y);
    t.translate(translation);
    float parent_distance = sqrt(result_pos.x*result_pos.x + result_pos.y*result_pos.y);
    t.rotate(1.0f /*(parent_distance/5000.0f)*/, parent_->getPosition());
    sf::Vector2f new_pos = t.transformPoint(getPosition());
    sf::Transformable::setPosition(new_pos);
    last_parent_pos_ = parent_pos;
  }

  //setOrigin(origin_);
  rotate(0.1f);
}

void CelestialObject::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
  states.transform *= getTransform();
  target.draw(sprite_, states);
}

bool CelestialObject::isPointIn(const sf::Vector2f pos) const
{
  return sprite_.getGlobalBounds().contains(pos);
}

void CelestialObject::setColor(const sf::Color color)
{
  sprite_.setColor(color);
}

bool CelestialObject::hasParent() const
{
  return (parent_ != nullptr);
}

std::shared_ptr<CelestialObject> CelestialObject::getParent() const
{
  return parent_;
}
