#include <gtest/gtest.h>
#include "../src/Logger/Logger.h"

// ========================================
// Logger Tests
// ========================================
class LoggerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Limpar mensagens antes de cada teste
        Logger::messages.clear();
    }
    
    void TearDown() override {
        // Limpar mensagens após cada teste
        Logger::messages.clear();
    }
};

TEST_F(LoggerTest, LogInfo) {
    Logger::Log("Test message");
    
    EXPECT_EQ(Logger::messages.size(), 1);
    EXPECT_EQ(Logger::messages[0].type, MGE_LOG_INFO);
    EXPECT_TRUE(Logger::messages[0].message.find("Test message") != std::string::npos);
}

TEST_F(LoggerTest, LogError) {
    Logger::Err("Error message");
    
    EXPECT_EQ(Logger::messages.size(), 1);
    EXPECT_EQ(Logger::messages[0].type, MGE_LOG_ERROR);
    EXPECT_TRUE(Logger::messages[0].message.find("Error message") != std::string::npos);
}

TEST_F(LoggerTest, MultipleMessages) {
    Logger::Log("Message 1");
    Logger::Log("Message 2");
    Logger::Err("Error 1");
    Logger::Log("Message 3");
    
    EXPECT_EQ(Logger::messages.size(), 4);
}

TEST_F(LoggerTest, MessageTypes) {
    Logger::Log("Info");
    Logger::Err("Error");
    
    EXPECT_EQ(Logger::messages[0].type, MGE_LOG_INFO);
    EXPECT_EQ(Logger::messages[1].type, MGE_LOG_ERROR);
}

TEST_F(LoggerTest, EmptyMessage) {
    Logger::Log("");
    
    EXPECT_EQ(Logger::messages.size(), 1);
}

TEST_F(LoggerTest, LongMessage) {
    std::string longMsg(1000, 'A');
    Logger::Log(longMsg);
    
    EXPECT_EQ(Logger::messages.size(), 1);
    EXPECT_TRUE(Logger::messages[0].message.find(longMsg) != std::string::npos);
}

TEST_F(LoggerTest, SpecialCharacters) {
    Logger::Log("Special chars: !@#$%^&*()[]{}");
    
    EXPECT_EQ(Logger::messages.size(), 1);
    EXPECT_TRUE(Logger::messages[0].message.find("Special chars") != std::string::npos);
}

TEST_F(LoggerTest, NumbersInMessage) {
    Logger::Log("Value: 12345");
    
    EXPECT_EQ(Logger::messages.size(), 1);
    EXPECT_TRUE(Logger::messages[0].message.find("12345") != std::string::npos);
}

TEST_F(LoggerTest, MessagesAccumulate) {
    size_t count = 0;
    
    for (int i = 0; i < 100; i++) {
        Logger::Log("Message " + std::to_string(i));
        count++;
    }
    
    EXPECT_EQ(Logger::messages.size(), count);
}

TEST_F(LoggerTest, MessagePersistence) {
    Logger::Log("Persistent message");
    
    EXPECT_EQ(Logger::messages.size(), 1);
    
    // Mensagem deve continuar disponível
    EXPECT_EQ(Logger::messages[0].type, MGE_LOG_INFO);
}

TEST_F(LoggerTest, MixedMessageTypes) {
    for (int i = 0; i < 10; i++) {
        if (i % 2 == 0) {
            Logger::Log("Info " + std::to_string(i));
        } else {
            Logger::Err("Error " + std::to_string(i));
        }
    }
    
    EXPECT_EQ(Logger::messages.size(), 10);
    
    int infoCount = 0;
    int errorCount = 0;
    
    for (const auto& msg : Logger::messages) {
        if (msg.type == MGE_LOG_INFO) infoCount++;
        if (msg.type == MGE_LOG_ERROR) errorCount++;
    }
    
    EXPECT_EQ(infoCount, 5);
    EXPECT_EQ(errorCount, 5);
}

// ========================================
// LogEntry Tests
// ========================================
TEST(LogEntryTest, LogEntryCreation) {
    LogEntry entry;
    entry.type = MGE_LOG_INFO;
    entry.message = "Test";
    
    EXPECT_EQ(entry.type, MGE_LOG_INFO);
    EXPECT_EQ(entry.message, "Test");
}

TEST(LogEntryTest, LogTypeEnum) {
    EXPECT_NE(MGE_LOG_INFO, MGE_LOG_WARNING);
    EXPECT_NE(MGE_LOG_WARNING, MGE_LOG_ERROR);
    EXPECT_NE(MGE_LOG_INFO, MGE_LOG_ERROR);
}

TEST(LogEntryTest, MessageAssignment) {
    LogEntry entry;
    
    entry.message = "First message";
    EXPECT_EQ(entry.message, "First message");
    
    entry.message = "Second message";
    EXPECT_EQ(entry.message, "Second message");
}

// ========================================
// Logger Integration Tests
// ========================================
TEST_F(LoggerTest, LoggerInGameContext) {
    Logger::Log("Game started");
    Logger::Log("Level loaded");
    Logger::Log("Player spawned");
    Logger::Err("Collision detected");
    Logger::Log("Game ended");
    
    EXPECT_EQ(Logger::messages.size(), 5);
    
    // Verificar ordem
    EXPECT_TRUE(Logger::messages[0].message.find("Game started") != std::string::npos);
    EXPECT_TRUE(Logger::messages[4].message.find("Game ended") != std::string::npos);
}

TEST_F(LoggerTest, LoggerWithNumbers) {
    Logger::Log("FPS: " + std::to_string(60));
    Logger::Log("Entities: " + std::to_string(150));
    Logger::Log("Score: " + std::to_string(9999));
    
    EXPECT_EQ(Logger::messages.size(), 3);
}
