#ifndef ECS_H
#define ECS_H
#include "spdlog/spdlog.h"
#include <bitset>
#include <cstdint>
#include <memory>
#include <set>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>

/**
 * Upper limit for the number of components that can be associated with a single
 * entity in the ECS (Entity Component System). It is set to 32, meaning an
 * entity can have up to 32 different components.
 */
const uint8_t MAX_COMPONENTS = 32;

/*
 * Bitset to represent which components an entity has or which entities a system
 * is interested in.
 */
typedef std::bitset<MAX_COMPONENTS> Signature;

// ------------ Entity ---------------------------------------------------------

class Entity {
  private:
    // Unique identifier for an entity.
    uint16_t m_id;
    // Entity's owner (forward declaration)

  public:
    class Registry* registry;
    Entity(uint16_t id) : m_id(id) {};

    // Retrieves the unique identifier of the entity.
    uint16_t getId() const;

    // Required to be placed in a set
    bool operator<(const Entity& e) const { return m_id < e.getId(); }

    template <typename TComponent, typename... TArgs>
    void addComponent(TArgs&&... args);
    template <typename TComponent> void removeComponent();
    template <typename TComponent> bool hasComponent() const;
    template <typename TComponent> TComponent& getComponent() const;
};

// ----------- Component ----------------

/*
 * Interface for all components in the ECS (Entity Component System).It provides
 * a protected static member variable to generate unique IDs for each component
 * type.
 */
struct IComponent {
  protected:
    static uint8_t nextId;
};

template <typename TComponent> class Component : public IComponent {
    /*
     * Generates and returns a unique identifier for each call. The identifier
     * is generated using a static variable that increments with each call,
     * ensuring that each ID is unique.
     */
  public:
    static uint8_t getId() {
      static auto id = nextId++;
      return id;
    }
};

// --------------- System ---------------

class System {
  private:
    /*
     * The signature is used to determine which components the system will
     * handle. It is typically a bitset where each bit represents the presence
     * or absence of a specific component type.
     */
    Signature m_signature;
    /**
     * This vector holds instances of the Entity class, representing all the
     * entities that are currently managed by the ECS. Each entity in this
     * vector can have various components associated with it, allowing for
     * flexible and modular game object management.
     */
    std::vector<Entity> m_entities;

  public:
    System() = default;
    ~System() = default;

    // Adds an entity to the system.
    void addEntity(Entity entity);

    // Removes an entity from the system.
    void removeEntity(Entity entity);

    /*
     * Returns a constant reference to a vector containing the
     * entities that are currently managed by the system.
     */
    const std::vector<Entity>& getEntities() const;

    /*
     * Retrieves the signature of the system, which is used to identify theœ
     * components that the system is interested in.
     */
    const Signature& getSignature() const;

    /*
     * Sets the bit corresponding to the component type T in the system's
     * signature. It ensures that the system will only operate on entities that
     * have the required component.
     */
    template <typename T> void requireComponent();
};

// -------------- Registry ---------------------

/**
 * Interface for a pool of objects, providing a virtual destructor.
 * Derived classes should implement the specific functionality for managing the
 * pool of objects.
 */
class IPool {
  public:
    virtual ~IPool() {}
};

// Pool (container) of objects of type T
template <typename T> class Pool : public IPool {
  private:
    std::vector<T> m_data;

  public:
    Pool(uint16_t size = 100) { resize(size); }
    virtual ~Pool() = default;

    bool isEmpty() const { return m_data.empty(); }
    uint16_t getSize() const { return m_data.size(); }
    void resize(uint16_t size) { m_data.resize(size); }
    void clear() { m_data.clear(); }
    void add(T object) { m_data.push_back(object); }
    void set(uint16_t index, T object) { m_data[index] = object; };
    T& get(uint16_t index) { return m_data[index]; }
    T& operator[](uint16_t index) { return m_data[index]; }
};

class Registry {
  private:
    // Number of entities added to the scene.
    uint16_t m_numEntities = 0;

    // Entities buffer to be created in the next `Registry.update()`.
    std::set<Entity> m_entitiesToBeAdded;

    // Entities buffer to be removed in the next `Registry.update()`.
    std::set<Entity> m_entitiesToBeRemoved;

    /**
     * Collection of component pools. Each pool contains all the data for a
     * certain component type. Collection index is the entity ID.
     */
    std::vector<std::shared_ptr<IPool>> m_componentPools;

    /**
     * Collection of component signatures per entity. Represents which
     * components are enabled for a given entity. Collection index is the entity
     * ID.
     */
    std::vector<Signature> m_entityComponentSignatures;

    /*
     * Map of active systems. Index is the system `std::type_index`.
     */
    std::unordered_map<std::type_index, std::shared_ptr<System>> m_systems;

  public:
    Registry() { spdlog::info("[Registry] created."); }
    ~Registry() { spdlog::info("[Registry] destroyed."); }
    Entity createEntity();

