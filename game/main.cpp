#include "CraftGame.hpp"
#include "lumina/lumina.hpp"

#include <iostream>
#include <string>

using namespace lumina;

int main(int argc, char **argv) {
  Vec2i size(800, 600);
  bool fullscreen = false;

  slog("CraftGame v1.0 started...");
  Logger::setGlobalStdLevelFilter(LogLevel::Info);

  for(int i = 0; i < argc; ++i) {
    string str(argv[i]);
    
    if(str.size() < 2 || str[0] != '-') {
      continue;
    }

    switch(str[1]) {
      case 'f':
        fullscreen = true;
        break;
      case 'x':
        try {
          size.x = stoi(str.substr(2));
        } 
        catch(...) {
          std::cout << "Cannot parse x!" << std::endl;
        }
        break;
      case 'y':
        try {
          size.y = stoi(str.substr(2));
        } 
        catch(...) {
          std::cout << "Cannot parse y!" << std::endl;
        }
        break;
      default:
        std::cout << "Invalid option " << str << std::endl;

    }
  }

  CraftGame game;
  game.init(size, fullscreen);
  game.start();
}
