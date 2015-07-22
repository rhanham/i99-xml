----------------------------------
--          VARIABLES           --
----------------------------------
heapOptionName = "memheap-size"
blockOptionName = "memblock-size"

----------------------------------
--          FUNCTIONS           --
----------------------------------
function isPowOf2(n)

	--Keep doubling counter until it's no longer less than n (i.e equal or greater than).
	local counter = 1
	while(counter < n) do
			counter = counter * 2
	end
			
	return counter == n --If n is equal to counter, n must be a power of 2.
	
end

function i99_MemPoolSize()

	--Set the default memory sizes.
	if not _OPTIONS[heapOptionName] then
		return 64 * 1024 * 1024 -- 64MiB default.
	else
		return tonumber(_OPTIONS[heapOptionName])
	end
end

function i99_MemBlockSize()
	if not _OPTIONS[blockOptionName] then
		return 64 * 1024 -- 64KiB default.
	else
		return tonumber(_OPTIONS[blockOptionName])
	end
end

function i99_xmlOptions()
	--Defines the options.
	newoption { 
		trigger = heapOptionName,
		description = "The size of the plugins memory pool",
		value = "BYTES"
	}

	newoption { 
		trigger = blockOptionName,
		description = "The size of each allocation block.",
		value = "BYTES"
	}
	
end

function i99_xmlValidate()

	local result = true
	

	--Ensure mempool-size is a number if it has been defined.
	if _OPTIONS[heapOptionName] and tonumber(_OPTIONS[heapOptionName]) == nil then
		printf("Error: %s is not a number.", heapOptionName)
		result = false
	end
	
	--Ensure memblock-size is a number if it has been defined.
	if _OPTIONS[blockOptionName] and tonumber(_OPTIONS[blockOptionName]) == nil then
		printf("Error: %s is not a number.", blockOptionName)
		result = false
	end
	
	if i99_MemBlockSize() > i99_MemPoolSize() then
		printf("Error: %s larger than %s.", blockOptionName, heapOptionName)
		result = false
	end
	
	return result;	
end




function i99_xmlConfig()	
	
	--Define the operating system the plugin is built for.
	configuration { "linux" }
		defines { "LINUX" }
	configuration { "windows" }
		defines { "WIN32" }
	configuration { "bsd" }
		defines { "FREEBSD" }
	configuration {}

	--Add the C++11 flag.
	configuration { "linux", "gcc" }
		buildoptions { "--std=c++0x" }
	configuration { "windows", "gcc" }
		buildoptions { "--std=c++0x" }
	configuration {}
end
