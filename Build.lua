workspace "Game"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "Game"

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"
IncludeDir = {}

VULKAN_SDK = os.getenv("VULKAN_SDK")
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"
IncludeDir["glm"] = "../Engine/Vendor/glm"
IncludeDir["glfw"] = "../Engine/Vendor/glfw/include"
IncludeDir["ImGUI"] = "../Engine/Vendor/ImGUI"

LibraryDir = {}
LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"

Library = {}
Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"

include "Engine/Engine/Build-Engine.lua"
include "Game/Build-Game.lua"