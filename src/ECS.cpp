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

const std::vector<Entity>& System::getEntities() const { return m_entities; }

const Signature& System::getSignature() const { return m_signature; }

// --------- Registry implementation -----------

Entity Registry::createEntity() {
  uint16_t entityId = m_numEntities++;

  Entity entity(entityId);
  entity.registry = this;
  m_entitiesToBeAdded.insert(entity);
  if (entityId >= m_entityComponentSignatures.size()) {
    m_entityComponentSignatures.resize(entityId + 1);
  }

  spdlog::info("[Registry] Entity created with id = {}.", entityId);
  return entity;
}

void Registry::addEntityToSystems(Entity entity) {
  const uint16_t entityId = entity.getId();
  const auto entityComponentSignature = m_entityComponentSignatures[entityId];

  for (auto& system : m_systems) {
    const auto& systemComponentSignature = system.second->getSignature();

    if ((entityComponentSignature & systemComponentSignature) ==
        systemComponentSignature) {
      system.second->addEntity(entity);
    }
  }
}

void Registry::update() {
  for (auto entity : m_entitiesToBeAdded) {
    addEntityToSystems(entity);
  }
  m_entitiesToBeAdded.clear();

  // TODO: remove the entities in `m_entitiesToBeRemoved` from the active
  // systems.
}
