
sandbox_modules = {
    appframework_src = "../../projects/appframework/src",
    appframework_libs = "../../projects/appframework/libs",
}

-- must include vendors used by sandbox's modules
sandbox_modules_vendor = {
    glfw = "../../submodules/glfw/include",
    glm = "../../submodules/glm",
    imgui = "../../submodules/imgui",
    spdlog = "../../submodules/spdlog/include",
    openal_soft = "../../submodules/openal-soft/include",
    audiofile_adamstark = "../../submodules/audiofile"
}

project "sandbox"
    kind "ConsoleApp"
    language "C++"
    staticruntime "off"
    cppdialect "C++20"
    systemversion "latest"

    targetdir ("bin/builds/%{cfg.system}")
    objdir ("bin/intermediates/%{cfg.system}")

    files {
        "%{prj.location}/**.lua",
        "%{prj.location}/src/**.h",
        "%{prj.location}/src/**.cpp",
        "%{prj.location}/libs/**.h", 
        "%{prj.location}/libs/**.cpp",
    }

    includedirs {
        -- including this project's files
        "%{prj.location}/src",
        "%{prj.location}/libs",

        -- including sandbox's modules files
        "%{sandbox_modules.appframework_src}",
        "%{sandbox_modules.appframework_libs}",

        -- include submodules
        "%{sandbox_modules_vendor.glfw}",
        "%{sandbox_modules_vendor.glm}",
        "%{sandbox_modules_vendor.imgui}",
        "%{sandbox_modules_vendor.spdlog}",
        "%{sandbox_modules_vendor.openal_soft}",
        "%{sandbox_modules_vendor.audiofile_adamstark}"
    }

    links {
        "appframework"
    }

    defines {
        "GLFW_INCLUDE_NONE",
        "_CRT_SECURE_NO_WARNINGS"
    }

    postbuildcommands {
        '{MKDIR} "../../bin"',
        '{COPYFILE} "%{cfg.buildtarget.abspath}" "../../bin/%{cfg.buildtarget.name}"'
    }

    filter "system:windows" 
        defines { "WINDOWS" }   

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