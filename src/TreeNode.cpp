#include <visualizer_semrec/TreeNode.h>


TreeNode::TreeNode(std::string strIdentifier, unsigned int unWidth, unsigned int unHeight, SDL_Color colText, SDL_Color colBackground) : m_strIdentifier(strIdentifier), m_tnParent(nullptr), m_unWidth(unWidth), m_unHeight(unHeight), m_colText(colText), m_colBackground(colBackground), m_s2State({{0, 0}, {0, 0}}) {
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

void TreeNode::draw(SDL_Renderer* rdrRenderer, Physics::Vector2D v2Offset) {
  SDL_Color colBackground;
  
  if(this->selected()) {
    colBackground = {255, 200, 200, 255};
  } else {
    colBackground = this->backgroundColor();
  }
  
  this->drawTextBox(rdrRenderer, this->x() + v2Offset.dX, this->y() + v2Offset.dY, this->width(), this->height(), this->identifier(), this->textColor(), colBackground);
  
  unsigned int unRowWidth = 0;
  
  for(TreeNode::Ptr tnChild : m_vecChildren) {
    if(unRowWidth != 0) {
      unRowWidth += m_nSiblingDistance;
    }
    
    unRowWidth + tnChild->width();
  }
  
  unsigned int unOffset = this->x() - (unRowWidth / 2);
  
  for(TreeNode::Ptr tnChild : m_vecChildren) {
    int nXChild = unOffset;
    
    tnChild->draw(rdrRenderer, v2Offset);
    
    int nX1 = this->x() + v2Offset.dX;
    int nY1 = this->y() + this->height() / 2 + v2Offset.dY;
    int nX2 = tnChild->x() + v2Offset.dX;
    int nY2 = tnChild->y() - tnChild->height() / 2 + v2Offset.dY;
    
    if(nY2 > nY1) {
      this->drawLine(rdrRenderer, nX1, nY1, nX1, (nY1 + nY2) / 2, {0, 0, 0, 255});
      this->drawLine(rdrRenderer, nX1, (nY1 + nY2) / 2, nX2, (nY1 + nY2) / 2, {0, 0, 0, 255});
      this->drawLine(rdrRenderer, nX2, (nY1 + nY2) / 2, nX2, nY2, {0, 0, 0, 255});
    }
  }
}

void TreeNode::setX(int nX) {
  m_s2Goal.v2Position.dX = nX;
  //m_s2State.v2Position.dX = nX;
}

int TreeNode::x() {
  return m_s2State.v2Position.dX;
}

void TreeNode::setY(int nY) {
  m_s2Goal.v2Position.dY = nY;
  //m_s2State.v2Position.dY = nY;
}

int TreeNode::y() {
  return m_s2State.v2Position.dY;
}

void TreeNode::setState(Physics::State2D s2Set) {
  m_s2State = s2Set;
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

void TreeNode::recalculatePositions(bool bRecurse) {
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
    
    tnChild->setX(this->x() + unOffset - unSize / 2 + unChildSize / 2);
    unOffset += unChildSize + m_nSiblingDistance;
    
    tnChild->setY(this->y() + m_nLevelDistance);
    
    if(bRecurse) {
      tnChild->recalculatePositions();
    }
  }
}

void TreeNode::applyPhysics(double dElapsed) {
  double dK = 10.0;
  
  m_s2State.v2Velocity.dX = dK * (m_s2Goal.v2Position.dX - m_s2State.v2Position.dX);
  m_s2State.v2Velocity.dY = dK * (m_s2Goal.v2Position.dY - m_s2State.v2Position.dY);
  
  Physics::applyPhysics(m_s2State, dElapsed);
  
  for(TreeNode::Ptr tnChild : m_vecChildren) {
    this->recalculatePositions(false);
    
    tnChild->applyPhysics(dElapsed);
  }
}
