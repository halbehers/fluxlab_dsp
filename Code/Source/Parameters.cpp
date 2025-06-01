#include "Parameters.h"

void Parameters::registerPluginParameters(PluginAudioProcessor* audioProcessor)
{
    audioProcessor->registerParameter
           (
               PLUGIN_ENABLED_ID,
               "Plugin Enabled",
               PLUGIN_ENABLED_DEFAULT,
               [audioProcessor](bool value) { audioProcessor->audioEngine.setEnabled(value); },
               "Bypass the whole plugin."
            );
}

void Parameters::registerTempoParameters(PluginAudioProcessor* audioProcessor)
{
    audioProcessor->registerParameter
           (
               TEMPO_VALUE_ID,
               "Tempo",
               TEMPO_VALUE_DEFAULT,
               20.f,
               200.f,
               [audioProcessor](float value) {
                   audioProcessor->delayProcess.setBPM(value);
               },
               "The tempo selection."
            );
}

void Parameters::registerCrossfaderParameters(PluginAudioProcessor* audioProcessor)
{
    audioProcessor->registerParameter
           (
               CROSSFADER_VALUE_ID,
               "Crossfader",
               CROSSFADER_VALUE_DEFAULT,
               0.f,
               1.f,
               [audioProcessor](float value) {
                   audioProcessor->audioEngine.setDryWet(value);
               },
               "The main crossfader to select how much of the wet signal is going through."
            );
}

void Parameters::registerReverbParameters(PluginAudioProcessor* audioProcessor)
{
    audioProcessor->registerParameter
         (
             REVERB_ENABLED_ID,
             "Reverb Enabled",
             REVERB_ENABLED_DEFAULT,
             [audioProcessor](bool value) {
                 audioProcessor->reverbProcess.setEnabled(value);
             },
             "Bypass the reverb."
         );
    audioProcessor->registerParameter
    (
        REVERB_SIZE_ID,
        "Reverb Size",
        "Size",
        REVERB_SIZE_DEFAULT,
        0.f,
        1.f,
        [audioProcessor](float value) {
            audioProcessor->reverbProcess.setSize(value);
        },
        "Reverb's room size."
    );
    audioProcessor->registerParameter
    (
        REVERB_WIDTH_ID,
        "Reverb Width",
        "Width",
        REVERB_WIDTH_DEFAULT,
        0.f,
        1.f,
        [audioProcessor](float value) {
            audioProcessor->reverbProcess.setWidth(value);
        },
        "Reverb's width."
    );
    audioProcessor->registerParameter
    (
        REVERB_HPF_ID,
        "Reverb HPF",
        "High Pass",
        REVERB_HPF_DEFAULT,
        20.f,
        20000.f,
        [audioProcessor](float value) { audioProcessor->reverbProcess.setHPF(value); },
        "Reverb's high pass filter."
    );
    audioProcessor->registerParameter
    (
        REVERB_LPF_ID,
        "Reverb LPF",
        "Low Pass",
        REVERB_LPF_DEFAULT,
        20.f,
        20000.f,
        [audioProcessor](float value) { audioProcessor->reverbProcess.setLPF(value); },
        "Reverb's low pass filter."
    );
    audioProcessor->registerParameter
    (
        REVERB_DRY_WET_ID,
        "Reverb Dry/Wet",
        "Dry / Wet",
        REVERB_DRY_WET_DEFAULT,
        0.f,
        1.f,
        [audioProcessor](float value) { audioProcessor->reverbProcess.setDryWet(value); },
        "Reverb's dry / wet mix."
    );
}

void Parameters::registerDelayParameters(PluginAudioProcessor* audioProcessor)
{
    audioProcessor->registerParameter
        (
            DELAY_ENABLED_ID,
            "Delay Enabled",
            DELAY_ENABLED_DEFAULT,
            [audioProcessor](bool value) {
                audioProcessor->delayProcess.setEnabled(value);
            },
            "Bypass the delay."
        );
    audioProcessor->registerParameter
    (
        DELAY_TIME_MS_ID,
        "Delay Time (ms)",
        "Time (ms)",
        DELAY_TIME_MS_DEFAULT,
        1.f,
        2000.f,
        [audioProcessor](float value) {
            audioProcessor->delayProcess.setTimeInMs(value);
        },
        "Delay's time in ms."
    );
    audioProcessor->registerParameter
    (
        DELAY_TIMING_ID,
        "Delay Timing",
        "Timing",
        DELAY_TIMING_DEFAULT,
        ndsp::Timing::NOTE_32,
        ndsp::Timing::NOTE_FOUR_STEP,
        [audioProcessor](int value) {
            audioProcessor->delayProcess.setTiming(static_cast<ndsp::Timing::NoteTiming>(value));
        },
        "Delay's note timing."
    );
    audioProcessor->registerParameter
    (
        DELAY_FEEDBACK_ID,
        "Delay Feedback",
        "Feedback",
        DELAY_FEEDBACK_DEFAULT,
        0.f,
        .95f,
        [audioProcessor](float value) {
            audioProcessor->delayProcess.setFeedback(value);
        },
        "Delay's feedback."
    );
    audioProcessor->registerParameter
    (
        DELAY_HPF_ID,
        "Delay HPF",
        "High Pass",
        DELAY_HPF_DEFAULT,
        20.f,
        20000.f,
        [audioProcessor](float value) { audioProcessor->delayProcess.setHPF(value); },
        "Delay's high pass filter."
    );
    audioProcessor->registerParameter
    (
        DELAY_LPF_ID,
        "Delay LPF",
        "Low Pass",
        DELAY_LPF_DEFAULT,
        20.f,
        20000.f,
        [audioProcessor](float value) { audioProcessor->delayProcess.setLPF(value); },
        "Delay's low pass filter."
    );
    audioProcessor->registerParameter
    (
        DELAY_DRY_WET_ID,
        "Delay Dry/Wet",
        "Dry / Wet",
        DELAY_DRY_WET_DEFAULT,
        0.f,
        1.f,
        [audioProcessor](float value) { audioProcessor->delayProcess.setDryWet(value); },
        "Delay's dry / wet mix."
    );
}

