/*********************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2016, Institute for Artificial Intelligence,
 *  Universität Bremen.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************/

/** \author Jan Winkler */


#include <visualizer_semrec/PluginVisualizer.h>


namespace semrec {
  namespace plugins {
    PLUGIN_CLASS::PLUGIN_CLASS() : m_ttfFont(nullptr), m_expOwl(nullptr), m_tnTree(nullptr), m_tnActive(nullptr) {
      this->addDependency("symboliclog");
      this->setPluginVersion("0.1");
    }
    
    PLUGIN_CLASS::~PLUGIN_CLASS() {
      if(m_expOwl) {
	delete m_expOwl;
      }
    }
    
    TreeNode::Ptr PLUGIN_CLASS::addTreeNode(std::string strIdentifier) {
      TreeNode::Ptr tnAdd = TreeNode::create(strIdentifier, this->textWidth(strIdentifier) + 10);
      this->addTreeNode(tnAdd);
      
      return tnAdd;
    }
    
    void PLUGIN_CLASS::addTreeNode(TreeNode::Ptr tnAdd) {
      if(!m_tnTree) {
	m_tnTree = tnAdd;
      } else {
	m_tnActive->addChild(tnAdd);
      }
      
      m_tnActive = tnAdd;
    }
    
    void PLUGIN_CLASS::leaveTreeNode() {
      if(m_tnActive) {
	m_tnActive = m_tnActive->parent();
      }
    }
    
