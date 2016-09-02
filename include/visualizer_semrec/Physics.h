#ifndef __PHYSICS_H__
#define __PHYSICS_H__


#include <memory>
#include <iostream>


class Physics {
public:
  typedef std::shared_ptr<Physics> Ptr;
  
  typedef struct {
    double dX;
    double dY;
  } Vector2D;
  
  typedef struct {
    Vector2D v2Position;
    Vector2D v2Velocity;
  } State2D;
  
private:
protected:
public:
  Physics();
  ~Physics();
  
  static void applyPhysics(State2D& s2State, double dTimeElapsed);
  
  template<class ... Args>
    static Physics::Ptr create(Args ... args) {
    return std::make_shared<Physics>(std::forward<Args>(args)...);
  }
};


#endif /* __PHYSICS_H__ */
