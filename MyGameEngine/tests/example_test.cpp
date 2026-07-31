#include <gtest/gtest.h>
#include <glm/glm.hpp>
#include <box2d/box2d.h>

// Teste básico do Google Test
TEST(ExampleTest, BasicAssertions) {
    EXPECT_EQ(2 + 2, 4);
    EXPECT_TRUE(true);
    EXPECT_FALSE(false);
}

// Teste com GLM
TEST(GLMTest, VectorOperations) {
    glm::vec2 v1(3.0f, 4.0f);
    glm::vec2 v2(1.0f, 2.0f);
    
    glm::vec2 result = v1 + v2;
    
    EXPECT_FLOAT_EQ(result.x, 4.0f);
    EXPECT_FLOAT_EQ(result.y, 6.0f);
    
    float length = glm::length(v1);
    EXPECT_FLOAT_EQ(length, 5.0f);
}

// Teste com Box2D
TEST(Box2DTest, WorldCreation) {
    b2World world(b2Vec2(0.0f, -10.0f));
    
    // Verificar gravidade
    b2Vec2 gravity = world.GetGravity();
    EXPECT_FLOAT_EQ(gravity.x, 0.0f);
    EXPECT_FLOAT_EQ(gravity.y, -10.0f);
    
    // Criar um corpo
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.0f, 10.0f);
    
    b2Body* body = world.CreateBody(&bodyDef);
    ASSERT_NE(body, nullptr);
    
    b2Vec2 position = body->GetPosition();
    EXPECT_FLOAT_EQ(position.x, 0.0f);
    EXPECT_FLOAT_EQ(position.y, 10.0f);
}

// Teste de matemática simples
TEST(MathTest, BasicOperations) {
    int sum = 5 + 3;
    int product = 5 * 3;
    
    EXPECT_EQ(sum, 8);
    EXPECT_EQ(product, 15);
    EXPECT_GT(product, sum);
}
