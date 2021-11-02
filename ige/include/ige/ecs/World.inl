#include "World.hpp"
#include "ige/utility/Control.hpp"
#include <functional>

namespace ige::ecs {

template <Component... Cs>
template <Component... Css>
Query<Cs..., Css...> Query<Cs...>::all()
{
    return IGE_TODO(Query<Cs..., Css...>);
}

template <Component... Cs>
template <std::invocable<Cs&...> F>
void Query<Cs...>::each(F&& f)
{
    IGE_TODO();
}

template <Component... Cs>
Entity World::spawn(Cs&&... components)
{
    return IGE_TODO(Entity);
}

template <Plugin P>
bool World::load()
{
    // TODO: skip if plugin is already loaded
    spawn(P(*this));

    return true;
}

template <Component... Cs>
void World::add(Entity entity, Cs&&... components)
{
    IGE_TODO();
}

template <Component... Cs>
void World::remove(Entity entity, Cs&&... components)
{
    IGE_TODO();
}

template <Component C>
const C* World::get(Entity entity) const
{
    return IGE_TODO(const C*);
}

template <Component... Cs>
void World::set(Entity entity, Cs&&... components)
{
    IGE_TODO();
}

template <Component C>
const C* World::get() const
{
    return IGE_TODO(const C*);
}

}
