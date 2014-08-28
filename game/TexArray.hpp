#pragma once

#include <GL/glew.h>
#include "lumina/lumina.hpp"

using namespace lumina;

class TexArray : public TextureInterface {
public:
  TexArray() : m_handle(0) {
  }
  ~TexArray() {}

  void create(Vec2i dimension, TexFormat format, int layerCount);
  void addData(int index, void* data);
  void generateMipMaps();





private:
  GLuint m_handle;
  Vec2i m_dimension;
  int m_layerCount;
  TexParam m_activeParams;
  TexFormat m_format;



  void prePrime(int texUnit) final;
  void postPrime(int texUnit) final;
};