#include "celestialobject.h"
#include "consts.h"

CelestialObject::CelestialObject(const std::string &name, unsigned int radius, unsigned int age, unsigned int mass)
  : GameObject(name),
    sf::Sprite(),
    radius_(radius),
    age_(age),
    mass_(mass)
{
  velocity_.x = 0;
  velocity_.y = 0;

  float size = ((float)radius_ / (float)RADIUS_MAX) * 0.2f;
  setScale(size, size);

  sf::FloatRect bounds = getGlobalBounds();
  setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
}

CelestialObject::~CelestialObject()
{
  //dtor
}

void CelestialObject::update(sf::Time delta)
{
  rotate(0.1f);
}