    void removeEntity(Entity entity);
    /**
      Add/remove entities that are in the `m_entitiesToBeAdded` and
      `m_entitiesToBeRemoved` buffers. This function exists so entities are not
      added/removed during the frame logic. This update happens after the end of
      the frame update.
     */
    void update();

    /**
     * Adds a new component of type TComponent to the specified entity.
     * Forwards the provided arguments to the constructor of the component.
     */
    template <typename TComponent, typename... TArgs>
    void addComponent(Entity entity, TArgs&&... args);
    template <typename TComponent> void removeComponent(Entity entity);
    template <typename TComponent> bool hasComponent(Entity entity) const;
    template <typename TComponent>
    TComponent& getComponent(Entity entity) const;

    template <typename TSystem, typename... TArgs>
    void addSystem(TArgs&&... args);
    template <typename TSystem> void removeSystem();
    template <typename TSystem> bool hasSystem() const;
    template <typename TSystem> TSystem& getSystem() const;

    /** Checks the component signature of an entity and add the entity to the
     * systems that are interested in it.
     */
    void addEntityToSystems(Entity entity);
};

// ----------------------- Template functions implementation -----------

template <typename TComponent, typename... TArgs>
void Entity::addComponent(TArgs&&... args) {
  this->registry->addComponent<TComponent>(*this, std::forward<TArgs>(args)...);
}

template <typename TComponent> void Entity::removeComponent() {
  this->registry->removeComponent<TComponent>(*this);
}

template <typename TComponent> bool Entity::hasComponent() const {
  return this->registry->hasComponent<TComponent>(*this);
}

template <typename TComponent> TComponent& Entity::getComponent() const {
  return this->registry->getComponent<TComponent>(*this);
}

template <typename TComponent, typename... TArgs>
void Registry::addComponent(Entity entity, TArgs&&... args) {
  const uint8_t componentId = Component<TComponent>::getId();
  const uint16_t entityId = entity.getId();

  // resize m_componentPools if componentId does not exist in vector
  if (componentId >= m_componentPools.size()) {
    m_componentPools.resize(componentId + 1, nullptr);
  }

  // if a pool for that component does not exist, create one
  if (!m_componentPools[componentId]) {
    std::shared_ptr<Pool<TComponent>> newComponentPool =
        std::make_shared<Pool<TComponent>>();
    m_componentPools[componentId] = newComponentPool;
  }

  // get the pool of the component values for that component type
  std::shared_ptr<Pool<TComponent>> componentPool =
      std::static_pointer_cast<Pool<TComponent>>(m_componentPools[componentId]);

  // resize current component pool if new component does not fit
  if (entityId >= componentPool->getSize()) {
    componentPool->resize(m_numEntities);
  }

  // create new component object of the type T, and forward the various
  // parameters to the constructor
  TComponent newComponent(std::forward<TArgs>(args)...);

  // add the new component to the component pool list, using the entityId as
  // index
  componentPool->set(entityId, newComponent);

  // finally, change the component signature of the entity.
  m_entityComponentSignatures[entityId].set(componentId);
  spdlog::info("[Registry] componentId=" + std::to_string(componentId) +
               " added to entityId= " + std::to_string(componentId));
}

template <typename TComponent> void Registry::removeComponent(Entity entity) {
  const uint8_t componentId = Component<TComponent>::getId();
  const uint16_t entityId = entity.getId();

  m_entityComponentSignatures[entityId].set(componentId, false);
  spdlog::info("[Registry] componentId=" + std::to_string(componentId) +
               " was removed from entityId=" + std::to_string(entityId));
}

template <typename TComponent>
bool Registry::hasComponent(Entity entity) const {
  const uint8_t componentId = Component<TComponent>::getId();
  const uint16_t entityId = entity.getId();

  return m_entityComponentSignatures[entityId].test(componentId);
}

template <typename TComponent>
TComponent& Registry::getComponent(Entity entity) const {
  const uint8_t componentId = Component<TComponent>::getId();
  const uint16_t entityId = entity.getId();
  auto componentPool =
      std::static_pointer_cast<Pool<TComponent>>(m_componentPools[componentId]);

  return componentPool->get(entityId);
}

template <typename TComponent> void System::requireComponent() {
  const auto componentId = Component<TComponent>::getId();
  m_signature.set(componentId);
}

template <typename TSystem, typename... TArgs>
void Registry::addSystem(TArgs&&... args) {
  std::shared_ptr<TSystem> newSystem =
      std::make_shared<TSystem>(std::forward<TArgs>(args)...);
  m_systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));
}

template <typename TSystem> void Registry::removeSystem() {
  auto system = m_systems.find(std::type_index(typeid(TSystem)));
  m_systems.erase(system);
}

template <typename TSystem> bool Registry::hasSystem() const {
  return m_systems.find(std::type_index(typeid(TSystem))) != m_systems.end();
}

template <typename TSystem> TSystem& Registry::getSystem() const {
  auto system = m_systems.find(std::type_index(typeid(TSystem)));
  return *(std::static_pointer_cast<TSystem>(system->second));
}

#endif
