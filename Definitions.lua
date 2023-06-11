outputdir = "%{cfg.buildcfg}_%{cfg.architecture}"

Includes = {}
Includes["Zeus"] = "%{wks.location}/Zeus"
Includes["GLFW"] = "%{wks.location}/Zeus/vendor/GLFW/GLFW/include"
Includes["imgui"] = "%{wks.location}/Zeus/vendor/imgui"
Includes["glm"] = "%{wks.location}/Zeus/vendor/glm/include"
Includes["stb_image"] = "%{wks.location}/Zeus/vendor/stb_image/include"
Includes["Zerializer"] = "%{wks.location}/Zerializer/include"
Includes["Phyzics"] = "%{wks.location}/Phyzics/src/include"
