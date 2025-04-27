#pragma once

struct Parameters
{
    // General.
    static constexpr std::string PLUGIN_ENABLED_ID = "plugin-enabled";
    static constexpr bool PLUGIN_ENABLED_DEFAULT = true;

    // Reverb.
    static constexpr std::string REVERB_ENABLED_ID = "reverb-enabled";
    static constexpr bool REVERB_ENABLED_DEFAULT = true;
    static constexpr std::string REVERB_SIZE_ID = "reverb-size";
    static constexpr float REVERB_SIZE_DEFAULT = .5f;
    static constexpr std::string REVERB_WIDTH_ID = "reverb-width";
    static constexpr float REVERB_WIDTH_DEFAULT = .5f;
    static constexpr std::string REVERB_HPF_ID = "reverb-hpf";
    static constexpr float REVERB_HPF_DEFAULT = 120.f;
    static constexpr std::string REVERB_LPF_ID = "reverb-lpf";
    static constexpr float REVERB_LPF_DEFAULT = 10000.f;
};
