#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <stack>
#include <string>
#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>

class GameState;

class GameEngine
{
  public:
    GameEngine(const std::string& title, const unsigned int width = 800, const unsigned int height = 600);

    void run(std::unique_ptr<GameState> state);

    void update(sf::Time delta);
    void draw();

    void quit();
    bool running() const;

    template <typename T>
    std::unique_ptr<T> build(std::string name);

    sf::RenderWindow screen;

  protected:
  private:
    std::stack<std::unique_ptr<GameState>> states_;
    sf::View view_;

    bool running_;

};

template <typename T>
std::unique_ptr<T> GameEngine::build(std::string name)
{
  return std::move(std::unique_ptr<T>(new T(*this, std::move(name))));
}



#endif // GAMEENGINE_H
