# Testes do MyGameEngine

Este diretório contém os testes unitários do projeto usando **Google Test**.

## Estrutura

```
tests/
├── example_test.cpp         # Testes básicos de validação (4 testes)
├── component_tests.cpp      # Testes de componentes ECS (21 testes)
├── ecs_tests.cpp           # Testes da arquitetura ECS (22 testes)
├── logger_tests.cpp        # Testes do sistema de logging (16 testes)
├── math_physics_tests.cpp  # Testes de matemática e física (27 testes)
└── README.md               # Este arquivo
```

## Cobertura de Testes Atual

### 🎯 Testes Básicos (example_test.cpp)
- ✅ **Google Test**: Validação básica do framework
- ✅ **GLM**: Operações com vetores 2D
- ✅ **Box2D**: Criação de mundo físico com gravidade
- ✅ **Matemática**: Operações aritméticas básicas

### 🧩 Componentes (component_tests.cpp)
- ✅ **TransformComponent** (4 testes): posição, escala, rotação, velocidade
- ✅ **RigidBodyComponent** (3 testes): física e velocidade
- ✅ **HealthComponent** (6 testes): vida, dano, cura
- ✅ **BoxColliderComponent** (4 testes): colisões AABB
- ✅ **ProjectileComponent** (4 testes): projéteis e dano

### 🔧 ECS (ecs_tests.cpp)
- ✅ **Entity** (3 testes): criação e comparação
- ✅ **Component Management** (6 testes): adicionar, remover, modificar
- ✅ **Registry** (3 testes): gerenciamento de entidades
- ✅ **Tags & Groups** (5 testes): organização de entidades
- ✅ **Systems** (2 testes): sistemas com requisitos de componentes

### 📝 Logger (logger_tests.cpp)
- ✅ **Logger** (11 testes): mensagens info/erro, persistência
- ✅ **LogEntry** (3 testes): estrutura de entradas
- ✅ **Integration** (2 testes): uso em contexto de jogo

### 🔢 Math & Physics (math_physics_tests.cpp)
- ✅ **GLM Vectors** (10 testes): operações vetoriais
- ✅ **Physics** (5 testes): movimento, gravidade, fricção
- ✅ **Collision** (4 testes): AABB e círculos
- ✅ **Angles** (2 testes): conversão e rotação
- ✅ **Interpolation** (2 testes): lerp e mix
- ✅ **Clamping** (2 testes): limites de valores
- ✅ **Game Math** (2 testes): coordenadas e viewport

**Total**: **87 testes** (100% passando) ✅

## Como Executar os Testes

### Opção 1: Via CMake (Recomendado)

```powershell
# Configurar e compilar
cmake --build build --config Debug

# Executar todos os testes
ctest -C Debug --output-on-failure --test-dir build

# Ou executar o executável diretamente
.\build\bin\Debug\MyGameEngine_tests.exe
```

### Opção 2: Via Visual Studio

1. Compile o projeto
2. Vá em `Test > Run All Tests`
3. Os resultados aparecerão no Test Explorer

### Opção 3: Executar testes específicos

```powershell
# Executar apenas testes do GLM
.\build\bin\Debug\MyGameEngine_tests.exe --gtest_filter=GLMTest.*

# Executar apenas testes do Box2D
.\build\bin\Debug\MyGameEngine_tests.exe --gtest_filter=Box2DTest.*

# Listar todos os testes
.\build\bin\Debug\MyGameEngine_tests.exe --gtest_list_tests
```

## Resultado dos Testes

```
Test project C:/Users/Alan_/OneDrive/Desktop/ALL/projects/MyGameEngine/MyGameEngine/build
    Start 1: ExampleTest.BasicAssertions
1/4 Test #1: ExampleTest.BasicAssertions ......   Passed    0.01 sec
    Start 2: GLMTest.VectorOperations
2/4 Test #2: GLMTest.VectorOperations .........   Passed    0.02 sec
    Start 3: Box2DTest.WorldCreation
3/4 Test #3: Box2DTest.WorldCreation ..........   Passed    0.02 sec
    Start 4: MathTest.BasicOperations
4/4 Test #4: MathTest.BasicOperations .........   Passed    0.01 sec

100% tests passed, 0 tests failed out of 4

Total Test time (real) =   0.07 sec
```

## Como Criar Novos Testes

1. Crie um novo arquivo `.cpp` nesta pasta
2. Inclua o header do gtest:
```cpp
#include <gtest/gtest.h>
```

3. Escreva seus testes:
```cpp
TEST(NomeDoGrupo, NomeDoTeste) {
    // Seu código aqui
    EXPECT_EQ(2 + 2, 4);
    ASSERT_TRUE(alguma_condicao);
}
```

