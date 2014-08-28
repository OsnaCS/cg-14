#include "CraftGame.hpp"
#include <iostream>
#include <string>

int main(int argc, char **argv) {
  Vec2i size(800, 600);
  bool fullscreen = false;

  for(int i = 0; i < argc; ++i) {
    string str(argv[i]);
    std::cout << str << std::endl;
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

  std::cout << size << std::endl;

  // return 0;


  CraftGame game;
  game.init(size, fullscreen);
  game.start();
}
