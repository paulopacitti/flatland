#include "./ECS.hpp"

uint16_t Entity::getId() const { return m_id; }

void System::addEntity(Entity entity) { m_entities.push_back(entity); }

void System::removeEntity(Entity entity) {
  /* C++ lambda built-in approach:
   m_entities.erase(std::remove_if(m_entities.begin(), m_entities.end(),
           [entity](const Entity &e) { return e.getId() == entity.getId() ;})
           m_entities.end());
  */

  for (auto e = m_entities.begin(); e != m_entities.end();) {
    if (e->getId() == entity.getId()) {
      m_entities.erase(e);
      break;
    }
    ++e;
  }
}

const std::vector<Entity> &System::getEntities() const { return m_entities; }

const Signature &System::getSignature() const { return m_signature; }