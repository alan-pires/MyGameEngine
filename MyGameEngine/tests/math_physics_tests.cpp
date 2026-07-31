#include <gtest/gtest.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>

// ========================================
// GLM Vector Math Tests
// ========================================
TEST(MathTest, Vec2Creation) {
    glm::vec2 v(10.0f, 20.0f);
    
    EXPECT_FLOAT_EQ(v.x, 10.0f);
    EXPECT_FLOAT_EQ(v.y, 20.0f);
}

TEST(MathTest, Vec2Addition) {
    glm::vec2 v1(5.0f, 10.0f);
    glm::vec2 v2(3.0f, 7.0f);
    
    glm::vec2 result = v1 + v2;
    
    EXPECT_FLOAT_EQ(result.x, 8.0f);
    EXPECT_FLOAT_EQ(result.y, 17.0f);
}

TEST(MathTest, Vec2Subtraction) {
    glm::vec2 v1(10.0f, 20.0f);
    glm::vec2 v2(3.0f, 5.0f);
    
    glm::vec2 result = v1 - v2;
    
    EXPECT_FLOAT_EQ(result.x, 7.0f);
    EXPECT_FLOAT_EQ(result.y, 15.0f);
}

TEST(MathTest, Vec2Multiplication) {
    glm::vec2 v(5.0f, 10.0f);
    float scalar = 3.0f;
    
    glm::vec2 result = v * scalar;
    
    EXPECT_FLOAT_EQ(result.x, 15.0f);
    EXPECT_FLOAT_EQ(result.y, 30.0f);
}

TEST(MathTest, Vec2Division) {
    glm::vec2 v(20.0f, 40.0f);
    float scalar = 4.0f;
    
    glm::vec2 result = v / scalar;
    
    EXPECT_FLOAT_EQ(result.x, 5.0f);
    EXPECT_FLOAT_EQ(result.y, 10.0f);
}

TEST(MathTest, Vec2Length) {
    glm::vec2 v(3.0f, 4.0f);
    
    float length = glm::length(v);
    
    EXPECT_FLOAT_EQ(length, 5.0f);
}

TEST(MathTest, Vec2Normalize) {
    glm::vec2 v(3.0f, 4.0f);
    
    glm::vec2 normalized = glm::normalize(v);
    
    EXPECT_FLOAT_EQ(normalized.x, 0.6f);
    EXPECT_FLOAT_EQ(normalized.y, 0.8f);
    EXPECT_NEAR(glm::length(normalized), 1.0f, 0.0001f);
}

TEST(MathTest, Vec2DotProduct) {
    glm::vec2 v1(2.0f, 3.0f);
    glm::vec2 v2(4.0f, 5.0f);
    
    float dot = glm::dot(v1, v2);
    
    EXPECT_FLOAT_EQ(dot, 23.0f); // (2*4 + 3*5 = 8 + 15 = 23)
}

TEST(MathTest, Vec2Distance) {
    glm::vec2 v1(0.0f, 0.0f);
    glm::vec2 v2(3.0f, 4.0f);
    
    float distance = glm::distance(v1, v2);
    
    EXPECT_FLOAT_EQ(distance, 5.0f);
}

TEST(MathTest, Vec2ZeroVector) {
    glm::vec2 zero(0.0f, 0.0f);
    
    EXPECT_FLOAT_EQ(zero.x, 0.0f);
    EXPECT_FLOAT_EQ(zero.y, 0.0f);
    EXPECT_FLOAT_EQ(glm::length(zero), 0.0f);
}

// ========================================
// Movement Physics Tests
// ========================================
TEST(PhysicsTest, BasicMovement) {
    glm::vec2 position(0.0f, 0.0f);
    glm::vec2 velocity(10.0f, 5.0f);
    float deltaTime = 1.0f;
    
    position += velocity * deltaTime;
    
    EXPECT_FLOAT_EQ(position.x, 10.0f);
    EXPECT_FLOAT_EQ(position.y, 5.0f);
}

TEST(PhysicsTest, Acceleration) {
    glm::vec2 velocity(0.0f, 0.0f);
    glm::vec2 acceleration(5.0f, 0.0f);
    float deltaTime = 2.0f;
    
    velocity += acceleration * deltaTime;
    
    EXPECT_FLOAT_EQ(velocity.x, 10.0f);
    EXPECT_FLOAT_EQ(velocity.y, 0.0f);
}

TEST(PhysicsTest, Friction) {
    glm::vec2 velocity(100.0f, 0.0f);
    float friction = 0.9f;
    
    velocity *= friction;
    
    EXPECT_FLOAT_EQ(velocity.x, 90.0f);
}

TEST(PhysicsTest, Gravity) {
    glm::vec2 velocity(0.0f, 0.0f);
    glm::vec2 gravity(0.0f, 9.8f);
    float deltaTime = 1.0f;
    
    velocity += gravity * deltaTime;
    
    EXPECT_FLOAT_EQ(velocity.y, 9.8f);
}

TEST(PhysicsTest, TerminalVelocity) {
    glm::vec2 velocity(0.0f, 200.0f);
    float maxVelocity = 150.0f;
    
    if (glm::length(velocity) > maxVelocity) {
        velocity = glm::normalize(velocity) * maxVelocity;
    }
    
    EXPECT_FLOAT_EQ(glm::length(velocity), maxVelocity);
}

