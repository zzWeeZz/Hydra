workspace "Hydra_Engine"
    architecture "x64"
    startproject "Sandbox"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }
include "Dependencies.lua"
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Hydra"
include "Sandbox"
include "Hydra/vendor/SPIRV-Reflect"
include "Hydra/vendor/GLFW"
include "Hydra/vendor/glm"
