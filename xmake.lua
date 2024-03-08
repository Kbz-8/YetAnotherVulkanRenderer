set_languages("cxx17")

add_rules("mode.debug", "mode.release")
set_allowedplats("windows", "mingw", "linux", "macosx")
set_allowedmodes("debug", "release")
set_defaultmode("release")

set_project("PipelinesPioneer")

if is_mode("debug") then
	add_defines("YAVR_DEBUG")
elseif is_mode("release") then
	add_defines("YAVR_RELEASE")
	set_fpmodels("fast")
	add_vectorexts("sse", "sse2", "sse3", "ssse3")
end

if is_plat("windows", "mingw") then
	add_defines("VK_USE_PLATFORM_WIN32_KHR")
elseif is_plat("linux") then
	add_defines("VK_USE_PLATFORM_XLIB_KHR")
	-- add_defines("VK_USE_PLATFORM_WAYLAND_KHR")
elseif is_plat("macosx") then
	add_defines("VK_USE_PLATFORM_METAL_EXT")
end

add_requires("vulkan-headers", "libsdl")
add_requires("glm", { configs = { header_only = true }})

target("yavr")
	set_kind("binary")
	add_includedirs("Runtime/Includes")
	add_files("Runtime/Sources/**.cpp")
	add_packages("libsdl", "vulkan-headers", "glm")
	set_pcxxheader("Runtime/Includes/PreCompiled.h")

	add_defines("VERTEX_SHADER_FILE=\"$(projectdir)/Shaders/Build/Vertex.spv\"")
	add_defines("FRAGMENT_SHADER_FILE=\"$(projectdir)/Shaders/Build/Fragment.spv\"")
	add_defines("FRACTAL_COMPUTE_SHADER_FILE=\"$(projectdir)/Shaders/Build/FractalCompute.spv\"")

	add_links("vulkan")

	on_load(function(target)
		os.run("sh ./Scripts/CompileShaders.sh %s", os.workingdir())
	end)

	on_clean(function(target)
		if target:objectfiles() then
			for _, file in ipairs(target:objectfiles()) do
				if os.exists(file) then
					print("Removing " .. file)
					os.rm(file)
				end
			end
		end
		if target:targetfile() and os.exists(target:targetfile()) then
			print("Removing " .. target:targetfile())
			os.rm(target:targetfile())
		end
		if os.exists(os.workingdir() .. "/Shaders/Build/") then
			print("Removing shaders build dir")
			os.rm(os.workingdir() .. "/Shaders/Build")
		end
	end)
target_end() -- optional but I think the code is cleaner with this
