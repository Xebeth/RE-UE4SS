#define NOMINMAX

#include <filesystem>
#include <Windows.h>

#include <DynamicOutput/DynamicOutput.hpp>
#include <Helpers/Win32Error.hpp>
#include <Helpers/String.hpp>
#include <Mod/CppMod.hpp>

namespace RC
{
    CppMod::CppMod(UE4SSProgram& program, StringType&& mod_name, StringType&& mod_path) : Mod(program, std::move(mod_name), std::move(mod_path))
    {
        m_dlls_path = m_mod_path / STR("dlls");

        if (!std::filesystem::exists(m_dlls_path))
        {
            m_last_error = std::format(STR("Could not find the dlls folder for mod {}: {}\n"), m_mod_name, to_wstring(m_dlls_path.string()));
            Output::send<LogLevel::Warning>(m_last_error);
            set_installable(false);
        }
        else
        {
            load_dll();
        }
    }

    auto CppMod::start_mod() -> void
    {
        try
        {
            m_mod = m_start_mod_func();
            m_is_started = m_mod != nullptr;
        }
        catch (std::exception& e)
        {
            if (!Output::has_internal_error())
            {
                m_last_error = std::format(STR("Failed to load dll <{}> for mod {}, because: {}\n"),
                                           ensure_str(m_dlls_path / STR("main.dll")),
                                           m_mod_name, ensure_str(e.what()));
                Output::send<LogLevel::Warning>(m_last_error);
            }
            else
            {
                auto error = std::format("Internal Error: {}\n", e.what());

                m_last_error = to_wstring(error);
                printf_s(error.c_str());
            }
        }
    }

    auto CppMod::uninstall() -> void
    {
        Output::send(STR("Stopping C++ mod '{}' for uninstall\n"), m_mod_name);

        if (m_mod && m_uninstall_mod_func)
        {
            // flag as stopped before actually deleting
            // the instance to prevent calls to on_update
            m_is_started = false;
            m_uninstall_mod_func(m_mod);
            m_last_error.clear();
            m_mod = nullptr;
        }
    }

    auto CppMod::fire_on_lua_start(StringViewType mod_name,
                                   LuaMadeSimple::Lua& lua,
                                   LuaMadeSimple::Lua& main_lua,
                                   LuaMadeSimple::Lua& async_lua,
                                   std::vector<LuaMadeSimple::Lua*>& hook_luas) -> void
    {
        if (m_mod)
        {
            m_mod->on_lua_start(mod_name, lua, main_lua, async_lua, hook_luas);
        }
    }

    auto CppMod::fire_on_lua_start(LuaMadeSimple::Lua& lua,
                                   LuaMadeSimple::Lua& main_lua,
                                   LuaMadeSimple::Lua& async_lua,
                                   std::vector<LuaMadeSimple::Lua*>& hook_luas) -> void
    {
        if (m_mod)
        {
            m_mod->on_lua_start(lua, main_lua, async_lua, hook_luas);
        }
    }

    auto CppMod::fire_on_lua_stop(StringViewType mod_name,
                                  LuaMadeSimple::Lua& lua,
                                  LuaMadeSimple::Lua& main_lua,
                                  LuaMadeSimple::Lua& async_lua,
                                  std::vector<LuaMadeSimple::Lua*>& hook_luas) -> void
    {
        if (m_mod)
        {
            m_mod->on_lua_stop(mod_name, lua, main_lua, async_lua, hook_luas);
        }
    }

    auto CppMod::fire_on_lua_stop(LuaMadeSimple::Lua& lua, LuaMadeSimple::Lua& main_lua, LuaMadeSimple::Lua& async_lua, std::vector<LuaMadeSimple::Lua*>& hook_luas) -> void
    {
        if (m_mod)
        {
            m_mod->on_lua_stop(lua, main_lua, async_lua, hook_luas);
        }
    }

    auto CppMod::fire_unreal_init() -> void
    {
        if (m_mod)
        {
            m_mod->on_unreal_init();
        }
    }

    auto CppMod::fire_ui_init() -> void
    {
        if (m_mod)
        {
            m_mod->on_ui_init();
        }
    }

    auto CppMod::fire_program_start() -> void
    {
        if (m_mod)
        {
            m_mod->on_program_start();
        }
    }

    auto CppMod::fire_update() -> void
    {
        if (m_mod)
        {
            m_mod->on_update();
        }
    }

    auto CppMod::fire_dll_load(StringViewType dll_name) -> void
    {
        if (m_mod)
        {
            m_mod->on_dll_load(dll_name);
        }
    }

    auto CppMod::unload_dll() -> void
    {
        if (is_started())
            uninstall();

        if (m_main_dll_module != nullptr)
        {
            FreeLibrary(m_main_dll_module);
            RemoveDllDirectory(m_dlls_path_cookie);
            m_uninstall_mod_func = nullptr;
            m_main_dll_module = nullptr;
            m_start_mod_func = nullptr;
        }
    }

    auto CppMod::load_dll() -> bool
    {
        if (is_loaded())
            return true;

        auto dll_path = get_dll_path();
        // Add mods dlls directory to search path for dynamic/shared linked libraries in mods
        m_dlls_path_cookie = AddDllDirectory(m_dlls_path.c_str());
        m_main_dll_module = LoadLibraryExW(dll_path.c_str(), nullptr, LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR | LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);

        if (!m_main_dll_module)
        {
            m_last_error = std::format(STR("Failed to load dll <{}> for mod {}, error code: 0x{:x}\n"), ensure_str(dll_path), m_mod_name, GetLastError());
            Output::send<LogLevel::Warning>(m_last_error);
            set_installable(false);

            return false;
        }

        m_uninstall_mod_func = reinterpret_cast<uninstall_type>(GetProcAddress(m_main_dll_module, "uninstall_mod"));
        m_start_mod_func = reinterpret_cast<start_type>(GetProcAddress(m_main_dll_module, "start_mod"));

        if (!m_start_mod_func || !m_uninstall_mod_func)
        {
            m_last_error = std::format(STR("Failed to find exported mod lifecycle functions for mod {}\n"), m_mod_name);
            Output::send<LogLevel::Warning>(m_last_error);
            set_installable(false);
            unload_dll();

            return false;
        }

        return true;
    }

    auto CppMod::get_dll_path() const -> std::filesystem::path
    {
        return m_dlls_path / STR("main.dll");
    }

    auto CppMod::mod_info() const -> ModMetadata
    {
        return m_mod != nullptr ? ModMetadata {
            .ModName = m_mod->ModName,
            .ModVersion = m_mod->ModVersion,
            .ModDescription = m_mod->ModDescription,
            .ModAuthors = m_mod->ModAuthors,
        } : ModMetadata {};
    }

    auto CppMod::is_loaded() const -> bool
    {
        return m_main_dll_module != nullptr;
    }

    CppMod::~CppMod()
    {
        unload_dll();
    }
} // namespace RC
