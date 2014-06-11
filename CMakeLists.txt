cmake_minimum_required (VERSION 2.8)
project(ag)

add_definitions("-Wall")
add_definitions("-O3")
#add_definitions("-g")

add_definitions("-fexceptions")

#defines
#add_definitions(-DBOOST_ASIO_DYN_LINK)


#tests
add_executable(greedy greedy.cpp)
add_executable(faerbe backtracking.cpp)

if (CMAKE_CXX_COMPILER MATCHES ".*clang")
	add_definitions("-std=c++1y")
	add_definitions("-stdlib=libc++")
	target_link_libraries(greedy c++)
	target_link_libraries(faerbe c++)
else()
	if(CMAKE_COMPILER_IS_GNUCXX)
		execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion
               	 OUTPUT_VARIABLE GCC_VERSION)
		if (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7)
        		message("using gcc with -std=c++0x")
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")	
		else()
        		message("using gcc with -std=gnu++0x")
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")	
		endif()
    	elseif(MSVC)
        	message("using msvc.")
    	else()
        	message("using known compiler.")
    endif()

endif()



