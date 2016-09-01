#ifndef __DRAWABLE_H__
#define __DRAWABLE_H__


// System
#include <memory>
#include <iostream>

// SDL
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL2_gfxPrimitives.h>


class Drawable {
public:
  typedef std::shared_ptr<Drawable> Ptr;
  
private:
protected:
  static std::string s_strFontFile;
  static TTF_Font* s_ttfFont;
  
public:
  Drawable();
  ~Drawable();
  
  template<class ... Args>
    static Drawable::Ptr create(Args ... args) {
    return std::make_shared<Drawable>(std::forward<Args>(args)...);
  }
  
  virtual void draw(SDL_Renderer* rdrRenderer) = 0;
  
  void drawText(SDL_Renderer* rdrRenderer, std::string strText, int nX, int nY, SDL_Color colColor, bool bCenter = false, bool bClip = false, SDL_Rect rctClip = {0, 0, 0, 0});
      
  SDL_Rect drawBox(SDL_Renderer* rdrRenderer, int nX, int nY, unsigned int unWidth, unsigned int unHeight, SDL_Color colColor, bool bCenter = false);
  SDL_Rect drawBox(SDL_Renderer* rdrRenderer, SDL_Rect rctRect, SDL_Color colColor, bool bCenter = false);
  SDL_Rect drawTextBox(SDL_Renderer* rdrRenderer, int nX, int nY, unsigned int unWidth, unsigned int unHeight, std::string strText, SDL_Color colText, SDL_Color colBackground);
      
  void drawLine(SDL_Renderer* rdrRenderer, int nX1, int nY1, int nX2, int nY2, SDL_Color colColor);
      
  unsigned int textWidth(std::string strText);
};


#endif /* __DRAWABLE_H__ */
