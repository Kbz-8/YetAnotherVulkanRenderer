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

add_requires("vulkan-headers", "libsdl")
add_requires("glm", { configs = { header_only = true }})

target("yavr")
	set_kind("binary")
	add_includedirs("Runtime/Includes")
	add_files("Runtime/Sources/**.cpp")
	add_packages("libsdl", "vulkan-headers", "glm")
	set_pcxxheader("Runtime/Includes/PreCompiled.h")

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
	end)

target_end() -- optional but I think the code is cleaner with this
