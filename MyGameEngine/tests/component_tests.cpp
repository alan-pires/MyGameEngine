#include <gtest/gtest.h>
#include "../src/Components/TransformComponent.h"
#include "../src/Components/RigidBodyComponent.h"
#include "../src/Components/HealthComponent.h"
#include "../src/Components/BoxColliderComponent.h"
#include "../src/Components/ProjectileComponent.h"
#include <glm/glm.hpp>

// ========================================
// TransformComponent Tests
// ========================================
TEST(TransformComponentTest, DefaultConstructor) {
    TransformComponent transform;
    
    EXPECT_FLOAT_EQ(transform.position.x, 0.0f);
    EXPECT_FLOAT_EQ(transform.position.y, 0.0f);
    EXPECT_FLOAT_EQ(transform.scale.x, 1.0f);
    EXPECT_FLOAT_EQ(transform.scale.y, 1.0f);
    EXPECT_DOUBLE_EQ(transform.rotation, 0.0);
}

TEST(TransformComponentTest, ParameterizedConstructor) {
    glm::vec2 pos(100.0f, 200.0f);
    glm::vec2 scale(2.0f, 3.0f);
    
    TransformComponent transform(pos, scale, 45.0);
    
    EXPECT_FLOAT_EQ(transform.position.x, 100.0f);
    EXPECT_FLOAT_EQ(transform.position.y, 200.0f);
    EXPECT_FLOAT_EQ(transform.scale.x, 2.0f);
    EXPECT_FLOAT_EQ(transform.scale.y, 3.0f);
    EXPECT_DOUBLE_EQ(transform.rotation, 45.0);
}

TEST(TransformComponentTest, VelocityAndAcceleration) {
    TransformComponent transform;
    transform.velocity = glm::vec2(50.0f, 75.0f);
    transform.acceleration = 10.0;
    transform.friction = 0.9;
    
    EXPECT_FLOAT_EQ(transform.velocity.x, 50.0f);
    EXPECT_FLOAT_EQ(transform.velocity.y, 75.0f);
    EXPECT_DOUBLE_EQ(transform.acceleration, 10.0);
    EXPECT_DOUBLE_EQ(transform.friction, 0.9);
}

TEST(TransformComponentTest, ModifyPosition) {
    TransformComponent transform(glm::vec2(10, 20));
    transform.position.x += 5.0f;
    transform.position.y += 10.0f;
    
    EXPECT_FLOAT_EQ(transform.position.x, 15.0f);
    EXPECT_FLOAT_EQ(transform.position.y, 30.0f);
}

// ========================================
// RigidBodyComponent Tests
// ========================================
TEST(RigidBodyComponentTest, DefaultConstructor) {
    RigidBodyComponent rigidbody;
    
    EXPECT_FLOAT_EQ(rigidbody.velocity.x, 0.0f);
    EXPECT_FLOAT_EQ(rigidbody.velocity.y, 0.0f);
}

TEST(RigidBodyComponentTest, VelocityConstructor) {
    RigidBodyComponent rigidbody(glm::vec2(100.0f, -50.0f));
    
    EXPECT_FLOAT_EQ(rigidbody.velocity.x, 100.0f);
    EXPECT_FLOAT_EQ(rigidbody.velocity.y, -50.0f);
}

TEST(RigidBodyComponentTest, ModifyVelocity) {
    RigidBodyComponent rigidbody(glm::vec2(50.0f, 50.0f));
    rigidbody.velocity.x = 75.0f;
    
    EXPECT_FLOAT_EQ(rigidbody.velocity.x, 75.0f);
    EXPECT_FLOAT_EQ(rigidbody.velocity.y, 50.0f);
}

// ========================================
// HealthComponent Tests
// ========================================
TEST(HealthComponentTest, DefaultConstructor) {
    HealthComponent health;
    
    EXPECT_EQ(health.healthPercentage, 0);
}

TEST(HealthComponentTest, ValueConstructor) {
    HealthComponent health(75);
    
    EXPECT_EQ(health.healthPercentage, 75);
}

TEST(HealthComponentTest, FullHealth) {
    HealthComponent health(100);
    
    EXPECT_EQ(health.healthPercentage, 100);
}

TEST(HealthComponentTest, TakeDamage) {
    HealthComponent health(100);
    health.healthPercentage -= 25;
    
    EXPECT_EQ(health.healthPercentage, 75);
}

TEST(HealthComponentTest, Healing) {
    HealthComponent health(50);
    health.healthPercentage += 30;
    
    EXPECT_EQ(health.healthPercentage, 80);
}

TEST(HealthComponentTest, ZeroHealth) {
    HealthComponent health(100);
    health.healthPercentage = 0;
    
    EXPECT_EQ(health.healthPercentage, 0);
}

// ========================================
// BoxColliderComponent Tests
// ========================================
TEST(BoxColliderComponentTest, DefaultConstructor) {
    BoxColliderComponent collider;
    
    EXPECT_EQ(collider.width, 0);
    EXPECT_EQ(collider.height, 0);
}

TEST(BoxColliderComponentTest, ParameterizedConstructor) {
    BoxColliderComponent collider(64, 64, glm::vec2(8, 8));
    
    EXPECT_EQ(collider.width, 64);
    EXPECT_EQ(collider.height, 64);
    EXPECT_FLOAT_EQ(collider.offset.x, 8.0f);
    EXPECT_FLOAT_EQ(collider.offset.y, 8.0f);
}

TEST(BoxColliderComponentTest, NonSquareCollider) {
    BoxColliderComponent collider(100, 50);
    
    EXPECT_EQ(collider.width, 100);
    EXPECT_EQ(collider.height, 50);
}

TEST(BoxColliderComponentTest, ZeroOffset) {
    BoxColliderComponent collider(32, 32, glm::vec2(0, 0));
    
    EXPECT_FLOAT_EQ(collider.offset.x, 0.0f);
    EXPECT_FLOAT_EQ(collider.offset.y, 0.0f);
}

// ========================================
// ProjectileComponent Tests
// ========================================
TEST(ProjectileComponentTest, DefaultConstructor) {
    ProjectileComponent projectile;
    
    // Apenas valida que o componente existe
    SUCCEED();
}

TEST(ProjectileComponentTest, FriendlyProjectile) {
    ProjectileComponent projectile(true, 25, 5000);
    
    EXPECT_TRUE(projectile.isFriendly);
    EXPECT_EQ(projectile.hitPercentDamage, 25);
    EXPECT_EQ(projectile.duration, 5000);
}

TEST(ProjectileComponentTest, EnemyProjectile) {
    ProjectileComponent projectile(false, 10, 3000);
    
    EXPECT_FALSE(projectile.isFriendly);
    EXPECT_EQ(projectile.hitPercentDamage, 10);
    EXPECT_EQ(projectile.duration, 3000);
}

TEST(ProjectileComponentTest, HighDamageProjectile) {
    ProjectileComponent projectile(true, 100, 1000);
    
    EXPECT_EQ(projectile.hitPercentDamage, 100);
    EXPECT_EQ(projectile.duration, 1000);
}
