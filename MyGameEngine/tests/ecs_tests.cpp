#include <gtest/gtest.h>
#include "../src/ECS/ECS.h"
#include "../src/Components/TransformComponent.h"
#include "../src/Components/RigidBodyComponent.h"
#include "../src/Components/HealthComponent.h"

// ========================================
// Entity Tests
// ========================================
TEST(EntityTest, EntityCreation) {
    Registry registry;
    Entity entity = registry.CreateEntity();
    
    EXPECT_GE(entity.GetId(), 0);
}

TEST(EntityTest, MultipleEntities) {
    Registry registry;
    Entity entity1 = registry.CreateEntity();
    Entity entity2 = registry.CreateEntity();
    Entity entity3 = registry.CreateEntity();
    
    EXPECT_NE(entity1.GetId(), entity2.GetId());
    EXPECT_NE(entity2.GetId(), entity3.GetId());
    EXPECT_NE(entity1.GetId(), entity3.GetId());
}

TEST(EntityTest, EntityComparison) {
    Registry registry;
    Entity entity1 = registry.CreateEntity();
    Entity entity2 = registry.CreateEntity();
    Entity entity3(entity1);
    
    EXPECT_NE(entity1, entity2);
    EXPECT_EQ(entity1, entity3);
    EXPECT_LT(entity1.GetId(), entity2.GetId());
}

// ========================================
// Component Management Tests
// ========================================
TEST(EntityTest, AddComponent) {
    Registry registry;
    Entity entity = registry.CreateEntity();
    
    entity.AddComponent<TransformComponent>(glm::vec2(10, 20));
    
    EXPECT_TRUE(entity.HasComponent<TransformComponent>());
}

TEST(EntityTest, GetComponent) {
    Registry registry;
    Entity entity = registry.CreateEntity();
    
    entity.AddComponent<TransformComponent>(glm::vec2(100, 200));
    
    auto& transform = entity.GetComponent<TransformComponent>();
    
    EXPECT_FLOAT_EQ(transform.position.x, 100.0f);
    EXPECT_FLOAT_EQ(transform.position.y, 200.0f);
}

TEST(EntityTest, MultipleComponents) {
    Registry registry;
    Entity entity = registry.CreateEntity();
    
    entity.AddComponent<TransformComponent>(glm::vec2(50, 75));
    entity.AddComponent<RigidBodyComponent>(glm::vec2(10, -5));
    entity.AddComponent<HealthComponent>(100);
    
    EXPECT_TRUE(entity.HasComponent<TransformComponent>());
    EXPECT_TRUE(entity.HasComponent<RigidBodyComponent>());
    EXPECT_TRUE(entity.HasComponent<HealthComponent>());
}

TEST(EntityTest, RemoveComponent) {
    Registry registry;
    Entity entity = registry.CreateEntity();
    
    entity.AddComponent<TransformComponent>();
    EXPECT_TRUE(entity.HasComponent<TransformComponent>());
    
    entity.RemoveComponent<TransformComponent>();
    EXPECT_FALSE(entity.HasComponent<TransformComponent>());
}

TEST(EntityTest, ModifyComponent) {
    Registry registry;
    Entity entity = registry.CreateEntity();
    
    entity.AddComponent<HealthComponent>(100);
    
    auto& health = entity.GetComponent<HealthComponent>();
    health.healthPercentage = 75;
    
    EXPECT_EQ(entity.GetComponent<HealthComponent>().healthPercentage, 75);
}

TEST(EntityTest, HasComponentBeforeAdd) {
    Registry registry;
    Entity entity = registry.CreateEntity();
    
    EXPECT_FALSE(entity.HasComponent<TransformComponent>());
}

// ========================================
// Registry Tests
// ========================================
TEST(RegistryTest, CreateMultipleEntities) {
    Registry registry;
    
    for (int i = 0; i < 100; i++) {
        Entity entity = registry.CreateEntity();
        EXPECT_GE(entity.GetId(), 0);
    }
}