void Parameters::registerPhaserParameters(PluginAudioProcessor* audioProcessor)
{
    audioProcessor->registerParameter
    (
        PHASER_ENABLED_ID,
        "Phaser Enabled",
        PHASER_ENABLED_DEFAULT,
        [audioProcessor](bool value) {
            audioProcessor->phaserProcess.setEnabled(value);
        },
        "Bypass the chorus."
    );
    audioProcessor->registerParameter
    (
        PHASER_RATE_ID,
        "Phaser Rate",
        "Rate",
        PHASER_RATE_DEFAULT,
        0.f,
        100.f,
        [audioProcessor](float value) {
            audioProcessor->phaserProcess.setRate(value);
        },
        "Phaser's rate."
    );
    audioProcessor->registerParameter
    (
        PHASER_DEPTH_ID,
        "Phaser Depth",
        "Depth",
        PHASER_DEPTH_DEFAULT,
        0.f,
        1.f,
        [audioProcessor](float value) {
            audioProcessor->phaserProcess.setDepth(value);
        },
        "Phaser's depth."
    );
    audioProcessor->registerParameter
    (
        PHASER_HPF_ID,
        "Phaser HPF",
        "High Pass",
        PHASER_HPF_DEFAULT,
        20.f,
        20000.f,
        [audioProcessor](float value) { audioProcessor->phaserProcess.setHPF(value); },
        "Phaser's high pass filter."
    );
    audioProcessor->registerParameter
    (
        PHASER_LPF_ID,
        "Phaser LPF",
        "Low Pass",
        PHASER_LPF_DEFAULT,
        20.f,
        20000.f,
        [audioProcessor](float value) { audioProcessor->phaserProcess.setLPF(value); },
        "Phaser's low pass filter."
    );
    audioProcessor->registerParameter
    (
        PHASER_DRY_WET_ID,
        "Phaser Dry/Wet",
        "Dry / Wet",
        PHASER_DRY_WET_DEFAULT,
        0.f,
        1.f,
        [audioProcessor](float value) { audioProcessor->phaserProcess.setDryWet(value); },
        "Phaser's dry / wet mix."
    );
}

void Parameters::registerChorusParameters(PluginAudioProcessor* audioProcessor)
{
    audioProcessor->registerParameter
        (
            CHORUS_ENABLED_ID,
            "Chorus Enabled",
            CHORUS_ENABLED_DEFAULT,
            [audioProcessor](bool value) {
                audioProcessor->chorusProcess.setEnabled(value);
            },
            "Bypass the chorus."
        );
    audioProcessor->registerParameter
    (
        CHORUS_RATE_ID,
        "Chorus Rate",
        "Rate",
        CHORUS_RATE_DEFAULT,
        0.f,
        100.f,
        [audioProcessor](float value) {
            audioProcessor->chorusProcess.setRate(value);
        },
        "Chorus's rate."
    );
    audioProcessor->registerParameter
    (
        CHORUS_DEPTH_ID,
        "Chorus Depth",
        "Depth",
        CHORUS_DEPTH_DEFAULT,
        0.f,
        1.f,
        [audioProcessor](float value) {
            audioProcessor->chorusProcess.setDepth(value);
        },
        "Chorus's depth."
    );
    audioProcessor->registerParameter
    (
        CHORUS_HPF_ID,
        "Chorus HPF",
        "High Pass",
        CHORUS_HPF_DEFAULT,
        20.f,
        20000.f,
        [audioProcessor](float value) { audioProcessor->chorusProcess.setHPF(value); },
        "Chorus's high pass filter."
    );
    audioProcessor->registerParameter
    (
        CHORUS_LPF_ID,
        "Chorus LPF",
        "Low Pass",
        CHORUS_LPF_DEFAULT,
        20.f,
        20000.f,
        [audioProcessor](float value) { audioProcessor->chorusProcess.setLPF(value); },
        "Chorus's low pass filter."
    );
    audioProcessor->registerParameter
    (
        CHORUS_DRY_WET_ID,
        "Chorus Dry/Wet",
        "Dry / Wet",
        CHORUS_DRY_WET_DEFAULT,
        0.f,
        1.f,
        [audioProcessor](float value) { audioProcessor->chorusProcess.setDryWet(value); },
        "Chorus's dry / wet mix."
    );
}

