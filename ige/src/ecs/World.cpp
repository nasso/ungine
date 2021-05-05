/*
** EPITECH PROJECT, 2021
** ige
** File description:
** World
*/

#include "ige/ecs/World.hpp"

namespace ige {
namespace ecs {

    World::EntityRef::EntityRef(World& wld, EntityId id)
        : m_wld(wld)
        , m_id(id)
    {
    }

    bool World::EntityRef::operator==(const EntityRef& other) const
    {
        return m_id == other.m_id && &m_wld == &other.m_wld;
    }

    bool World::EntityRef::operator!=(const EntityRef& other) const
    {
        return !(*this == other);
    }

    bool World::EntityRef::remove()
    {
        return m_wld.remove_entity(m_id);
    }

    World::EntityRef World::create_entity()
    {
        return { *this, ++m_last_entity };
    }

    bool World::remove_entity(EntityId ent)
    {
        for (auto& comp : m_components) {
            comp.second(ent);
        }

        return true;
    }

}
}
