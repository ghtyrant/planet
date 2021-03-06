#include <iostream>
#include "gameengine.h"
#include "introstate.h"
#include "playstate.h"
#include "easylogging++.h"
#include "gameobject.h"

_INITIALIZE_EASYLOGGINGPP

int main()
{
  LOG(INFO) << "Starting up game ...";

  GameEngine game("Hehe", 1024, 786);
  //game.run(game.build<IntroState>("IntroState"));
  game.run(game.build<PlayState>("PlayState"));

  sf::Clock clock; 
  while (game.running())
  {
    game.update(clock.restart());
    game.draw();
  }

  return 0;
}
