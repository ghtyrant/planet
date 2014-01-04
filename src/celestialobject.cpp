#include "celestialobject.h"
#include "consts.h"
#include "easylogging++.h"

CelestialObject::CelestialObject(const std::string &name, const sf::Texture &tex, unsigned int radius, unsigned int age, unsigned int mass)
  : GameObject(name),
    sf::Sprite(),
    radius_(radius),
    age_(age),
    mass_(mass)
{
  velocity_.x = 0;
  velocity_.y = 0;

  setTexture(tex);

  sf::FloatRect bounds = getGlobalBounds();
  //LOG(INFO) << "Width: " << bounds.width << " Height: " << bounds.height;
  setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);

  float size = ((float)radius_ / (float)RADIUS_MAX) * 0.5f;
  setScale(size, size);
}

CelestialObject::~CelestialObject()
{
  //dtor
}

void CelestialObject::update(sf::Time delta)
{
  rotate(0.1f);
}
