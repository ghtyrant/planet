#ifndef SPACEGENERATOR_H
#define SPACEGENERATOR_H

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>
#include <memory>
#include <cmath>

#include "celestialobject.h"
#include "consts.h"
#include "easylogging++.h"

class ShootingStar : public sf::Sprite
{
public:
  ShootingStar(unsigned int x, unsigned int y, unsigned int tx, unsigned int ty, sf::Time max_age, const sf::Color &color, const sf::Texture &texture)
    : sf::Sprite(), tx_(tx), ty_(ty), alpha_dir_(1), alpha_(0), max_age_(max_age)
  {
    setPosition(x, y);
    setTexture(texture);
    setColor(color);
    setScale(0.1f, 0.1f);

    sf::Vector2f d;
    d.x = x - tx;
    d.y = y - ty;

    float angle = atan2(d.y, d.x);
    setRotation((angle > 0 ? angle : (2*PI + angle)) * 360 / (2*PI));
    step_x_ = cos(angle) * 10.0f;
    step_y_ = sin(angle) * 10.0f;

    alpha_step_ = 255.0f / (max_age_.asMilliseconds() / 2.0f);
  }

  bool update(sf::Time delta)
  {
    age_ += delta;

    if (age_ >= max_age_)
      return false;

    sf::Color color = getColor();

    alpha_ += (alpha_step_ * delta.asMilliseconds()) * alpha_dir_;

    if (alpha_ >= 255)
    {
      alpha_dir_ = -1;
      alpha_ = 255;
    }

    color.a = static_cast<unsigned char>(alpha_);

    setColor(color);

    move(step_x_, step_y_);

    return true;
  }

  unsigned int tx_;
  unsigned int ty_;
  float step_x_;
  float step_y_;
  char alpha_dir_;
  float alpha_step_;
  float alpha_;
  sf::Time max_age_;
  sf::Time age_;
};

class Space
{
  public:
    Space();

    void createBackground();
    sf::Texture createPlanetTexture();
    void generate();
    void simulate();
    void draw(sf::RenderTarget &target);
    std::shared_ptr<CelestialObject> coordsOverObject(const sf::Vector2f pos);
    void update(sf::Time delta);
  protected:
  private:
    void spawnShootingStar();

    std::vector<std::shared_ptr<CelestialObject>> objects_;
    std::vector<std::shared_ptr<sf::Texture>> planet_textures_;
    std::vector<std::shared_ptr<sf::Texture>> nebula_textures_;
    std::vector<std::shared_ptr<sf::Texture>> sun_textures_;
    sf::Texture background_tex_;
    sf::Sprite background_;

    std::shared_ptr<CelestialObject> sun_;

    std::vector<std::unique_ptr<ShootingStar>> shooting_stars_;

    sf::Texture shooting_star_tex_;
    sf::Time shooting_star_timer_;

    sf::Texture bla_;
};

#endif // SPACEGENERATOR_H
