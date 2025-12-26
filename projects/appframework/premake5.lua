

-- AppFramework Project

appf_modules = {
    glfw = "../../submodules/glfw/include",
    glm = "../../submodules/glm",
    imgui = "../../submodules/imgui"
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
        "%{appf_modules.glfw}",
        "%{appf_modules.glm}",
        "%{appf_modules.imgui}"
    }

    links {
        "glfw",
        "imgui",
        "opengl32.lib"
    }

    defines {
        "GLFW_INCLUDE_NONE"
    }

    postbuildcommands {
        '{MKDIR} "../../bin"',
        '{COPYFILE} "%{cfg.buildtarget.abspath}" "../../bin/%{cfg.buildtarget.name}"',
        '{COPYFILE} "%{cfg.buildtarget.abspath}" "../sandbox/bin/builds/windows/%{cfg.buildtarget.name}"',
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
        defines {
            "__DEBUG__BUILD__"
        }   

    filter "configurations:Release"
        runtime "Release"
        optimize "On"
        symbols "Off"
        defines {
            "__RELEASE__BUILD__"
        }   