

-- AppFramework Project

appf_modules = {
    glfw = "../../submodules/glfw/include",
    glm = "../../submodules/glm",
    imgui = "../../submodules/imgui",
    spdlog = "../../submodules/spdlog/include",
    openal_soft = "../../submodules/openal-soft/include",
    audiofile_adamstark = "../../submodules/audiofile"
}

include "../../submodules/glfw"
include "../../submodules/imgui"

project "appframework"
    kind "SharedLib"
    language "C++"
    cppdialect "C++20"
    systemversion "latest"

    targetdir ("bin/builds/%{cfg.system}")
    objdir ("bin/intermediates/%{cfg.system}")

    pchheader "afpch.h"
    pchsource "%{prj.location}/src/afpch.cpp"

    defines {
        "GLFW_INCLUDE_NONE",
        "_CRT_SECURE_NO_WARNINGS"
    }

    postbuildcommands {
        '{MKDIR} "../../bin"',
        '{COPYFILE} "%{cfg.buildtarget.abspath}" "../../bin/%{cfg.buildtarget.name}"',
        '{COPYFILE} "%{cfg.buildtarget.abspath}" "../sandbox/bin/builds/windows/%{cfg.buildtarget.name}"',
    }

    files {
        "%{prj.location}/**.lua",
        "%{prj.location}/src/**.h",
        "%{prj.location}/src/**.cpp",
        "%{prj.location}/libs/**.h",
        "%{prj.location}/libs/**.cpp",
    }

    includedirs {
        "%{prj.location}/src",
        "%{prj.location}/libs",
        "%{appf_modules.glfw}", -- compile
        "%{appf_modules.glm}", -- header-only
        "%{appf_modules.imgui}", -- compile
        "%{appf_modules.spdlog}", -- header-only
        "%{appf_modules.openal_soft}", -- compile/header-only
        "%{appf_modules.audiofile_adamstark}" -- single-header-compile
    }

    links {
        "glfw",
        "imgui",
        "opengl32.lib"
    }

    filter "configurations:Debug"
        links {
            "../../submodules/openal-soft/binaries/openal_debug.lib"
        }

    filter "configurations:Release"
        links {
            "../../submodules/openal-soft/binaries/openal_release.lib"
        }

    filter "system:windows" 
        defines { "WINDOWS", "BUILD_DLL" }   

    filter "system:linux"
        defines { "LINUX" }

    filter "system:macosx"
        defines { "MACOSX" }

    filter "configurations:Debug"
        runtime "Debug"
        optimize "Off"
        symbols "On"
        buildoptions { "/utf-8"}
        defines {
            "__DEBUG__BUILD__"
        }   

    filter "configurations:Release"
        runtime "Release"
        optimize "On"
        symbols "Off"
        buildoptions { "/utf-8"}
        defines {
            "__RELEASE__BUILD__"
        }   