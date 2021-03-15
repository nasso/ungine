/*
** EPITECH PROJECT, 2021
** un
** File description:
** world
*/

#include "un/ecs/World.hpp"
#include "rtl/Option.hpp"
#include "gtest/gtest.h"

using rtl::some;
using un::ecs::World;

TEST(WorldTest, Spawn)
{
    World world;

    World::EntityId entities[] = {
        world.createEntity(),
        world.createEntity(),
        world.createEntity(),
    };

    ASSERT_NE(entities[0], entities[1]);
    ASSERT_NE(entities[1], entities[2]);
    ASSERT_NE(entities[2], entities[0]);
}

TEST(WorldTest, SetResource)
{
    World world;

    world.set<std::string>("hello");
    world.set<int>(65);
    world.set<float>(48.3f);
    world.set<double>();
    world.set<std::pair<int, int>>(38, 19);
}

TEST(WorldTest, GetResource)
{
    World world;

    world.set<std::string>("hello");
    world.set<int>(65);
    world.set<float>(48.3f);
    world.set<double>();
    world.set<std::pair<int, int>>(38, 19);

    ASSERT_EQ(world.get<std::string>(), some("hello"));
    ASSERT_EQ(world.get<int>(), some(65));
    ASSERT_EQ(world.get<float>(), some(48.3f));
    ASSERT_EQ(world.get<double>(), some(0));
    ASSERT_EQ((world.get<std::pair<int, int>>()), some(std::make_pair(38, 19)));
    ASSERT_TRUE(world.get<std::vector<int>>().is_none());
}

TEST(WorldTest, AddComponent)
{
    World world;

    auto ent = world.createEntity();

    world.addComponent<std::string>(ent, "hello");
    world.addComponent<int>(ent, 65);
    world.addComponent<float>(ent, 48.3f);
    world.addComponent<double>(ent);
    world.addComponent<std::pair<int, int>>(ent, 38, 19);
}

TEST(WorldTest, GetComponent)
{
    World world;

    auto ent = world.createEntity();

    world.addComponent<std::string>(ent, "hello");
    world.addComponent<int>(ent, 65);
    world.addComponent<float>(ent, 48.3f);
    world.addComponent<double>(ent);
    world.addComponent<std::pair<int, int>>(ent, 38, 19);

    ASSERT_EQ(world.getComponent<std::string>(ent), some("hello"));
    ASSERT_EQ(world.getComponent<int>(ent), some(65));
    ASSERT_EQ(world.getComponent<float>(ent), some(48.3f));
    ASSERT_EQ(world.getComponent<double>(ent), some(0));
    ASSERT_EQ((world.getComponent<std::pair<int, int>>(ent)),
        some(std::make_pair(38, 19)));
    ASSERT_TRUE(world.getComponent<std::vector<int>>(ent).is_none());
}

TEST(WorldTest, RemoveComponent)
{
    World world;

    auto ent = world.createEntity();

    world.addComponent<std::string>(ent, "hello");
    world.addComponent<int>(ent, 65);
    world.addComponent<float>(ent, 48.3f);
    world.addComponent<double>(ent);
    world.addComponent<std::pair<int, int>>(ent, 38, 19);

    ASSERT_EQ(world.removeComponent<std::string>(ent), some("hello"));
    ASSERT_EQ(world.removeComponent<int>(ent), some(65));

    ASSERT_TRUE(world.getComponent<std::string>(ent).is_none());
    ASSERT_TRUE(world.getComponent<int>(ent).is_none());
    ASSERT_TRUE(world.getComponent<std::vector<int>>(ent).is_none());
    ASSERT_EQ(world.getComponent<float>(ent), some(48.3f));
    ASSERT_EQ(world.getComponent<double>(ent), some(0));
    ASSERT_EQ((world.getComponent<std::pair<int, int>>(ent)),
        some(std::make_pair(38, 19)));
}

TEST(WorldTest, RemoveEntity)
{
    World world;

    auto ent = world.createEntity();

    world.addComponent<std::string>(ent, "hello");
    world.addComponent<int>(ent, 65);
    world.addComponent<float>(ent, 48.3f);
    world.addComponent<double>(ent);
    world.addComponent<std::pair<int, int>>(ent, 38, 19);

    world.removeEntity(ent);

    ASSERT_TRUE(world.getComponent<std::string>(ent).is_none());
    ASSERT_TRUE(world.getComponent<int>(ent).is_none());
    ASSERT_TRUE(world.getComponent<float>(ent).is_none());
    ASSERT_TRUE(world.getComponent<double>(ent).is_none());
    ASSERT_TRUE(world.getComponent<std::vector<int>>(ent).is_none());
}