// ========================================
// Collision Detection Tests
// ========================================
TEST(CollisionTest, AABBCollision_Overlap) {
    // Box 1
    glm::vec2 pos1(0.0f, 0.0f);
    float width1 = 32.0f;
    float height1 = 32.0f;
    
    // Box 2 (sobrepondo)
    glm::vec2 pos2(16.0f, 16.0f);
    float width2 = 32.0f;
    float height2 = 32.0f;
    
    bool collision = (pos1.x < pos2.x + width2 &&
                     pos1.x + width1 > pos2.x &&
                     pos1.y < pos2.y + height2 &&
                     pos1.y + height1 > pos2.y);
    
    EXPECT_TRUE(collision);
}

TEST(CollisionTest, AABBCollision_NoOverlap) {
    // Box 1
    glm::vec2 pos1(0.0f, 0.0f);
    float width1 = 32.0f;
    float height1 = 32.0f;
    
    // Box 2 (distante)
    glm::vec2 pos2(100.0f, 100.0f);
    float width2 = 32.0f;
    float height2 = 32.0f;
    
    bool collision = (pos1.x < pos2.x + width2 &&
                     pos1.x + width1 > pos2.x &&
                     pos1.y < pos2.y + height2 &&
                     pos1.y + height1 > pos2.y);
    
    EXPECT_FALSE(collision);
}

TEST(CollisionTest, CircleCollision_Overlap) {
    glm::vec2 center1(0.0f, 0.0f);
    float radius1 = 10.0f;
    
    glm::vec2 center2(15.0f, 0.0f);
    float radius2 = 10.0f;
    
    float distance = glm::distance(center1, center2);
    bool collision = distance < (radius1 + radius2);
    
    EXPECT_TRUE(collision);
}

TEST(CollisionTest, CircleCollision_NoOverlap) {
    glm::vec2 center1(0.0f, 0.0f);
    float radius1 = 10.0f;
    
    glm::vec2 center2(100.0f, 0.0f);
    float radius2 = 10.0f;
    
    float distance = glm::distance(center1, center2);
    bool collision = distance < (radius1 + radius2);
    
    EXPECT_FALSE(collision);
}

// ========================================
// Angle and Rotation Tests
// ========================================
TEST(MathTest, AngleConversion) {
    float degrees = 90.0f;
    float radians = glm::radians(degrees);
    
    EXPECT_NEAR(radians, glm::half_pi<float>(), 0.001f);
}

TEST(MathTest, RotationMatrix) {
    glm::vec2 point(1.0f, 0.0f);
    float angle = glm::radians(90.0f);
    
    // Rotação manual
    float cosA = std::cos(angle);
    float sinA = std::sin(angle);
    
    glm::vec2 rotated(
        point.x * cosA - point.y * sinA,
        point.x * sinA + point.y * cosA
    );
    
    EXPECT_NEAR(rotated.x, 0.0f, 0.001f);
    EXPECT_NEAR(rotated.y, 1.0f, 0.001f);
}

// ========================================
// Interpolation Tests
// ========================================
TEST(MathTest, LinearInterpolation) {
    float start = 0.0f;
    float end = 100.0f;
    float t = 0.5f;
    
    float result = start + t * (end - start);
    
    EXPECT_FLOAT_EQ(result, 50.0f);
}

TEST(MathTest, Vec2Lerp) {
    glm::vec2 start(0.0f, 0.0f);
    glm::vec2 end(100.0f, 100.0f);
    float t = 0.25f;
    
    glm::vec2 result = glm::mix(start, end, t);
    
    EXPECT_FLOAT_EQ(result.x, 25.0f);
    EXPECT_FLOAT_EQ(result.y, 25.0f);
}

// ========================================
// Clamping Tests
// ========================================
TEST(MathTest, ClampValue) {
    float value = 150.0f;
    float min = 0.0f;
    float max = 100.0f;
    
    float clamped = glm::clamp(value, min, max);
    
    EXPECT_FLOAT_EQ(clamped, 100.0f);
}

TEST(MathTest, ClampVector) {
    glm::vec2 velocity(200.0f, -150.0f);
    float maxSpeed = 100.0f;
    
    if (glm::length(velocity) > maxSpeed) {
        velocity = glm::normalize(velocity) * maxSpeed;
    }
    
    EXPECT_NEAR(glm::length(velocity), maxSpeed, 0.001f);
}

// ========================================
// Game-specific Math Tests
// ========================================
TEST(GameMathTest, ScreenToWorld) {
    glm::vec2 screenPos(400, 300);
    glm::vec2 cameraOffset(100, 50);
    
    glm::vec2 worldPos = screenPos + cameraOffset;
    
    EXPECT_FLOAT_EQ(worldPos.x, 500.0f);
    EXPECT_FLOAT_EQ(worldPos.y, 350.0f);
}

TEST(GameMathTest, IsInView) {
    glm::vec2 entityPos(500, 300);
    glm::vec2 cameraPos(400, 200);
    float viewWidth = 800.0f;
    float viewHeight = 600.0f;
    
    bool inView = (entityPos.x >= cameraPos.x && 
                  entityPos.x <= cameraPos.x + viewWidth &&
                  entityPos.y >= cameraPos.y && 
                  entityPos.y <= cameraPos.y + viewHeight);
    
    EXPECT_TRUE(inView);
}
