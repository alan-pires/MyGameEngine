from conan import ConanFile

required_conan_version = ">=2.0"

class MyGameEngineConan(ConanFile):
    name = "MyGameEngine"
    version = "0.1"
    settings = "os", "arch", "compiler", "build_type"
    requires = [
        "sdl/2.30.5",
        "sdl_image/2.8.8",
        "sdl_ttf/2.24.0",
        "sdl_mixer/2.0.4",
        "lua/5.4.6",
        "glm/0.9.9.8",
        "sol2/3.2.3",
        "imgui/1.89.8",
        "box2d/2.4.1",
        "gtest/1.14.0"
    ]
    generators = "CMakeToolchain", "CMakeDeps"
    default_options = {
        "sdl/*:shared": False,
        "sdl_image/*:shared": False,
        "sdl_ttf/*:shared": False,
        "sdl_mixer/*:shared": False,
        "lua/*:shared": False,
        "imgui/*:shared": False,
        "box2d/*:shared": False,
        "gtest/*:shared": False
    }

    def layout(self):
        self.folders.build = "build"
