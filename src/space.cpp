#include <random>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "space.h"
#include "easylogging++.h"
#include "consts.h"
#include "perlinnoise.h"

Space::Space()
{
  for (int i = 0; i < 5; i++)
  {
    sf::Image img;
    std::ostringstream out;
    out << "media/planet" << (i+1) << ".png";
    img.loadFromFile(out.str());
    img.createMaskFromColor(sf::Color::Black);
    auto tmp = std::make_shared<sf::Texture>();
    tmp->loadFromImage(img);
    planet_textures_.push_back(std::move(tmp));
  }

  for (int i = 0; i < 4; i++)
  {
    sf::Image img;
    std::ostringstream out;
    out << "media/nebula" << (i+1) << ".png";
    img.loadFromFile(out.str());
    //img.createMaskFromColor(sf::Color::Black);
    auto tmp = std::make_shared<sf::Texture>();
    tmp->loadFromImage(img);
    nebula_textures_.push_back(std::move(tmp));
  }

  shooting_star_tex_.loadFromFile("media/shootingstar.png");

  createBackground();
}

void Space::createBackground()
{
  sf::Image tmp;
  tmp.create(STARFIELD_SIZE, STARFIELD_SIZE, sf::Color::Black);

  std::random_device rd;
  std::uniform_int_distribution<> pos_dist(0, STARFIELD_SIZE-1);
  std::uniform_int_distribution<> star_type(0, 100);
  std::uniform_int_distribution<> alpha_dist(0, 255);

  for (unsigned int i = 0; i < STAR_COUNT; i++)
  {
    sf::Color c(0, 0, 0);
    int type = star_type(rd);
    if (type < 70)
    {
      c.r = 255;
      c.g = 255;
      c.b = 255;
    }
    else if(type < 80)
    {
      c.r = 180;
      c.g = 180;
      c.b = 255;
    }
    else if(type < 90)
    {
      c.r = 255;
      c.g = 180;
      c.b = 180;
    }
    else if(type < 100)
    {
      c.r = 255;
      c.g = 255;
      c.b = 180;
    }

    c.a = alpha_dist(rd);

    tmp.setPixel(pos_dist(rd), pos_dist(rd), c);
  }

  // Render the starfield to a temporary texture
  sf::Texture tmp_tex;
  tmp_tex.loadFromImage(tmp);

  // Create a sprite and assign the starfield texture
  sf::Sprite tmp_sprite;
  tmp_sprite.setTexture(tmp_tex);
  float new_scale = (float)SPACE_SIZE / (float)STARFIELD_SIZE;
  tmp_sprite.setScale(new_scale, new_scale);

  // Create a RenderTexture and draw our starfield to it
  sf::RenderTexture render_tex;
  render_tex.create(STARFIELD_SIZE, STARFIELD_SIZE);
  render_tex.draw(tmp_sprite);

  std::uniform_int_distribution<> nebula_pos_dist(100, STARFIELD_SIZE-100);
  std::uniform_real_distribution<float> nebula_size_dist(0.1f, 5.0f);
  std::uniform_int_distribution<> nebula_rotation_dist(0, 360);
  std::uniform_int_distribution<> nebula_type_dist(0, 3);
  for (unsigned int i = 0; i < NEBULA_COUNT; i++)
  {
    sf::Sprite tmp;
    tmp.setTexture(*nebula_textures_[nebula_type_dist(rd)]);
    tmp.setPosition(pos_dist(rd), pos_dist(rd));
    tmp.setColor(sf::Color(alpha_dist(rd), alpha_dist(rd), alpha_dist(rd), alpha_dist(rd) / 3));
    tmp.setRotation((float) nebula_rotation_dist(rd));

    float scale = nebula_size_dist(rd);
    tmp.setScale(scale, scale);
    render_tex.draw(tmp);
  }

  background_tex_ = render_tex.getTexture();
  background_tex_.setRepeated(true);
  background_tex_.setSmooth(true);
  background_.setTexture(background_tex_);
}

sf::Texture Space::createPlanetTexture()
{
  PerlinNoise pn(0.2, 0.2, 1, 10, 23);

  sf::Image img;
  img.create(100, 100);

  for (double x = 0; x < 100; x++)
  {
    for (double y = 0; y < 100; y++)
    {
      double height = (pn.get(x, y) + 0.5f) * 255.0f;
      if (height == 0)
        height = 1;
      sf::Color color(height, height, height, 255);
      img.setPixel(x, y, color);
    }
  }

  sf::Texture tmp_tex;
  tmp_tex.loadFromImage(img);

  sf::RenderTexture tex;
  tex.create(100, 100);

  sf::CircleShape c;
  c.setRadius(50);
  c.setTexture(&tmp_tex);

  tex.draw(c);

  return tex.getTexture();
}

void Space::generate()
{
  std::random_device rd;
  std::uniform_int_distribution<> mass_dist(MASS_MIN, MASS_MAX);
  std::uniform_int_distribution<> radius_dist(RADIUS_MIN, RADIUS_MAX);
  std::uniform_int_distribution<> age_dist(AGE_MIN, AGE_MAX);

  std::uniform_int_distribution<> pos_dist(0, SPACE_SIZE);

  std::uniform_int_distribution<> planet_texture(0, 4);

  bla_ = createPlanetTexture();

  for (int i = 0; i < 10; i++)
  {
    auto o = std::make_shared<CelestialObject>("Ladida1", *planet_textures_[planet_texture(rd)], radius_dist(rd), age_dist(rd), mass_dist(rd));
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

  for (auto it = shooting_stars_.begin(); it != shooting_stars_.end();)
  {
    if (!(*it)->update(delta))
    {
      LOG(INFO) << "Deleting a shooting star!";
      it = shooting_stars_.erase(it);
    }
    else
      ++it;

  }

  shooting_star_timer_ += delta;

  if (shooting_star_timer_.asMilliseconds() >= SHOOTING_STAR_SPAWN_TIME)
  {
    shooting_star_timer_ = sf::seconds(0);

    std::random_device rd;
    std::uniform_int_distribution<> spawn_shooting_star_change(0, 100);

    if (spawn_shooting_star_change(rd) > 50)
      spawnShootingStar();
  }
}

void Space::draw(sf::RenderTarget &target)
{
  target.draw(background_);

  for (auto& o : objects_)
  {
    target.draw(*o);
  }

  for (auto& o : shooting_stars_)
    target.draw(*o);
}

void Space::spawnShootingStar()
{

  std::random_device rd;
  std::uniform_int_distribution<> pos_dist(0, STARFIELD_SIZE);
  std::uniform_int_distribution<> age_dist(SHOOTING_STAR_AGE_MIN, SHOOTING_STAR_AGE_MAX);
  std::uniform_int_distribution<> color_dist(0, 255);

  std::unique_ptr<ShootingStar> ss(new ShootingStar(pos_dist(rd),
                                                    pos_dist(rd),
                                                    pos_dist(rd),
                                                    pos_dist(rd),
                                                    sf::milliseconds(age_dist(rd)),
                                                    sf::Color(255, 255, 255, 0),
                                                    shooting_star_tex_));

  sf::Vector2f bla = ss->getPosition();

  shooting_stars_.push_back(std::move(ss));
}
