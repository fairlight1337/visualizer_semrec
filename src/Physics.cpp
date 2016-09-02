#include <visualizer_semrec/Physics.h>


Physics::Physics() {
}

Physics::~Physics() {
}

void Physics::applyPhysics(State2D& s2State, double dTimeElapsed) {
  s2State.v2Position.dX += s2State.v2Velocity.dX * dTimeElapsed;
  s2State.v2Position.dY += s2State.v2Velocity.dY * dTimeElapsed;
}
