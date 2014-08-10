#include "lumina/lumina.hpp"
using namespace lum;

int main() {
  Window win("CG Praktikum :-)");
  win.setVersionHint(3, 3);
  win.open();
  slog(" Opened Window :)");

  while(win.isValid()) {
    win.update();
  }
}