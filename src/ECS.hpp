#ifndef ECS_H
#define ECS_H
#include <bitset>
#include <cstdint>
#include <typeindex>
#include <unordered_map>
#include <vector>

const uint8_t MAX_COMPONENTS = 32;

/*
 * Bitset to represent which components an entity has or which entities a system
 * is interested in.
 */
typedef std::bitset<MAX_COMPONENTS> Signature;

/*
 * Interface for all components in the ECS (Entity Component System).It provides
 * a protected static member variable to generate unique IDs for each component
 * type.
 */
struct IComponent {
protected:
  static uint8_t m_nextId;
};

class IPool {
public:
  virtual ~IPool() {}
};

class Entity {
private:
  // Unique identifier for an entity.
  uint16_t m_id;

public:
  Entity(uint16_t id) : m_id(id) {};

  // Retrieves the unique identifier of the entity.
  uint16_t getId() const;
};

template <typename T> class Component : public IComponent {
  /*
   * Generates and returns a unique identifier for each call. The identifier is
   * generated using a static variable that increments with each call, ensuring
   * that each ID is unique.
   */
  static uint8_t getId() {
    static auto id = m_nextId++;
    return id;
  }
};

class System {
private:
  /*
   * The signature is used to determine which components the system will handle.
   * It is typically a bitset where each bit represents the presence or absence
   * of a specific component type.
   */
  Signature m_signature;
  /**
   * This vector holds instances of the Entity class, representing all the
   * entities that are currently managed by the ECS. Each entity in this vector
   * can have various components associated with it, allowing for flexible and
   * modular game object management.
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
  const std::vector<Entity> &getEntities() const;

  /*
   * Retrieves the signature of the system, which is used to identify theœ
   * components that the system is interested in.
   */
  const Signature &getSignature() const;

  /*
   * Sets the bit corresponding to the component type T in the system's
   * signature. It ensures that the system will only operate on entities that
   * have the required component.
   */
  template <typename T> void requireComponent();
};

// Pool (container) of objeccts of type T
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
  T &get(uint16_t index) { return m_data[index]; }
  T &operator[](uint16_t index) { return m_data[index]; }
};

class Registry {
private:
  // Number of entities added to the scene.
  uint16_t numEntities = 0;
  /**
   * Collection of component pools. Each pool contains all the data for a
   * certain component type. Collection index is the entity ID.
   */
  std::vector<Pool<IPool> *> componentPools;

  /**
   * Collection of component signatures per entity. Represents which components
   * are enabled for a given entity. Collection index is the entity ID.
   */
  std::vector<Signature> entityComponentSignatures;

  /*
   * Map of active systems. Index is the system `std::type_index`.
   */
  std::unordered_map<std::type_index, System *> systems;

public:
  Registry() = default;
  Entity createEntity();
  void killEntity(Entity entity);
  void addSystem(System system);
};

/*
 * Sets the bit corresponding to the component type T in the system's signature.
 * It ensures that the system will only operate on entities that have this
 * component type.
 */
template <typename T> void System::requireComponent() {
  const auto componentId = Component<T>::getId();
  m_signature.set(componentId);
}

#endif