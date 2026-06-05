project "Flocking Bees The Game"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   targetdir "Binaries/%{cfg.buildcfg}"
   staticruntime "off"

   files { "Source/**.h", "Source/**.cpp" }

   includedirs
   {
      "../Engine/Vendor/glm",
      "../Engine/Vendor/glfw/include",
      "../Engine/Vendor/ImGUI",
      "%{IncludeDir.VulkanSDK}",
      
      "../Engine/Engine/Source",
   }

   links { "Engine" }

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

   filter "system:windows"
       systemversion "latest"
       defines { }

   filter "configurations:Debug"
       defines { "DEBUG" }
       runtime "Debug"
       symbols "On"

   filter "configurations:Release"
       defines { "RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "On"

   filter "configurations:Dist"
       kind "WindowedApp"
       defines { "DIST" }
       runtime "Release"
       optimize "On"
       symbols "Off"