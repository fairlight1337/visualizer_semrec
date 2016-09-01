#include <visualizer_semrec/TreeNode.h>


TreeNode::TreeNode(std::string strIdentifier, unsigned int unWidth, unsigned int unHeight, SDL_Color colText, SDL_Color colBackground) : m_strIdentifier(strIdentifier), m_tnParent(nullptr), m_unWidth(unWidth), m_unHeight(unHeight), m_colText(colText), m_colBackground(colBackground) {
}

TreeNode::~TreeNode() {
}

void TreeNode::setParent(TreeNode::Ptr tnParent) {
  m_tnParent = tnParent;
}

TreeNode::Ptr TreeNode::parent() {
  return m_tnParent;
}

void TreeNode::addChild(TreeNode::Ptr tnAdd) {
  m_vecChildren.push_back(tnAdd);
  tnAdd->setParent(this->shared_from_this());
}

std::vector<TreeNode::Ptr> TreeNode::children() {
  return m_vecChildren;
}

std::string TreeNode::identifier() {
  return m_strIdentifier;
}

unsigned int TreeNode::width() {
  return m_unWidth;
}

unsigned int TreeNode::height() {
  return m_unHeight;
}

SDL_Color TreeNode::textColor() {
  return m_colText;
}

SDL_Color TreeNode::backgroundColor() {
  return m_colBackground;
}