TEST(RegistryTest, AddComponentToEntity) {
    Registry registry;
    Entity entity = registry.CreateEntity();
    
    registry.AddComponent<TransformComponent>(entity, glm::vec2(25, 35));
    
    EXPECT_TRUE(entity.HasComponent<TransformComponent>());
    
    auto& transform = entity.GetComponent<TransformComponent>();
    EXPECT_FLOAT_EQ(transform.position.x, 25.0f);
    EXPECT_FLOAT_EQ(transform.position.y, 35.0f);
}

TEST(RegistryTest, RemoveComponentFromEntity) {
    Registry registry;
    Entity entity = registry.CreateEntity();
    
    entity.AddComponent<RigidBodyComponent>(glm::vec2(5, 5));
    EXPECT_TRUE(entity.HasComponent<RigidBodyComponent>());
    
    registry.RemoveComponent<RigidBodyComponent>(entity);
    EXPECT_FALSE(entity.HasComponent<RigidBodyComponent>());
}

// ========================================
// Tag and Group Tests
// ========================================
TEST(EntityTest, TagEntity) {
    Registry registry;
    Entity entity = registry.CreateEntity();
    
    entity.setTag("player");
    
    EXPECT_TRUE(entity.HasTag("player"));
}

TEST(RegistryTest, GetEntityByTag) {
    Registry registry;
    Entity player = registry.CreateEntity();
    player.setTag("player");
    
    Entity retrieved = registry.GetEntityByTag("player");
    
    EXPECT_EQ(player.GetId(), retrieved.GetId());
}

TEST(EntityTest, GroupEntity) {
    Registry registry;
    Entity enemy1 = registry.CreateEntity();
    Entity enemy2 = registry.CreateEntity();
    
    enemy1.setGroup("enemies");
    enemy2.setGroup("enemies");
    
    EXPECT_TRUE(enemy1.BelongsToGroup("enemies"));
    EXPECT_TRUE(enemy2.BelongsToGroup("enemies"));
}

TEST(RegistryTest, GetEntitiesByGroup) {
    Registry registry;
    
    Entity enemy1 = registry.CreateEntity();
    Entity enemy2 = registry.CreateEntity();
    Entity enemy3 = registry.CreateEntity();
    
    enemy1.setGroup("enemies");
    enemy2.setGroup("enemies");
    enemy3.setGroup("enemies");
    
    auto enemies = registry.GetEntitiesByGroup("enemies");
    
    EXPECT_EQ(enemies.size(), 3);
}

TEST(RegistryTest, MixedGroups) {
    Registry registry;
    
    Entity player = registry.CreateEntity();
    Entity enemy1 = registry.CreateEntity();
    Entity enemy2 = registry.CreateEntity();
    Entity obstacle = registry.CreateEntity();
    
    player.setGroup("players");
    enemy1.setGroup("enemies");
    enemy2.setGroup("enemies");
    obstacle.setGroup("obstacles");
    
    auto enemies = registry.GetEntitiesByGroup("enemies");
    auto players = registry.GetEntitiesByGroup("players");
    auto obstacles = registry.GetEntitiesByGroup("obstacles");
    
    EXPECT_EQ(enemies.size(), 2);
    EXPECT_EQ(players.size(), 1);
    EXPECT_EQ(obstacles.size(), 1);
}

// ========================================
// System Tests
// ========================================
class TestSystem : public System {
public:
    TestSystem() {
        RequireComponent<TransformComponent>();
        RequireComponent<RigidBodyComponent>();
    }
};

TEST(SystemTest, SystemCreation) {
    TestSystem system;
    SUCCEED();
}

TEST(SystemTest, RequireComponents) {
    Registry registry;
    registry.AddSystem<TestSystem>();
    
    Entity entity1 = registry.CreateEntity();
    entity1.AddComponent<TransformComponent>();
    entity1.AddComponent<RigidBodyComponent>();
    
    Entity entity2 = registry.CreateEntity();
    entity2.AddComponent<TransformComponent>();
    
    registry.Update();
    
    auto& system = registry.GetSystem<TestSystem>();
    auto entities = system.GetSystemEntities();
    
    // Pelo menos entity1 deve estar no sistema
    EXPECT_GE(entities.size(), 1);
}
