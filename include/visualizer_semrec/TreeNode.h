#ifndef __TREENODE_H__
#define __TREENODE_H__


#include <memory>
#include <iostream>
#include <vector>
#include <string>

#include <SDL/SDL.h>


class TreeNode : public std::enable_shared_from_this<TreeNode> {
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
  
  template<class ... Args>
    static TreeNode::Ptr create(Args ... args) {
    return std::make_shared<TreeNode>(std::forward<Args>(args)...);
  }
};


#endif /* __TREENODE_H__ */