    Result PLUGIN_CLASS::init(int argc, char** argv) {
      Result resInit = defaultResult();
      
      Designator* cdConfig = this->getIndividualConfig();
      
      this->setSubscribedToEvent("symbolic-begin-context", true);
      this->setSubscribedToEvent("symbolic-end-context", true);
      this->setSubscribedToEvent("start-new-experiment", true);
      
      m_expOwl = new CExporterOwl();
      std::string strSemanticsDescriptorFile = cdConfig->stringValue("semantics-descriptor-file");
      m_strFontFile = cdConfig->stringValue("font-file");
      
      if(strSemanticsDescriptorFile != "") {
	if(m_expOwl->loadSemanticsDescriptorFile(strSemanticsDescriptorFile) == false) {
	  this->warn("Failed to load semantics descriptor file '" + strSemanticsDescriptorFile + "'.");
	}
      } else {
	this->warn("No semantics descriptor file was specified.");
      }
      
      if(SDL_Init(SDL_INIT_EVERYTHING) >= 0) {
	if(TTF_Init() >= 0) {
	  m_ttfFont = TTF_OpenFont(m_strFontFile.c_str(), 16);
	  std::cout << TTF_GetError() << std::endl;
	  m_unWidth =  800;
	  m_unHeight = 600;
	  m_strTitle = "SemRec Visualizer";
	  
	  m_swnWindow = SDL_CreateWindow(m_strTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_unWidth, m_unHeight, SDL_WINDOW_SHOWN);
	  m_rdrRenderer = SDL_CreateRenderer(m_swnWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	  
	  m_gxContext = SDL_GL_GetCurrentContext();
	  
	  this->addTreeNode("knowrob:RootElement");
	}
      }
      
      return resInit;
    }
    
    Result PLUGIN_CLASS::deinit() {
      return defaultResult();
    }
    
    Result PLUGIN_CLASS::cycle() {
      Result resCycle = defaultResult();
      this->deployCycleData(resCycle);
      
      this->dispatchEvents();
      this->draw();
      
      return resCycle;
    }
    
    void PLUGIN_CLASS::consumeEvent(Event evEvent) {
      if(evEvent.strEventName == "symbolic-begin-context") {
	if(evEvent.lstNodes.size() > 0) {
	  Node* ndNode = evEvent.lstNodes.front();
	  std::string strOWLClass = m_expOwl->owlClassForNode(ndNode, false, true);
	  
	  this->addTreeNode(strOWLClass);
	}
      } else if(evEvent.strEventName == "symbolic-end-context") {
	if(evEvent.lstNodes.size() > 0) {
	  Node* ndNode = evEvent.lstNodes.front();
	  std::string strOWLClass = m_expOwl->owlClassForNode(ndNode, false, true);
	  
	  this->leaveTreeNode();
	}
      } else if(evEvent.strEventName == "start-new-experiment") {
	m_tnTree = nullptr;
	this->addTreeNode("knowrob:RootElement");
      }
    }
    
    void PLUGIN_CLASS::dispatchEvents() {
      SDL_Event evEvent;
      
      if(SDL_PollEvent(&evEvent)) {
      }
    }
    
    void PLUGIN_CLASS::draw() {
      SDL_GL_MakeCurrent(m_swnWindow, m_gxContext);
      
      SDL_SetRenderDrawColor(m_rdrRenderer, 0, 0, 0, 255);
      SDL_RenderClear(m_rdrRenderer);
      
      this->draw(m_rdrRenderer);
      
      SDL_RenderPresent(m_rdrRenderer);
    }
    
    void PLUGIN_CLASS::draw(SDL_Renderer* rdrRenderer) {
      if(m_tnTree) {
	this->drawTreeNode(rdrRenderer, m_tnTree, m_unWidth / 2, m_unHeight / 2);
      }
    }
    
    void PLUGIN_CLASS::drawTreeNode(SDL_Renderer* rdrRenderer, TreeNode::Ptr tnDraw, int nX, int nY) {
      const int nLevelDistance = 50;
      const int nSiblingDistance = 20;
      
      this->drawTextBox(rdrRenderer, nX, nY, tnDraw->width(), tnDraw->height(), tnDraw->identifier(), tnDraw->textColor(), tnDraw->backgroundColor());
      
      std::vector<TreeNode::Ptr> vecChildren = tnDraw->children();
      unsigned int unRowWidth = 0;
      
      for(TreeNode::Ptr tnChild : vecChildren) {
	if(unRowWidth != 0) {
	  unRowWidth += nSiblingDistance;
	}
	
	unRowWidth + tnChild->width();
      }
      
      unsigned int unOffset = nX - (unRowWidth / 2);
      
      for(TreeNode::Ptr tnChild : vecChildren) {
	int nXChild = unOffset;
	
	this->drawTreeNode(rdrRenderer, tnChild, unOffset, nY + tnDraw->height() / 2 + tnChild->height() / 2 + nLevelDistance);
	unOffset += tnChild->width() + nSiblingDistance;
	
	//this->drawLine(rdrRenderer, nX, nY + unNodeHeight / 2, nXChild, nYChild - unNodeHeight / 2, {0, 128, 0, 255});
      }
    }
    
    int PLUGIN_CLASS::branchWidth(TreeNode::Ptr tnRoot) {
      return 0;
    }
    
    void PLUGIN_CLASS::drawLine(SDL_Renderer* rdrRenderer, int nX1, int nY1, int nX2, int nY2, SDL_Color colColor) {
      lineRGBA(rdrRenderer, nX1, nY1, nX2, nY2, colColor.r, colColor.g, colColor.b, colColor.a);
    }
    
    SDL_Rect PLUGIN_CLASS::drawTextBox(SDL_Renderer* rdrRenderer, int nX, int nY, unsigned int unWidth, unsigned int unHeight, std::string strText, SDL_Color colText, SDL_Color colBackground) {
      SDL_Rect rctBox = this->drawBox(rdrRenderer, nX, nY, unWidth, unHeight, colBackground, true);
      this->drawText(rdrRenderer, strText, nX, nY, colText, true, true, rctBox);
      
      return rctBox;
    }
    
    void PLUGIN_CLASS::drawText(SDL_Renderer* rdrRenderer, std::string strText, int nX, int nY, SDL_Color colColor, bool bCenter, bool bClip, SDL_Rect rctClip) {
      SDL_Surface* sfText = TTF_RenderText_Blended(m_ttfFont, strText.c_str(), colColor);
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
    
    SDL_Rect PLUGIN_CLASS::drawBox(SDL_Renderer* rdrRenderer, int nX, int nY, unsigned int unWidth, unsigned int unHeight, SDL_Color colColor, bool bCenter) {
      return this->drawBox(rdrRenderer, {nX, nY, (int)unWidth, (int)unHeight}, colColor, bCenter);
    }
    
    SDL_Rect PLUGIN_CLASS::drawBox(SDL_Renderer* rdrRenderer, SDL_Rect rctRect, SDL_Color colColor, bool bCenter) {
      if(bCenter) {
	rctRect.x -= rctRect.w / 2;
	rctRect.y -= rctRect.h / 2;
      }
      
      boxRGBA(rdrRenderer, rctRect.x, rctRect.y, rctRect.x + rctRect.w, rctRect.y + rctRect.h, colColor.r, colColor.g, colColor.b, colColor.a);
      
      return rctRect;
    }
    
    unsigned int PLUGIN_CLASS::textWidth(std::string strText) {
      SDL_Surface* sfText = TTF_RenderText_Blended(m_ttfFont, strText.c_str(), {0, 0, 0, 0});
      unsigned int unTextWidth = sfText->w;
      
      SDL_FreeSurface(sfText);
      
      return unTextWidth;
    }
  }
  
  extern "C" plugins::PLUGIN_CLASS* createInstance() {
    return new plugins::PLUGIN_CLASS();
  }
  
  extern "C" void destroyInstance(plugins::PLUGIN_CLASS* icDestroy) {
    delete icDestroy;
  }
}
