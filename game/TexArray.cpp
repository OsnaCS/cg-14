#include "TexArray.hpp"

void TexArray::create(Vec2i dimension, TexFormat format, int layerCount) {
  // check if another texture is currently primed
  if(TextureUnits::isPrimed(getMaxTexUnits() - 1)) {
    logAndThrow<LogicEx>("[Texture] Cannot execute 'create': Last texture "
                         "unit has to be unused!");
  }

  // check arguments
  if(dimension.x <= 0 || dimension.y <= 0) {
    logError("[Texture] Invalid argument: dimension<",
             dimension, "> is negative!");
    throw InvalidArgEx("[Texture] Invalid argument: dimension is negative");
  }
  // show warnings when strange resolutions are used
  if((dimension.x % 2) != 0 || (dimension.y % 2) != 0) {
    logWarning("[Texture] dimension<", dimension, "> is not MOD 2!");
  }

  // save properties
  m_format = format;
  m_dimension = dimension;
  m_layerCount = layerCount;

  // create texture and storage for it
  glGenTextures(1, &m_handle);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glActiveTexture(getMaxTexUnits() - 1);
  glBindTexture(GL_TEXTURE_2D_ARRAY, m_handle);

  glTexStorage3D(GL_TEXTURE_2D_ARRAY, 9, texFormatToGLInternalFormat(m_format), 
                 dimension.x, dimension.y, layerCount);
  glGetError();


  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


  checkGLError("[Texture] Error<", GLERR, "> while creating texture-ar!");
  // glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

void TexArray::addData(int index, void* data) {
  glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, index, m_dimension.x, 
                  m_dimension.y, 1, texFormatToGLFormat(m_format), 
                  texFormatToGLType(m_format), data);

  checkGLError("addData<", GLERR, ">");
}

void TexArray::generateMipMaps() {
  glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
  checkGLError("MipMap<", GLERR, ">");
}


void TexArray::prePrime(int texUnit) {
  glActiveTexture(GL_TEXTURE0 + texUnit);
  glBindTexture(GL_TEXTURE_2D_ARRAY, m_handle);
  checkGLError("prePrime<", GLERR, ">");
  TextureUnits::setPrimed(texUnit);
}

void TexArray::postPrime(int texUnit) {
  glActiveTexture(GL_TEXTURE0 + texUnit);
  glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
  checkGLError("postPrime<", GLERR, ">");
  TextureUnits::unsetPrimed(texUnit);
}
