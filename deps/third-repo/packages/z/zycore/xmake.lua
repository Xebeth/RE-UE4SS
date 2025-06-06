package("zycore")
    add_urls("git@github.com:zyantific/zycore-c.git")
    add_urls("https://github.com/zyantific/zycore-c.git")
    
    add_versions("v1.5.1", "53f6d2730d29b7d6399154501bc65c1c89f0b0fd")

    add_deps("cmake")

    add_defines("ZYCORE_STATIC_BUILD", { public = true })

    on_install(function (package)
        local configs = {}
        table.insert(configs, "-DCMAKE_BUILD_TYPE=" .. (package:debug() and "Debug" or "Release"))
        table.insert(configs, "-DZYCORE_BUILD_SHARED_LIB=OFF")
        import("package.tools.cmake").install(package, configs)
    end)

    on_test(function (package)
        assert(package:check_cxxsnippets({test = [[
            #include <Zycore/Comparison.h>
            #include <Zycore/Vector.h>
            void test() {
                ZyanVector vector;
                ZyanU16 buffer[32];
            }
        ]]}))
    end)
