#include "MixingEngineService.h"
#include <iostream>
#include <memory>


/**
 * TODO: Implement MixingEngineService constructor
 */
MixingEngineService::MixingEngineService()
     : decks(), active_deck(1), auto_sync(false), bpm_tolerance(0)
{
    // Your implementation here
    std::cout << "[MixingEngineService] Initialized with 2 empty decks." << std::endl;
    decks[0] = nullptr;
    decks[1] = nullptr;
}

/**
 * TODO: Implement MixingEngineService destructor
 */
MixingEngineService::~MixingEngineService() {
    // Your implementation here
    std::cout << "[MixingEngineService] Cleaning up decks..." << std::endl;
    for(int i = 0; i < 2; i++){
        if(decks[i]){
            delete decks[i];
            decks[i] = nullptr;
        }
    }

}


/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {
    // Your implementation here
    std::cout << "\n=== Loading Track to Deck ===" <<  std::endl;
    PointerWrapper<AudioTrack> wrapped_track(track.clone());
    if(!wrapped_track){
        std::cerr << "[ERROR] Track: \"" << track.get_title() << "\" failed to clone" << std::endl;
        return -1;
    }
    
    int target_deck_index = 1 - active_deck;
    std::cout << "[Deck Switch] Target deck: " << target_deck_index << std::endl;
    if(decks[target_deck_index]){
        delete decks[target_deck_index];
        decks[target_deck_index] = nullptr;
    }
    wrapped_track->load();
    wrapped_track->analyze_beatgrid();
    if(!decks[active_deck]){
        std::cout << "[Sync BPM] Cannot sync - one of the decks is empty." << std::endl;
    } else if(decks[active_deck] != nullptr && auto_sync){
        if(!can_mix_tracks(wrapped_track)){
            sync_bpm(wrapped_track);
        }
    }

    AudioTrack* released_track = wrapped_track.release();
    decks[target_deck_index] = released_track;
    std::cout << "[Load Complete] '" << released_track->get_title() 
              << "' is now loaded on deck " << target_deck_index << std::endl;
    // if(decks[active_deck] != nullptr){
    //     std::cout << "[Unload] Unloading previous deck " << active_deck 
    //               << " (" << decks[active_deck]->get_title() << ")" << std::endl;
    //     delete decks[active_deck];
    //     decks[active_deck] = nullptr;
    // }
    active_deck = target_deck_index;
    std::cout << "[Active Deck] Switched to deck " << active_deck << std::endl;
    return target_deck_index;

}

/**
 * @brief Display current deck status
 */
void MixingEngineService::displayDeckStatus() const {
    std::cout << "\n=== Deck Status ===\n";
    for (size_t i = 0; i < 2; ++i) {
        if (decks[i])
            std::cout << "Deck " << i << ": " << decks[i]->get_title() << "\n";
        else
            std::cout << "Deck " << i << ": [EMPTY]\n";
    }
    std::cout << "Active Deck: " << active_deck << "\n";
    std::cout << "===================\n";
}

/**
 * TODO: Implement can_mix_tracks method
 * 
 * Check if two tracks can be mixed based on BPM difference.
 * 
 * @param track: Track to check for mixing compatibility
 * @return: true if BPM difference <= tolerance, false otherwise
 */
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack>& track) const {
    // Your implementation here
    if(!decks[active_deck] || !track){
        return false;
    }
    double difference = track->get_bpm() - decks[active_deck]->get_bpm();
    if(difference < 0){
        difference = -difference;
    }
    return difference <= bpm_tolerance;

}

/**
 * TODO: Implement sync_bpm method
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {
    // Your implementation here
    if(decks[active_deck] && track){
        double average_bpm = (track->get_bpm() + decks[active_deck]->get_bpm()) / 2 ;
        double original_bpm = track->get_bpm();
        track->set_bpm(average_bpm);
        std::cout << "[Sync BPM] Syncing BPM from " << original_bpm
              << " to " << average_bpm << std::endl;
    } 
    
    
}
