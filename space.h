#ifndef SPACEGENERATOR_H
#define SPACEGENERATOR_H

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>
#include <memory>

#include "celestialobject.h"

class Space
{
  public:
    Space();

    void generate();
    void simulate();
    void draw(sf::RenderTarget &target);
    std::shared_ptr<CelestialObject> coordsOverObject(const sf::Vector2f pos);
    void update(sf::Time delta);
  protected:
  private:
    std::vector<std::shared_ptr<CelestialObject>> objects_;
    std::vector<std::shared_ptr<sf::Texture>> planet_textures_;
    sf::Texture background_tex_;
    sf::Sprite background_;
};

#endif // SPACEGENERATOR_H