void Parameters::registerDJFilterParameters(PluginAudioProcessor* audioProcessor)
{
    audioProcessor->registerParameter
    (
        DJ_FILTER_ENABLED_ID,
        "DJ Filter Enabled",
        DJ_FILTER_ENABLED_DEFAULT,
        [audioProcessor](bool value) {
            audioProcessor->djFilterProcess.setEnabled(value);
        },
        "Bypass the DJ Filter."
    );
    audioProcessor->registerParameter
    (
        DJ_FILTER_VALUE_ID,
        "DJ Filter Value",
        "Value",
        DJ_FILTER_VALUE_DEFAULT,
        -1.f,
        1.f,
        [audioProcessor](float value) {
            audioProcessor->djFilterProcess.setValue(value);
        },
        "DJ filter's value."
    );
    audioProcessor->registerParameter
    (
        DJ_FILTER_Q_ID,
        "DJ Filter Resonance",
        "Res.",
        DJ_FILTER_Q_DEFAULT,
        .707f,
        10.f,
        [audioProcessor](float value) {
            audioProcessor->djFilterProcess.setQ(value);
        },
        "DJ filter's Resonance."
    );
}

void Parameters::registerCompressorParameters(PluginAudioProcessor* audioProcessor)
{
    audioProcessor->registerParameter
    (
        COMPRESSOR_ENABLED_ID,
        "Phaser Enabled",
        COMPRESSOR_ENABLED_DEFAULT,
        [audioProcessor](bool value) {
            audioProcessor->compressorProcess.setEnabled(value);
        },
        "Bypass the compressor."
    );
    audioProcessor->registerParameter
    (
        COMPRESSOR_THRESHOLD_ID,
        "Compressor Threshold",
        "Threshold",
        COMPRESSOR_THRESHOLD_DEFAULT,
        -60.f,
        0.f,
        [audioProcessor](float value) {
            audioProcessor->compressorProcess.setThreshold(value);
        },
        "Compressor's threshold."
    );
    audioProcessor->registerParameter
    (
        COMPRESSOR_RATIO_ID,
        "Compressor Ratio",
        "Ratio",
        COMPRESSOR_RATIO_DEFAULT,
        1.f,
        20.f,
        [audioProcessor](float value) {
            audioProcessor->compressorProcess.setRatio(value);
        },
        "Compressor's ratio."
    );
    audioProcessor->registerParameter
    (
        COMPRESSOR_ATTACK_ID,
        "Compressor Attack",
        "Attack",
        COMPRESSOR_ATTACK_DEFAULT,
        .1f,
        800.f,
        [audioProcessor](float value) { audioProcessor->compressorProcess.setAttack(value); },
        "Compressor's attack."
    );
    audioProcessor->registerParameter
    (
        COMPRESSOR_RELEASE_ID,
        "Compressor Release",
        "Release",
        COMPRESSOR_RELEASE_DEFAULT,
        .1f,
        800.f,
        [audioProcessor](float value) { audioProcessor->compressorProcess.setRelease(value); },
        "Compressor's release."
    );
    audioProcessor->registerParameter
    (
        COMPRESSOR_DRY_WET_ID,
        "Compressor Dry/Wet",
        "Dry / Wet",
        COMPRESSOR_DRY_WET_DEFAULT,
        0.f,
        1.f,
        [audioProcessor](float value) { audioProcessor->compressorProcess.setDryWet(value); },
        "Compressor's dry / wet mix."
    );
}

void Parameters::registerSection(Section section, PluginAudioProcessor* audioProcessor)
{
    switch (section)
    {
        case PLUGIN:
            registerPluginParameters(audioProcessor);
            break;
        case TEMPO:
            registerTempoParameters(audioProcessor);
            break;
        case CROSSFADER:
            registerCrossfaderParameters(audioProcessor);
            break;
        case REVERB:
            registerReverbParameters(audioProcessor);
            break;
        case DELAY:
            registerDelayParameters(audioProcessor);
            break;
        case CHORUS:
            registerChorusParameters(audioProcessor);
            break;
        case PHASER:
            registerPhaserParameters(audioProcessor);
            break;
        case DJ_FILTER:
            registerDJFilterParameters(audioProcessor);
            break;
        case COMPRESSOR:
            registerCompressorParameters(audioProcessor);
            break;
    }
}

void Parameters::registerAllSections(PluginAudioProcessor* audioProcessor)
{
    for (int section = PLUGIN; section <= COMPRESSOR; ++section)
    {
        registerSection(static_cast<Section>(section), audioProcessor);
    }
}