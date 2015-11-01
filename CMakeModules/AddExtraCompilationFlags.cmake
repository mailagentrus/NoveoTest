MACRO(ADD_EXTRA_COMPILATION_FLAGS)
    include(CheckCXXCompilerFlag)
    if (CMAKE_COMPILER_IS_GNUCXX OR MINGW) #Not only UNIX but also WIN32 for MinGW

        set(WARNING_ALL "-Wall")
        CHECK_CXX_COMPILER_FLAG(${WARNING_ALL} WARNING_ALL_ALLOWED)
        if (WARNING_ALL_ALLOWED)
            MESSAGE("Compiler flag ${WARNING_ALL} allowed")
            set(ACTIVATE_WARNING_ALL "ON" CACHE BOOL "activate -Wall flag")
        else()
            MESSAGE("Compiler flag ${WARNING_ALL} not allowed")
        endif (WARNING_ALL_ALLOWED)

        set(WARNING_EXTRA "-Wextra")
        CHECK_CXX_COMPILER_FLAG(${WARNING_EXTRA} WARNING_EXTRA_ALLOWED)
        if (WARNING_EXTRA_ALLOWED)
            MESSAGE("Compiler flag ${WARNING_EXTRA} allowed")
            set(ACTIVATE_WARNING_EXTRA "ON" CACHE BOOL "activate -Wextra flag")
        else()
            MESSAGE("Compiler flag ${WARNING_EXTRA} not allowed")
        endif (WARNING_EXTRA_ALLOWED)

        set(WARNING_STRICT_OVERFLOW "-Wstrict-overflow=5")
        CHECK_CXX_COMPILER_FLAG(${WARNING_STRICT_OVERFLOW} WARNING_STRICT_OVERFLOW_ALLOWED)
        if (WARNING_STRICT_OVERFLOW_ALLOWED)
            MESSAGE("Compiler flag ${WARNING_STRICT_OVERFLOW} allowed")
            set(ACTIVATE_WARNING_STRICT_OVERFLOW "OFF" CACHE STRING "activate -Wstrict-overflow=5")
        else()
            MESSAGE("Compiler flag ${WARNING_STRICT_OVERFLOW} not allowed")
        endif (WARNING_STRICT_OVERFLOW_ALLOWED)

        set(WARNING_FLOAT_EQUAL "-Wfloat-equal")
        CHECK_CXX_COMPILER_FLAG(${WARNING_FLOAT_EQUAL} WARNING_FLOAT_EQUAL_ALLOWED)
        if (WARNING_FLOAT_EQUAL_ALLOWED)
            MESSAGE("Compiler flag ${WARNING_FLOAT_EQUAL} allowed")
            set(ACTIVATE_WARNING_FLOAT_EQUAL "OFF" CACHE BOOL "activate -Wfloat-equal flag")
        else()
            MESSAGE("Compiler flag ${WARNING_FLOAT_EQUAL} not allowed")
        endif (WARNING_FLOAT_EQUAL_ALLOWED)

        set(WARNING_SIGN_CONVERSION "-Wsign-conversion")
        CHECK_CXX_COMPILER_FLAG(${WARNING_SIGN_CONVERSION} WARNING_SIGN_CONVERSION_ALLOWED)
        if (WARNING_SIGN_CONVERSION_ALLOWED)
            MESSAGE("Compiler flag ${WARNING_SIGN_CONVERSION} allowed")
            set(ACTIVATE_WARNING_SIGN_CONVERSION "OFF" CACHE BOOL "activate -Wsign-conversion flag")
        else()
            MESSAGE("Compiler flag ${WARNING_SIGN_CONVERSION} not allowed")
        endif (WARNING_SIGN_CONVERSION_ALLOWED)

        set(WARNING_FPERMISSIVE "-fpermissive")
        CHECK_CXX_COMPILER_FLAG(${WARNING_FPERMISSIVE} WARNING_FPERMISSIVE_ALLOWED)
        if (WARNING_FPERMISSIVE_ALLOWED)
            MESSAGE("Compiler flag ${WARNING_FPERMISSIVE_ALLOWED} allowed")
            set(ACTIVATE_WARNING_FPERMISSIVE "ON" CACHE BOOL "activate -fpermissive flag")
        else()
            MESSAGE("Compiler flag ${WARNING_FPERMISSIVE_ALLOWED} not allowed")
        endif (WARNING_FPERMISSIVE_ALLOWED)

        if (ACTIVATE_WARNING_ALL)
            list(APPEND CMAKE_CXX_FLAGS ${WARNING_ALL})
        else()
            string(REPLACE ${WARNING_ALL} "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
        endif (ACTIVATE_WARNING_ALL)

        if (ACTIVATE_WARNING_EXTRA)
            list(APPEND CMAKE_CXX_FLAGS ${WARNING_EXTRA})
        else()
            string(REPLACE ${WARNING_EXTRA} "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
        endif (ACTIVATE_WARNING_EXTRA)

        if (ACTIVATE_WARNING_STRICT_OVERFLOW)
            list(APPEND CMAKE_CXX_FLAGS ${WARNING_STRICT_OVERFLOW})
            list(APPEND CMAKE_C_FLAGS ${WARNING_STRICT_OVERFLOW})
        else()
            string(REPLACE ${WARNING_STRICT_OVERFLOW} "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
            string(REPLACE ${WARNING_STRICT_OVERFLOW} "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
        endif (ACTIVATE_WARNING_STRICT_OVERFLOW)

        if (ACTIVATE_WARNING_FLOAT_EQUAL)
            list(APPEND CMAKE_CXX_FLAGS ${WARNING_FLOAT_EQUAL})
        else()
            string(REPLACE ${WARNING_FLOAT_EQUAL} "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
        endif (ACTIVATE_WARNING_FLOAT_EQUAL)

        if (ACTIVATE_WARNING_SIGN_CONVERSION)
            list(APPEND CMAKE_CXX_FLAGS ${WARNING_SIGN_CONVERSION})
        else()
            string(REPLACE ${WARNING_SIGN_CONVERSION} "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
        endif (ACTIVATE_WARNING_SIGN_CONVERSION)

        if (ACTIVATE_WARNING_FPERMISSIVE)
            list(APPEND CMAKE_CXX_FLAGS ${WARNING_FPERMISSIVE})
        else()
            string(REPLACE ${WARNING_FPERMISSIVE} "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
        endif (ACTIVATE_WARNING_FPERMISSIVE)

        #Set c++11 by default
        list(APPEND CMAKE_CXX_FLAGS "-std=c++11")

        #Set c99 by default
        list(APPEND CMAKE_C_FLAGS "-std=c99")

    elseif(MSVC)
        # Add specific compilation flags for Windows Visual

        set(WARNING_ALL "/Wall")
        CHECK_CXX_COMPILER_FLAG(${WARNING_ALL} WARNING_ALL_ALLOWED)
        if (WARNING_ALL_ALLOWED)
            MESSAGE("Compiler flag ${WARNING_ALL} allowed")
            set(ACTIVATE_WARNING_ALL "OFF" CACHE BOOL "activate /Wall flag")
        else()
            MESSAGE("Compiler flag ${WARNING_ALL} not allowed")
        endif (WARNING_ALL_ALLOWED)

        if (ACTIVATE_WARNING_ALL)
            list(APPEND CMAKE_CXX_FLAGS ${WARNING_ALL})
        else()
            string(REPLACE ${WARNING_ALL} "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
        endif ()
        if (MSVC80 OR MSVC90 OR MSVC10 OR MSVC11)
            # To avoid compiler warning (level 4) C4571, compile with /EHa if you still want
            # your catch(...) blocks to catch structured exceptions.
            list(APPEND CMAKE_CXX_FLAGS "/EHa")
        endif (MSVC80 OR MSVC90 OR MSVC10 OR MSVC11)

        set(MULTITHREADED_COMPILATION "/MP")
        MESSAGE("Compiler flag ${MULTITHREADED_COMPILATION} allowed")
        set(ACTIVATE_MULTITHREADED_COMPILATION "ON" CACHE BOOL "activate /MP flag")

        if (ACTIVATE_MULTITHREADED_COMPILATION)
            list(APPEND CMAKE_CXX_FLAGS ${MULTITHREADED_COMPILATION})
        else()
            string(REPLACE ${MULTITHREADED_COMPILATION} "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
        endif (ACTIVATE_MULTITHREADED_COMPILATION)

        #For exceptions
        list(APPEND CMAKE_CXX_FLAGS "/EHsc")

        #Set c99 by default
        list(APPEND CMAKE_C_FLAGS "/std=c99")
    endif ()

    # If compiler support symbol visibility, enable it.
    include(CheckCCompilerFlag)
    check_c_compiler_flag(-fvisibility=hidden HAS_VISIBILITY)
    if (HAS_VISIBILITY)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")
    endif (HAS_VISIBILITY)

    # Remove duplicates compilation flags
    separate_arguments(CMAKE_CXX_FLAGS)
    list(REMOVE_DUPLICATES CMAKE_CXX_FLAGS)
    string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" CACHE STRING "common C++ build flags" FORCE)
    separate_arguments(CMAKE_C_FLAGS)
    list(REMOVE_DUPLICATES CMAKE_C_FLAGS)
    string(REPLACE ";" " " CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" CACHE STRING "common C build flags" FORCE)

    if (MSVC)
        # Replace /MT to /MD flag
        FOREACH (flag_var CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
                CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
            STRING(REGEX REPLACE "/MT" "/MD" ${flag_var} "${${flag_var}}")
        ENDFOREACH (flag_var)

        FOREACH (flag_var CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
                CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)
            STRING(REGEX REPLACE "/MT" "/MD" ${flag_var} "${${flag_var}}")
        ENDFOREACH (flag_var)
    endif ()

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" CACHE STRING "Updated flags" FORCE)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}" CACHE STRING "Updated flags" FORCE)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}" CACHE STRING "Updated flags" FORCE)
    set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL}" CACHE STRING "Updated flags" FORCE)
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}" CACHE STRING "Updated flags" FORCE)

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" CACHE STRING "Updated flags" FORCE)
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}" CACHE STRING "Updated flags" FORCE)
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}" CACHE STRING "Updated flags" FORCE)
    set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL}" CACHE STRING "Updated flags" FORCE)
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO}" CACHE STRING "Updated flags" FORCE)

ENDMACRO(ADD_EXTRA_COMPILATION_FLAGS)
