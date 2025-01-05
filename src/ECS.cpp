#include "./ECS.hpp"
#include "spdlog/spdlog.h"

uint8_t IComponent::nextId = 0;

// -------- Entity implementation ------------

uint16_t Entity::getId() const { return m_id; }

// --------- System implementation -----------

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

// --------- Registry implementation -----------

Entity Registry::createEntity() {
  uint16_t entityId = m_numEntities++;

  Entity entity(entityId);
  m_entitiesToBeAdded.insert(entity);

  spdlog::debug("[Registry] Entity created with id = {}.", entityId);
  return entity;
}

void Registry::update() {
  // TODO: add the entities in `m_entitiesToBeAdded` to the active systems.
  // TODO: remove the entities in `m_entitiesToBeRemoved` from the active
  // systems.
}