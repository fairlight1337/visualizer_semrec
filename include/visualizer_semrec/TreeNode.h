#ifndef __TREENODE_H__
#define __TREENODE_H__


#include <memory>
#include <iostream>
#include <vector>
#include <string>

#include <visualizer_semrec/Physics.h>
#include <visualizer_semrec/Drawable.h>


class TreeNode : public std::enable_shared_from_this<TreeNode>, public Drawable {
public:
  typedef std::shared_ptr<TreeNode> Ptr;
  
private:
  std::string m_strIdentifier;
  std::vector<TreeNode::Ptr> m_vecChildren;
  TreeNode::Ptr m_tnParent;
  
  unsigned int m_unWidth;
  unsigned int m_unHeight;
  
  SDL_Color m_colText;
  SDL_Color m_colBackground;
  
  bool m_bSelected;
  
  const int m_nLevelDistance = 50;
  const int m_nSiblingDistance = 20;
  
  Physics::State2D m_s2State;
  
protected:
public:
  TreeNode(std::string strIdentifier, unsigned int unWidth = 200, unsigned int unHeight = 30, SDL_Color colText = {255, 255, 255, 255}, SDL_Color colBackground = {0, 128, 0, 255});
  ~TreeNode();
  
  void setParent(TreeNode::Ptr tnParent);
  TreeNode::Ptr parent();
  void addChild(TreeNode::Ptr tnAdd);
  std::vector<TreeNode::Ptr> children();
  
  std::string identifier();
  
  unsigned int width();
  unsigned int height();
  
  SDL_Color textColor();
  SDL_Color backgroundColor();
  
  void setSelected(bool bSelected);
  bool selected();
  
  void draw(SDL_Renderer* rdrRenderer, Physics::Vector2D v2Offset) override;
  
  void setX(int nX);
  int x();
  
  void setY(int nY);
  int y();
  
  unsigned int branchWidth();
  void recalculatePositions();
  
  void applyPhysics(double dElapsed);
  
  template<class ... Args>
    static TreeNode::Ptr create(Args ... args) {
    return std::make_shared<TreeNode>(std::forward<Args>(args)...);
  }
};


#endif /* __TREENODE_H__ */
