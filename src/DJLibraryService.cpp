#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>

DJLibraryService::~DJLibraryService(){
    for(AudioTrack* track : library){
        if(track){
            delete track;
            track = nullptr;
        }
    }
    library.clear();
}





DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist), library() {}
/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */

void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    for(size_t i = 0; i < library_tracks.size(); i++){
        SessionConfig::TrackInfo curr = library_tracks[i];
        AudioTrack* track = nullptr;
        if(curr.type == "MP3"){
            track = new MP3Track(curr.title, curr.artists, curr.duration_seconds, curr.bpm, curr.extra_param1, curr.extra_param2);
        } else if(curr.type == "WAV") {
            track = new WAVTrack(curr.title, curr.artists, curr.duration_seconds, curr.bpm, curr.extra_param1, curr.extra_param2);
        } else {
            std::cout << "[WARNING] unknown type" << std::endl;
        }
        library.push_back(track);
        std::cout << "[INFO] Track library built: " << library.size() << " tracks loaded";
    }
  
}

/**
 * @brief Display the current state of the DJ library playlist
 * 
 */
void DJLibraryService::displayLibrary() const {
    std::cout << "=== DJ Library Playlist: " 
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty()) {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 * 
 * @return Playlist& 
 */
Playlist& DJLibraryService::getPlaylist() {
    // Your implementation here
    return playlist;
}

/**
 * TODO: Implement findTrack method
 * 
 * HINT: Leverage Playlist's find_track method
 */
AudioTrack* DJLibraryService::findTrack(const std::string& track_title) {
    return  playlist.find_track(track_title);;
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
                                               const std::vector<int>& track_indices) {
    std::cout << "[INFO] Loading playlist: " << playlist_name << std::endl;
    playlist = Playlist(playlist_name);
    for(int index : track_indices){
        int real_index = index-1;
        if(real_index < 0 || static_cast<size_t>(real_index) >= library.size()){
            std::cout << "[WARNING] Invalid Track Index" << index << std::endl;
            continue;
        } else {
            PointerWrapper<AudioTrack> track = library[real_index]->clone();
            if(!track) {
                std::cerr << "[ERROR] track is Null Pointer" << std::endl;
                continue;
            } else {
                track->load();
                track->analyze_beatgrid();
                playlist.add_track(track.get());
                std::cout << "Added '" << track->get_title() << "' to playlist '" << playlist_name << "'" << std::endl;
            }
        }
    }

                std::cout << "[INFO] Playlist loaded: " << playlist_name 
                    << " (" << playlist.get_track_count() << " tracks)" << std::endl;


}
/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    std::vector<std::string> titles; 
    std::vector<AudioTrack*> tracks = playlist.getTracks();
    for(AudioTrack* track : tracks){
        if(track){
            titles.push_back(track->get_title());
        }
    }
    return titles;
}
