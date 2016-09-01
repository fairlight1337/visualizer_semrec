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

void TreeNode::setSelected(bool bSelected) {
  m_bSelected = bSelected;
}

bool TreeNode::selected() {
  return m_bSelected;
}

void TreeNode::draw(SDL_Renderer* rdrRenderer) {
  SDL_Color colBackground;
  
  if(this->selected()) {
    colBackground = {128, 0, 0, 255};
  } else {
    colBackground = this->backgroundColor();
  }
  
  this->drawTextBox(rdrRenderer, m_nX, m_nY, this->width(), this->height(), this->identifier(), this->textColor(), colBackground);
  
  unsigned int unRowWidth = 0;
  
  for(TreeNode::Ptr tnChild : m_vecChildren) {
    if(unRowWidth != 0) {
      unRowWidth += m_nSiblingDistance;
    }
    
    unRowWidth + tnChild->width();
  }
  
  unsigned int unOffset = m_nX - (unRowWidth / 2);
  
  for(TreeNode::Ptr tnChild : m_vecChildren) {
    int nXChild = unOffset;
    
    tnChild->draw(rdrRenderer);
    this->drawLine(rdrRenderer, m_nX, m_nY + this->height() / 2, tnChild->x(), tnChild->y() - tnChild->height() / 2, {0, 128, 0, 255});
  }
}

void TreeNode::setX(int nX) {
  m_nX = nX;
}

int TreeNode::x() {
  return m_nX;
}

void TreeNode::setY(int nY) {
  m_nY = nY;
}

int TreeNode::y() {
  return m_nY;
}

unsigned int TreeNode::branchWidth() {
  unsigned int unWidth = m_unWidth;
  unsigned int unChildrenWidth = (m_vecChildren.size() > 0 ? (m_vecChildren.size() - 1) * m_nSiblingDistance : 0);
  
  for(TreeNode::Ptr tnChild : m_vecChildren) {
    unChildrenWidth += tnChild->branchWidth();
  }
  
  unWidth = (unWidth > unChildrenWidth ? unWidth : unChildrenWidth);
  
  return unWidth;
}

void TreeNode::recalculatePositions() {
  std::vector<unsigned int> vecSizes;
  unsigned int unSize = 0;
  
  for(TreeNode::Ptr tnChild : m_vecChildren) {
    unsigned int unWidth = tnChild->branchWidth();
    vecSizes.push_back(unWidth);
    
    unSize += unWidth;
  }
  
  unsigned int unOffset = 0;
  
  for(unsigned int unI = 0; unI < m_vecChildren.size(); ++unI) {
    TreeNode::Ptr tnChild = m_vecChildren[unI];
    unsigned int unChildSize = vecSizes[unI];
    
    tnChild->setX(m_nX + unOffset - unSize / 2 + unChildSize / 2);
    unOffset += unChildSize + m_nSiblingDistance;
    
    tnChild->setY(m_nY + m_nLevelDistance);
    
    tnChild->recalculatePositions();
  }
}
