#include <visualizer_semrec/Drawable.h>


std::string Drawable::s_strFontFile = "";
TTF_Font* Drawable::s_ttfFont = nullptr;


Drawable::Drawable() : m_dTimeLast(this->getSystemTimeStampPrecise()) {
}

Drawable::~Drawable() {
}

void Drawable::drawLine(SDL_Renderer* rdrRenderer, int nX1, int nY1, int nX2, int nY2, SDL_Color colColor) {
  lineRGBA(rdrRenderer, nX1, nY1, nX2, nY2, colColor.r, colColor.g, colColor.b, colColor.a);
}
    
SDL_Rect Drawable::drawTextBox(SDL_Renderer* rdrRenderer, int nX, int nY, unsigned int unWidth, unsigned int unHeight, std::string strText, SDL_Color colText, SDL_Color colBackground) {
  SDL_Rect rctBox = this->drawBox(rdrRenderer, nX, nY, unWidth, unHeight, colBackground, true);
  this->drawText(rdrRenderer, strText, nX, nY, colText, true, true, rctBox);
      
  return rctBox;
}
    
void Drawable::drawText(SDL_Renderer* rdrRenderer, std::string strText, int nX, int nY, SDL_Color colColor, bool bCenter, bool bClip, SDL_Rect rctClip) {
  SDL_Surface* sfText = TTF_RenderText_Blended(s_ttfFont, strText.c_str(), colColor);
  SDL_Texture* txText = SDL_CreateTextureFromSurface(rdrRenderer, sfText);
      
  SDL_Rect rctSource = {0, 0, sfText->w, sfText->h};
  SDL_Rect rctDestination = {nX, nY, sfText->w, sfText->h};
      
  if(bCenter) {
    rctDestination.x -= rctDestination.w / 2;
    rctDestination.y -= rctDestination.h / 2;
  }
      
  if(bClip) {
    if(rctDestination.x < rctClip.x) {
      rctSource.x += rctClip.x - rctDestination.x;
      rctDestination.x = rctClip.x;
    }
	
    if(rctDestination.x + rctDestination.w > rctClip.x + rctClip.w) {
      rctSource.w -= (rctDestination.x + rctDestination.w) - (rctClip.x + rctClip.w);
      rctDestination.w = rctClip.w;
    }
	
    if(rctDestination.y < rctClip.y) {
      rctSource.y += rctClip.y - rctDestination.y;
      rctDestination.y = rctClip.y;
    }
	
    if(rctDestination.y + rctDestination.h > rctClip.y + rctClip.h) {
      rctSource.h -= (rctDestination.y + rctDestination.h) - (rctClip.y + rctClip.h);
      rctDestination.h = rctClip.h;
    }
  }
      
  SDL_RenderCopy(rdrRenderer, txText, &rctSource, &rctDestination); 
      
  SDL_FreeSurface(sfText);
  SDL_DestroyTexture(txText);
}
    
SDL_Rect Drawable::drawBox(SDL_Renderer* rdrRenderer, int nX, int nY, unsigned int unWidth, unsigned int unHeight, SDL_Color colColor, bool bCenter) {
  return this->drawBox(rdrRenderer, {nX, nY, (int)unWidth, (int)unHeight}, colColor, bCenter);
}
    
SDL_Rect Drawable::drawBox(SDL_Renderer* rdrRenderer, SDL_Rect rctRect, SDL_Color colColor, bool bCenter) {
  if(bCenter) {
    rctRect.x -= rctRect.w / 2;
    rctRect.y -= rctRect.h / 2;
  }
  
  int nRadius = 5;
  
  roundedBoxRGBA(rdrRenderer, rctRect.x, rctRect.y, rctRect.x + rctRect.w, rctRect.y + rctRect.h, nRadius, colColor.r, colColor.g, colColor.b, colColor.a);
  roundedRectangleRGBA(rdrRenderer, rctRect.x, rctRect.y, rctRect.x + rctRect.w, rctRect.y + rctRect.h, nRadius, 0, 0, 0, colColor.a);
      
  return rctRect;
}
    
unsigned int Drawable::textWidth(std::string strText) {
  SDL_Surface* sfText = TTF_RenderText_Blended(s_ttfFont, strText.c_str(), {0, 0, 0, 0});
  unsigned int unTextWidth = sfText->w;
      
  SDL_FreeSurface(sfText);
      
  return unTextWidth;
}

double Drawable::getSystemTimeStampPrecise() {
  auto now = std::chrono::high_resolution_clock::now();
  auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
  
  return (double)(millis / 1000.0);
}

double Drawable::timeElapsed() {
  double dTimeNow = this->getSystemTimeStampPrecise();
  double dElapsed = dTimeNow - m_dTimeLast;
  m_dTimeLast = dTimeNow;
  
  return dElapsed;
}
