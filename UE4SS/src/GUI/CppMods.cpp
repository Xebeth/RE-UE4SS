#include <vector>

#include <DynamicOutput/DynamicOutput.hpp>
#include <GUI/ImGuiUtility.hpp>
#include <Helpers/String.hpp>

#include <imgui.h>

#include "UE4SSProgram.hpp"
#include "UnrealInitializer.hpp"

namespace RC::GUI::CppMods
{
    using namespace RC::Unreal;

    struct ModInfo
    {
        std::string ModAuthor{};
        std::string ModDescription{};
        std::string ModVersion{};
        std::string ModName{};
        std::string LastError{};
        bool Installed{};
        bool Installable{};
        bool Started{};
        bool Loaded{};
    };

    auto start_mod(CppMod* pMod) -> void
    {
        if (pMod != nullptr)
        {
            pMod->start_mod();

            if (pMod->is_started())
            {
                if (UnrealInitializer::StaticStorage::bIsInitialized)
                    pMod->fire_unreal_init();

                if (UE4SSProgram::get_program().is_program_started())
                    pMod->fire_program_start();
            }
        }
    }

    auto stop_mod(CppMod* pMod) -> void
    {
        if (pMod != nullptr)
        {
            UE4SSProgram::get_program().m_pause_events_processing = true;
            pMod->uninstall();
            UE4SSProgram::get_program().m_pause_events_processing = false;
        }
    }

    auto load_mod(CppMod* pMod) -> void
    {
        if (pMod != nullptr)
        {
            pMod->load_dll();
            start_mod(pMod);
        }
    }

    auto unload_mod(CppMod* pMod) -> void
    {
        if (pMod != nullptr)
        {
            UE4SSProgram::get_program().m_pause_events_processing = true;
            pMod->unload_dll();
            UE4SSProgram::get_program().m_pause_events_processing = false;
        }
    }

    auto render() -> void
    {
        if (UE4SSProgram::get_program().m_pause_events_processing)
            return;

        auto i = 0;

        for (auto& mod : UE4SSProgram::get_program().m_mods)
        {
            const auto modPtr = mod.get();

            if (!mod || !dynamic_cast<CppMod*>(modPtr))
                continue;

            auto cppMod = dynamic_cast<CppMod*>(modPtr);
            auto metadata = cppMod->mod_info();
            auto mod_info = ModInfo{};

            auto mod_name_property = metadata.ModName;
            mod_info.ModName = mod_name_property.empty() ? to_string(cppMod->get_name()) : to_string(mod_name_property);

            auto mod_author_property = metadata.ModAuthors;
            mod_info.ModAuthor = mod_author_property.empty() ? "Unknown" : to_string(mod_author_property);

            auto mod_description_property = metadata.ModDescription;
            mod_info.ModDescription = mod_description_property.empty() ? "Unknown" : to_string(mod_description_property);

            auto mod_version_property = metadata.ModVersion;
            mod_info.ModVersion = mod_version_property.empty() ? "Unknown" : to_string(mod_version_property);

            mod_info.LastError = to_string(cppMod->get_last_error());
            mod_info.Installable = cppMod->is_installable();
            mod_info.Installed = cppMod->is_installed();
            mod_info.Started = cppMod->is_started();
            mod_info.Loaded = cppMod->is_loaded();

            if (ImGui_TreeNodeEx(fmt::format("{}", mod_info.ModName).c_str(), fmt::format("{}_{}", mod_info.ModName, ++i).c_str(), ImGuiTreeNodeFlags_CollapsingHeader))
            {
                ImGui::Indent();
                ImGui::Text("Author: %s", mod_info.ModAuthor.c_str());
                ImGui::Text("Description: %s", mod_info.ModDescription.c_str());
                ImGui::Text("Version: %s", mod_info.ModVersion.c_str());
                ImGui::BeginDisabled();
                ImGui::Checkbox(fmt::format("Installable""##{}", i).c_str(), &mod_info.Installable); ImGui::SameLine();
                ImGui::Checkbox(fmt::format("Installed""##{}", i).c_str(), &mod_info.Installed); ImGui::SameLine();
                ImGui::Checkbox(fmt::format("Started""##{}", i).c_str(), &mod_info.Started); ImGui::SameLine();
                ImGui::Checkbox(fmt::format("Loaded""##{}", i).c_str(), &mod_info.Loaded); ImGui::SameLine();

                if (mod_info.Installable)
                    ImGui::EndDisabled();

                if (ImGui::Button(fmt::format("{}##{}", mod_info.Installable && mod_info.Loaded ? "Unload" : "Load", i).c_str()))
                {
                    if (mod_info.Loaded)
                        unload_mod(cppMod);
                    else
                        load_mod(cppMod);
                }

                if (mod_info.Loaded)
                {
                    ImGui::SameLine();

                    if (ImGui::Button(fmt::format("{}##{}", mod_info.Loaded && mod_info.Started ? "Stop" : "Start", i).c_str()))
                    {
                        if (mod_info.Started)
                            stop_mod(cppMod);
                        else
                            start_mod(cppMod);
                    }
                }

                if (!mod_info.Installable)
                    ImGui::EndDisabled();

                if (mod_info.LastError.empty() == false)
                {
                    ImGui::Text("Last error: "); ImGui::SameLine();
                    ImGui::TextColored(g_imgui_text_live_view_unreflected_data_color, "%s", mod_info.LastError.c_str());
                }
                ImGui::NewLine();
                ImGui::NewLine();
                ImGui::Unindent();
            }
        }
    }
} // namespace RC::GUI::CppMods
