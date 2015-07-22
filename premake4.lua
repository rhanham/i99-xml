dofile "script/i99premake4.lua"


if _ACTION == "clean" then
	os.rmdir("bin")
	os.rmdir("build")
end

i99_xmlOptions() --Defines the options
i99_xmlValidate() -- Validate inputs.

solution "i99-xml"
	configurations { "Debug", "Release" }
	location "build"
	targetdir "bin"
	language "C++"

	includedirs { "lib" }

	--Begin configuration	
	configuration "Debug"
		defines { "DEBUG" }
		flags { "Symbols" }
	configuration "Release"
		defines { "NDEBUG" }
		flags { "Optimize" }
	configuration{}
	--End configuration

	--Define the plugin project
	project "i99-xml"
		kind "SharedLib"
		targetname "i99_xml"
		targetprefix "" --Eliminates the 'lib' prefix.
		files { "src/*.cpp", "src/*.hpp", "lib/sdk/*.cpp" }	
		
		--Configure the project.
		i99_xmlConfig()
		
		--Define the pool and block sizes.
		defines { "I99XML_HEAP_SIZE=" .. i99_MemPoolSize() }
		defines { "I99XML_BLOCK_SIZE=" .. i99_MemBlockSize() }
