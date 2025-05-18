#pragma once

struct Parameters
{
    // General.
    static constexpr char PLUGIN_ENABLED_ID[] = "plugin-enabled";
    static constexpr bool PLUGIN_ENABLED_DEFAULT = true;

    // Reverb.
    static constexpr char REVERB_ENABLED_ID[] = "reverb-enabled";
    static constexpr bool REVERB_ENABLED_DEFAULT = true;
    static constexpr char REVERB_SIZE_ID[] = "reverb-size";
    static constexpr float REVERB_SIZE_DEFAULT = .5f;
    static constexpr char REVERB_WIDTH_ID[] = "reverb-width";
    static constexpr float REVERB_WIDTH_DEFAULT = .5f;
    static constexpr char REVERB_HPF_ID[] = "reverb-hpf";
    static constexpr float REVERB_HPF_DEFAULT = 120.f;
    static constexpr char REVERB_LPF_ID[] = "reverb-lpf";
    static constexpr float REVERB_LPF_DEFAULT = 10000.f;
    static constexpr char REVERB_DRY_WET_ID[] = "reverb-dry-wet";
    static constexpr float REVERB_DRY_WET_DEFAULT = 0.f;
};
