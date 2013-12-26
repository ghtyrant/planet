#include <random>

#include "space.h"
#include "easylogging++.h"
#include "consts.h"

Space::Space()
{
  for (int i = 0; i < 5; i++)
  {
    sf::Image img;
    std::ostringstream out;
    out << "media/planet" << (i+1) << ".png";
    img.loadFromFile(out.str());
    img.createMaskFromColor(sf::Color::Black);
    std::shared_ptr<sf::Texture> tmp(new sf::Texture);
    tmp->loadFromImage(img);
    planet_textures_.push_back(std::move(tmp));
  }
}

void Space::generate()
{
  std::random_device rd;
  std::uniform_int_distribution<> mass_dist(MASS_MIN, MASS_MAX);
  std::uniform_int_distribution<> radius_dist(RADIUS_MIN, RADIUS_MAX);
  std::uniform_int_distribution<> age_dist(AGE_MIN, AGE_MAX);

  std::uniform_int_distribution<> pos_dist(-1000, 1000);

  std::uniform_int_distribution<> planet_texture(0, 4);

  for (int i = 0; i < 200; i++)
  {
    std::shared_ptr<CelestialObject> o(new CelestialObject("Ladida1", radius_dist(rd), age_dist(rd), mass_dist(rd)));
    o->setTexture(*planet_textures_[planet_texture(rd)]);
    o->setPosition(pos_dist(rd), pos_dist(rd));
    o->setMass(mass_dist(rd));
    objects_.push_back(std::move(o));
  }
}

std::shared_ptr<CelestialObject> Space::coordsOverObject(const sf::Vector2f pos)
{
  for (auto& o : objects_)
    if (o->getGlobalBounds().contains(pos))
      return o;

  return nullptr;
}

void Space::simulate()
{
  for (auto& o1 : objects_)
  {
    // increase age
    o1->setAge(o1->getAge() + 1);

    // simulate repulsion
    for (auto& o2 : objects_)
    {
      if (o1 == o2)
        continue;

      sf::Vector2f v1 = o1->getPosition();
      sf::Vector2f v2 = o2->getPosition();

      sf::Vector2f d;
      d.x = v2.x - v1.x;
      d.y = v2.y - v1.y;

      float angle = atan2(d.y, d.x);
      float distance = sqrt(d.x*d.x + d.y*d.y);
      float force = -(o2->getMass() / (distance * distance));

      o1->addVelocity(cos(angle) * force, sin(angle) * force);
    }
    o1->move(o1->getVelocity());
    o1->setVelocity(0, 0);
  }
}

void Space::update(sf::Time delta)
{
  for (auto& o : objects_)
  {
    o->update(delta);
  }
}

void Space::draw(sf::RenderTarget &target)
{
  for (auto& o : objects_)
  {
    target.draw(*o);
  }
}
