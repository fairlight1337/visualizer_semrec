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


#ifndef __PLUGIN_VISUALIZER_H__
#define __PLUGIN_VISUALIZER_H__


#define PLUGIN_CLASS PluginVisualizer


// System
#include <cstdlib>
#include <iostream>
#include <mutex>

// Designators
#include <designators/Designator.h>

// SDL
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL2_gfxPrimitives.h>

// Private
#include <semrec/Types.h>
#include <semrec/ForwardDeclarations.h>
#include <semrec/Plugin.h>
#include <semrec/plugins/owlexporter/CExporterOwl.h>

#include <visualizer_semrec/TreeNode.h>
#include <visualizer_semrec/Drawable.h>
#include <visualizer_semrec/Physics.h>


namespace semrec {
  namespace plugins {
    class PLUGIN_CLASS : public Plugin, public Drawable {
    private:
      unsigned int m_unWidth;
      unsigned int m_unHeight;
      std::string m_strTitle;
      
      SDL_Window* m_swnWindow;
      SDL_Renderer* m_rdrRenderer;
      SDL_GLContext m_gxContext;
      
      CExporterOwl* m_expOwl;
      TreeNode::Ptr m_tnTree;
      TreeNode::Ptr m_tnActive;
      
      std::mutex m_mtxAccess;
      
      double m_dZoom;
      
      Physics::State2D m_s2State;
      
    public:
      PLUGIN_CLASS();
      ~PLUGIN_CLASS();
      
      virtual Result init(int argc, char** argv);
      virtual Result deinit();
      
      virtual Result cycle();
      
      virtual void consumeEvent(Event evEvent);
      
      TreeNode::Ptr addTreeNode(std::string strIdentifier);
      void addTreeNode(TreeNode::Ptr tnAdd);
      void leaveTreeNode();
      
      int branchWidth(TreeNode::Ptr tnRoot);
      
      void dispatchEvents();
      void draw();
      virtual void draw(SDL_Renderer* rdrRenderer) override;
      
      void setZoom(double dZoom);
      double zoom();
      
      void setPosition(Physics::Vector2D v2Position);
      Physics::Vector2D position();
      
      void setVelocity(Physics::Vector2D v2Velocity);
      Physics::Vector2D velocity();
    };
  }
  
  extern "C" plugins::PLUGIN_CLASS* createInstance();
  extern "C" void destroyInstance(plugins::PLUGIN_CLASS* icDestroy);
}


#endif /* __PLUGIN_VISUALIZER_H__ */
