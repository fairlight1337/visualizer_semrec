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
    PLUGIN_CLASS::PLUGIN_CLASS() : m_expOwl(nullptr), m_tnTree(nullptr), m_tnActive(nullptr) {
      this->addDependency("symboliclog");
      this->setPluginVersion("0.1");
    }
    
    PLUGIN_CLASS::~PLUGIN_CLASS() {
      if(m_expOwl) {
	delete m_expOwl;
      }
    }
    
    TreeNode::Ptr PLUGIN_CLASS::addTreeNode(std::string strIdentifier) {
      unsigned int unWidth = 200; // this->textWidth(strIdentifier) + 10
      
      TreeNode::Ptr tnAdd = TreeNode::create(strIdentifier, unWidth);
      this->addTreeNode(tnAdd);
      
      return tnAdd;
    }
    
    void PLUGIN_CLASS::addTreeNode(TreeNode::Ptr tnAdd) {
      std::lock_guard<std::mutex> lgLock(m_mtxAccess);
      
      if(m_tnActive) {
	m_tnActive->setSelected(false);
      }
      
      if(tnAdd) {
	if(!m_tnTree) {
	  m_tnTree = tnAdd;
	} else {
	  if(!m_tnActive) {
	    m_tnActive = m_tnTree;
	  }
	  
	  m_tnActive->addChild(tnAdd);
	}
	
	m_tnActive = tnAdd;
	m_tnActive->setSelected(true);
      }
      
      if(m_tnTree) {
	m_tnTree->setX(m_unWidth / 2);
	m_tnTree->setY(m_unHeight / 2);
	
	m_tnTree->recalculatePositions();
      }
    }
    
    void PLUGIN_CLASS::leaveTreeNode() {
      std::lock_guard<std::mutex> lgLock(m_mtxAccess);
      
      if(m_tnActive) {
	m_tnActive->setSelected(false);
	
	m_tnActive = m_tnActive->parent();
	
	if(m_tnActive) {
	  m_tnActive->setSelected(true);
	}
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
      s_strFontFile = cdConfig->stringValue("font-file");
      
      if(strSemanticsDescriptorFile != "") {
	if(m_expOwl->loadSemanticsDescriptorFile(strSemanticsDescriptorFile) == false) {
	  this->warn("Failed to load semantics descriptor file '" + strSemanticsDescriptorFile + "'.");
	}
      } else {
	this->warn("No semantics descriptor file was specified.");
      }
      
      if(SDL_Init(SDL_INIT_EVERYTHING) >= 0) {
	if(TTF_Init() >= 0) {
	  Drawable::s_ttfFont = TTF_OpenFont(Drawable::s_strFontFile.c_str(), 16);
	  
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
      std::lock_guard<std::mutex> lgLock(m_mtxAccess);
      
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
	m_tnTree->draw(rdrRenderer);
	this->drawTreeNode(rdrRenderer, m_tnTree, m_unWidth / 2, m_unHeight / 2);
      }
    }
    
    void PLUGIN_CLASS::drawTreeNode(SDL_Renderer* rdrRenderer, TreeNode::Ptr tnDraw, int nX, int nY) {
    }
    
    int PLUGIN_CLASS::branchWidth(TreeNode::Ptr tnRoot) {
      return 0;
    }
  }
  
  extern "C" plugins::PLUGIN_CLASS* createInstance() {
    return new plugins::PLUGIN_CLASS();
  }
  
  extern "C" void destroyInstance(plugins::PLUGIN_CLASS* icDestroy) {
    delete icDestroy;
  }
}
