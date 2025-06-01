#pragma once

#include "PluginProcessor.h"

struct Parameters
{
    // General.
    static constexpr char PLUGIN_ENABLED_ID[] = "plugin-enabled";
    static constexpr bool PLUGIN_ENABLED_DEFAULT = true;

    // Tempo.
    static constexpr char TEMPO_VALUE_ID[] = "tempo-value";
    static constexpr float TEMPO_VALUE_DEFAULT = 120.f;

    static constexpr char CROSSFADER_VALUE_ID[] = "crossfader-value";
    static constexpr float CROSSFADER_VALUE_DEFAULT = 0.f;

    // Reverb.
    static constexpr char REVERB_SECTION_ID[] = "reverb";
    static constexpr char REVERB_ENABLED_ID[] = "reverb-enabled";
    static constexpr bool REVERB_ENABLED_DEFAULT = true;
    static constexpr char REVERB_SIZE_ID[] = "reverb-size";
    static constexpr float REVERB_SIZE_DEFAULT = .5f;
    static constexpr char REVERB_WIDTH_ID[] = "reverb-width";
    static constexpr float REVERB_WIDTH_DEFAULT = .5f;
    static constexpr char REVERB_HPF_ID[] = "reverb-hpf";
    static constexpr float REVERB_HPF_DEFAULT = 2000.f;
    static constexpr char REVERB_LPF_ID[] = "reverb-lpf";
    static constexpr float REVERB_LPF_DEFAULT = 16000.f;
    static constexpr char REVERB_DRY_WET_ID[] = "reverb-dry-wet";
    static constexpr float REVERB_DRY_WET_DEFAULT = 0.f;

    // Delay.
    static constexpr char DELAY_SECTION_ID[] = "delay";
    static constexpr char DELAY_ENABLED_ID[] = "delay-enabled";
    static constexpr bool DELAY_ENABLED_DEFAULT = true;
    static constexpr char DELAY_TIME_MS_ID[] = "delay-time-ms";
    static constexpr float DELAY_TIME_MS_DEFAULT = 375.f;
    static constexpr char DELAY_TIMING_ID[] = "delay-timing";
    static constexpr int DELAY_TIMING_DEFAULT = ndsp::Timing::NOTE_4;
    static constexpr char DELAY_FEEDBACK_ID[] = "delay-feedback";
    static constexpr float DELAY_FEEDBACK_DEFAULT = 0.5f;
    static constexpr char DELAY_HPF_ID[] = "delay-hpf";
    static constexpr float DELAY_HPF_DEFAULT = 2000.f;
    static constexpr char DELAY_LPF_ID[] = "delay-lpf";
    static constexpr float DELAY_LPF_DEFAULT = 16000.f;
    static constexpr char DELAY_DRY_WET_ID[] = "delay-dry-wet";
    static constexpr float DELAY_DRY_WET_DEFAULT = 0.f;

    // Phaser.
    static constexpr char PHASER_SECTION_ID[] = "phaser";
    static constexpr char PHASER_ENABLED_ID[] = "phaser-enabled";
    static constexpr bool PHASER_ENABLED_DEFAULT = true;
    static constexpr char PHASER_RATE_ID[] = "phaser-rate";
    static constexpr int PHASER_RATE_DEFAULT = ndsp::Timing::NOTE_4;
    static constexpr char PHASER_DEPTH_ID[] = "phaser-depth";
    static constexpr float PHASER_DEPTH_DEFAULT = .5f;
    static constexpr char PHASER_HPF_ID[] = "phaser-hpf";
    static constexpr float PHASER_HPF_DEFAULT = 2000.f;
    static constexpr char PHASER_LPF_ID[] = "phaser-lpf";
    static constexpr float PHASER_LPF_DEFAULT = 16000.f;
    static constexpr char PHASER_DRY_WET_ID[] = "phaser-dry-wet";
    static constexpr float PHASER_DRY_WET_DEFAULT = 0.f;

