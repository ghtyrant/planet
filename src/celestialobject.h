#ifndef PLANET_H
#define PLANET_H

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include <list>

#include "gameobject.h"

class Atmosphere
{
public:
  Atmosphere() : health_(1) {}
  float health_;
};

class CelestialObject : public GameObject, public sf::Drawable, public sf::Transformable, public std::enable_shared_from_this<CelestialObject>
{
  public:
    enum Type
    {
      STAR,
      PLANET,
      GASPLANET,
    };

    CelestialObject(const std::string &name, const sf::Texture &tex, const sf::Texture &specular_tex, unsigned int radius, unsigned int age, unsigned int mass, float rotate_speed);
    virtual ~CelestialObject();

    void setType(CelestialObject::Type type) {type_ = type;}
    CelestialObject::Type getType() {return type_;}

    void setMass(int mass) {mass_ = mass;}
    int getMass() {return mass_;}

    void setVelocity(float x, float y) {velocity_.x = x; velocity_.y = y;}
    void addVelocity(float x, float y) {velocity_.x += x; velocity_.y += y;}
    const sf::Vector2f &getVelocity() {return velocity_;}

    unsigned int getAge() {return age_;}
    void setAge(unsigned int age) {age_ = age;}

    void update(sf::Time delta);
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

    bool isPointIn(const sf::Vector2f pos) const;

    void setColor(const sf::Color color);

    void addChild(std::shared_ptr<CelestialObject> child);

  protected:
  private:
    sf::Sprite sprite_;
    sf::Sprite specular_sprite_;
    sf::Vector2f velocity_;
    unsigned int radius_;
    unsigned int age_;
    unsigned int mass_;
    CelestialObject::Type type_;
    Atmosphere atmosphere_;
    std::list<std::shared_ptr<CelestialObject>> children_;
    float rotate_speed_;
    sf::CircleShape path_;

    sf::Vector2f origin_;
};

#endif // PLANET_H
