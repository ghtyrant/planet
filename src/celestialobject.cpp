#include <SFML/Graphics/RenderTarget.hpp>

#include "celestialobject.h"
#include "consts.h"
#include "easylogging++.h"

CelestialObject::CelestialObject(const std::string &name, const sf::Texture &tex, const sf::Texture &specular_tex, unsigned int radius, unsigned int age, unsigned int mass, float rotate_speed)
  : GameObject(name),
    sf::Drawable(),
    sf::Transformable(),
    std::enable_shared_from_this<CelestialObject>(),
    radius_(radius),
    age_(age),
    mass_(mass),
    rotate_speed_(rotate_speed)
{
  velocity_.x = 0;
  velocity_.y = 0;

  sprite_.setTexture(tex);
  specular_sprite_.setTexture(specular_tex);

  sf::FloatRect bounds = sprite_.getGlobalBounds();
  //LOG(INFO) << "Width: " << bounds.width << " Height: " << bounds.height;
  origin_.x = bounds.width / 2.0f;
  origin_.y = bounds.height / 2.0f;
  setOrigin(origin_);

  float size = ((float)radius_ / (float)RADIUS_MAX) * 1.0f;
  setScale(size, size);
}

CelestialObject::~CelestialObject()
{
  //dtor
}

void CelestialObject::update(sf::Time delta)
{
  rotate(rotate_speed_);

  for (auto &o : children_)
    o->update(delta); 
}

void CelestialObject::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
  states.transform *= getTransform();

  target.draw(sprite_, states);
  sf::RenderStates s(states);
  s.blendMode = sf::BlendAlpha;
  target.draw(specular_sprite_, s);

  for (auto &o : children_)
    target.draw(*o, states);
}

bool CelestialObject::isPointIn(const sf::Vector2f pos) const
{
  return sprite_.getGlobalBounds().contains(pos);
}

void CelestialObject::setColor(const sf::Color color)
{
  sprite_.setColor(color);
}

void CelestialObject::addChild(std::shared_ptr<CelestialObject> child)
{
  children_.push_back(child);
}