4. Para testar com fixtures (setup/teardown):
```cpp
class MinhaClasseTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Código executado antes de cada teste
    }
    
    void TearDown() override {
        // Código executado após cada teste
    }
};

TEST_F(MinhaClasseTest, MeuTeste) {
    // Usar fixture
}
```

## Macros Úteis do Google Test

### Asserções Básicas
- `EXPECT_EQ(a, b)` - Espera que a == b
- `EXPECT_NE(a, b)` - Espera que a != b
- `EXPECT_LT(a, b)` - Espera que a < b
- `EXPECT_LE(a, b)` - Espera que a <= b
- `EXPECT_GT(a, b)` - Espera que a > b
- `EXPECT_GE(a, b)` - Espera que a >= b

### Asserções Booleanas
- `EXPECT_TRUE(condition)` - Espera que seja true
- `EXPECT_FALSE(condition)` - Espera que seja false

### Asserções de Ponto Flutuante
- `EXPECT_FLOAT_EQ(a, b)` - Compara floats com tolerância
- `EXPECT_DOUBLE_EQ(a, b)` - Compara doubles com tolerância
- `EXPECT_NEAR(a, b, tolerance)` - Compara com tolerância customizada

### Asserções de String
- `EXPECT_STREQ(str1, str2)` - Strings iguais (C-strings)
- `EXPECT_STRNE(str1, str2)` - Strings diferentes

### Diferença entre EXPECT e ASSERT
- `EXPECT_*` - Continua o teste mesmo se falhar
- `ASSERT_*` - Para o teste imediatamente se falhar

## Exemplos de Testes Incluídos

### Teste do Google Test
```cpp
TEST(ExampleTest, BasicAssertions) {
    EXPECT_EQ(7 * 6, 42);
    EXPECT_TRUE(true);
    EXPECT_FALSE(false);
}
```

### Teste de Vetores GLM
```cpp
TEST(GLMTest, VectorOperations) {
    glm::vec2 v1(1.0f, 2.0f);
    glm::vec2 v2(3.0f, 4.0f);
    glm::vec2 result = v1 + v2;
    
    EXPECT_FLOAT_EQ(result.x, 4.0f);
    EXPECT_FLOAT_EQ(result.y, 6.0f);
}
```

### Teste de Física Box2D
```cpp
TEST(Box2DTest, WorldCreation) {
    b2Vec2 gravity(0.0f, 9.8f);
    b2World world(gravity);
    
    EXPECT_FLOAT_EQ(world.GetGravity().y, 9.8f);
}
```

### Teste Matemático
```cpp
TEST(MathTest, BasicOperations) {
    EXPECT_EQ(2 + 2, 4);
    EXPECT_EQ(10 - 5, 5);
    EXPECT_EQ(3 * 4, 12);
    EXPECT_EQ(15 / 3, 5);
}
```

## Integração Contínua

Os testes são executados automaticamente durante o build para garantir que nenhuma mudança quebre o código existente.

## Troubleshooting

### Erro: "Test executable not found"
- Certifique-se de ter compilado o projeto: `cmake --build build --config Debug`

### Erro: "gtest not found"
- Execute `conan install` para baixar as dependências:
```powershell
conan install . -of build --build=missing -s build_type=Debug -s compiler.cppstd=17 -pr default
```

### Testes falhando após mudanças
- Execute `cmake --build build --clean-first --config Debug` para recompilar tudo

### CMake não detecta novos testes
- Reconfigure o CMake:
```powershell
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake
```

## Boas Práticas

1. ✅ **Escreva testes antes de implementar** (TDD)
2. ✅ **Um teste deve testar apenas uma coisa**
3. ✅ **Nomes descritivos**: `TEST(ComponentName, SpecificBehavior)`
4. ✅ **Use fixtures** para setup/teardown repetitivo
5. ✅ **Testes devem ser independentes** (não dependem de ordem)
6. ✅ **Testes devem ser rápidos** (< 1 segundo cada)
7. ✅ **Limpe recursos** em TearDown()

## Próximos Passos

Para expandir a cobertura de testes, considere adicionar testes para:

- **Componentes**: TransformComponent, RigidBodyComponent, HealthComponent, etc.
- **ECS**: Entity, Registry, System, Pool
- **Eventos**: EventManager, CollisionEvent, KeyPressedEvent
- **Física**: Colisões, movimento, aceleração
- **Sistemas**: MovementSystem, CollisionSystem, RenderSystem
- **Game**: Inicialização, loop principal, carregamento de níveis

## Documentação Oficial

- Google Test: https://google.github.io/googletest/
- GLM: https://github.com/g-truc/glm
- Box2D: https://box2d.org/documentation/