    // Chorus.
    static constexpr char CHORUS_SECTION_ID[] = "chorus";
    static constexpr char CHORUS_ENABLED_ID[] = "chorus-enabled";
    static constexpr bool CHORUS_ENABLED_DEFAULT = true;
    static constexpr char CHORUS_RATE_ID[] = "chorus-rate";
    static constexpr int CHORUS_RATE_DEFAULT = ndsp::Timing::NOTE_4;
    static constexpr char CHORUS_DEPTH_ID[] = "chorus-depth";
    static constexpr float CHORUS_DEPTH_DEFAULT = .5f;
    static constexpr char CHORUS_HPF_ID[] = "chorus-hpf";
    static constexpr float CHORUS_HPF_DEFAULT = 2000.f;
    static constexpr char CHORUS_LPF_ID[] = "chorus-lpf";
    static constexpr float CHORUS_LPF_DEFAULT = 16000.f;
    static constexpr char CHORUS_DRY_WET_ID[] = "chorus-dry-wet";
    static constexpr float CHORUS_DRY_WET_DEFAULT = 0.f;

    // DJ Filter.
    static constexpr char DJ_FILTER_SECTION_ID[] = "dj-filter";
    static constexpr char DJ_FILTER_ENABLED_ID[] = "dj-filter-enabled";
    static constexpr bool DJ_FILTER_ENABLED_DEFAULT = true;
    static constexpr char DJ_FILTER_VALUE_ID[] = "dj-filter-value";
    static constexpr float DJ_FILTER_VALUE_DEFAULT = 0.f;
    static constexpr char DJ_FILTER_Q_ID[] = "dj-filter-q";
    static constexpr float DJ_FILTER_Q_DEFAULT = .707f;

    // Compressor.
    static constexpr char COMPRESSOR_SECTION_ID[] = "compressor";
    static constexpr char COMPRESSOR_ENABLED_ID[] = "compressor-enabled";
    static constexpr bool COMPRESSOR_ENABLED_DEFAULT = true;
    static constexpr char COMPRESSOR_THRESHOLD_ID[] = "compressor-threshold";
    static constexpr float COMPRESSOR_THRESHOLD_DEFAULT = -5.f;
    static constexpr char COMPRESSOR_RATIO_ID[] = "compressor-ratio";
    static constexpr float COMPRESSOR_RATIO_DEFAULT = 2.f;
    static constexpr char COMPRESSOR_ATTACK_ID[] = "compressor-attack";
    static constexpr float COMPRESSOR_ATTACK_DEFAULT = 100;
    static constexpr char COMPRESSOR_RELEASE_ID[] = "compressor-release";
    static constexpr float COMPRESSOR_RELEASE_DEFAULT = 200;
    static constexpr char COMPRESSOR_DRY_WET_ID[] = "compressor-dry-wet";
    static constexpr float COMPRESSOR_DRY_WET_DEFAULT = 0.f;

    enum Section
    {
        PLUGIN,
        TEMPO,
        CROSSFADER,
        REVERB,
        DELAY,
        PHASER,
        CHORUS,
        DJ_FILTER,
        COMPRESSOR,
    };

    static void registerPluginParameters(PluginAudioProcessor* audioProcessor);
    static void registerTempoParameters(PluginAudioProcessor* audioProcessor);
    static void registerCrossfaderParameters(PluginAudioProcessor* audioProcessor);
    static void registerReverbParameters(PluginAudioProcessor* audioProcessor);
    static void registerDelayParameters(PluginAudioProcessor* audioProcessor);
    static void registerPhaserParameters(PluginAudioProcessor* audioProcessor);
    static void registerChorusParameters(PluginAudioProcessor* audioProcessor);
    static void registerDJFilterParameters(PluginAudioProcessor* audioProcessor);
    static void registerCompressorParameters(PluginAudioProcessor* audioProcessor);

    static void registerSection(Section section, PluginAudioProcessor* audioProcessor);
    static void registerAllSections(PluginAudioProcessor* audioProcessor);
};
