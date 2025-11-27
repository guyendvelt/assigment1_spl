#include "WAVTrack.h"
#include <iostream>

WAVTrack::WAVTrack(const std::string& title, const std::vector<std::string>& artists, 
                   int duration, int bpm, int sample_rate, int bit_depth)
    : AudioTrack(title, artists, duration, bpm), sample_rate(sample_rate), bit_depth(bit_depth) {

    std::cout << "WAVTrack created: " << sample_rate << "Hz/" << bit_depth << "bit" << std::endl;
}

// ========== TODO: STUDENTS IMPLEMENT THESE VIRTUAL FUNCTIONS ==========

void WAVTrack::load() {
    // TODO: Implement realistic WAV loading simulation
    // NOTE: Use exactly 2 spaces before the arrow (→) character
    std::cout << "[WAVTrack::load] Loading WAV: \"" << title << "\" at " 
              << sample_rate << "Hz/" << bit_depth << "bit (uncompressed)..." << std::endl;
    long long estimated_size = (long long)duration_seconds * sample_rate * (bit_depth / 8) * 2;
    std::cout << "  -> Estimated file size: " << estimated_size << " bytes" << std::endl;
    std::cout << "  -> Fast loading due to uncompressed format." << std::endl;
}

void WAVTrack::analyze_beatgrid() {
    std::cout << "[WAVTrack::analyze_beatgrid] Analyzing beat grid for: \"" << title << "\"\n";
    double estimated_beats = (duration_seconds / 60.0) * bpm;
    double precision_factor = 1.0;
    std::cout << "  -> Estimated beats: " << estimated_beats 
              << "  -> Precision factor: " << precision_factor 
              << " (uncompressed audio)\n";
}

double WAVTrack::get_quality_score() const {
    // TODO: Implement WAV quality scoring
    // NOTE: Use exactly 2 spaces before each arrow (→) character
    // NOTE: Cast beats to integer when printing
    double score = 70.0;
    if (sample_rate >= 44100) { 
        score += 10.0;
    }
    if (sample_rate >= 96000) { 
        score += 5.0;
    }
    if (bit_depth >= 16) { 
        score += 10.0;
    }
    if (bit_depth >= 24) { 
        score += 5.0;
    }
    if (score > 100.0) {
        score = 100.0;
    }

    return score;
   
}

PointerWrapper<AudioTrack> WAVTrack::clone() const {
    // TODO: Implement the clone method
    return PointerWrapper<AudioTrack>(new WAVTrack(*this)); 
}