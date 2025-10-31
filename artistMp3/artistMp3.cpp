#include <iostream>
#include <filesystem>
#include <string>
#include "fileref.h"
#include "tag.h"
#include <vector>

namespace fs = std::filesystem;
//namespace TagLib is also here for TagLib::Tag, TagLib::FileRef, TagLib::String

static unsigned int successCount = 0;
static unsigned int skipCount = 0;
static unsigned int failureCount = 0;


void displayHelp() {
    std::cout << "For all specified artists, add artist to mp3 title\n";
    std::cout << "USAGE: ./artistMp3 [ -f FILES] [ -a ARTISTS]\n";
    std::cout << "OPTIONS:\n";
    std::cout << "  -h, --help      Show this help message\n";
    std::cout << "  -d, --dir       Specify the path to search\n";
    std::cout << "                  (default is current directory)\n";

    std::cout << "  -f, --files     Specify files to process (.mp3)\n";
    std::cout << "                  (default is all files in directory per the specified artists)\n";

    std::cout << "  -s, --sep       Specify separator between title and artist\n";
    std::cout << "                  (default is \" - \")\n";

    std::cout << "  -r, --rear      Rearrange to 'Title - Artist' instead of 'Artist - Title'\n";

    std::cout << "  -a, --artists   Specify artists to process\n";
    std::cout << "                  (default is all artists found in specified files)\n";
    
    std::cout << "  --total         Select EVERY artist and EVERY file in path\n";
    std::cout << "                  (incompatible with FILES and ARTISTS arguments)\n";
}

void writeResults() {
    std::cout<<"Total files processed: "<<(successCount + skipCount + failureCount)<<"\n";
    std::cout<<"Updated: "<<successCount<<"\n";
    std::cout<<"Skipped: "<<skipCount<<"\n";
    std::cout<<"Failed: "<<failureCount<<"\n";
}

int writeArtist(const std::string& filePath, TagLib::String& title, TagLib::String& artist, const std::string& separator, bool rearFlag) {
    try{
        if(artist==""){
            std::cout << "Skipped (no artist): " << fs::path(filePath).filename() << "\n";
            return -1; //skip empty artist
        }
        //Only take first artist if multiple
        //SpotifyDownloader uses "/" to separate multiple artists
        int artistsSep = artist.find("/");
        if(artistsSep != -1){
            artist = artist.substr(0, artistsSep);
        }
        if (title.find(artist) == std::string::npos) {
                if(filePath.find(artist.to8Bit(true)) != std::string::npos){
                std::cout << "Skipped (artist in filename): " << fs::path(filePath).filename() << "\n";
                return 0; //skip if artist already in filename
            }
            if(rearFlag){
                title = title + separator + artist;
            } else {
                title = artist + separator + title;
            }             
            fs::path p(filePath);
            fs::rename(p, p.parent_path() / (title.to8Bit(true) + p.extension().string()));
            std::cout << "Updated: " << p.filename() << "\n";
        }
    }catch(const std::exception& e){
        std::cout << "Error processing file: " << filePath << " - " << e.what() << "\n";
        return -1;
    }
    return 1;

}

void addTotal(std::vector<std::string>& files, const std::string& directoryPath, const std::string& separator, bool rearFlag) {
    for (const auto& entry : fs::directory_iterator(directoryPath)) {
        if (entry.path().extension() == ".mp3"
        && entry.exists() 
        && entry.is_regular_file()) {
            files.push_back(entry.path().string());
        }
    }


    for (const auto& filePath : files) {
        TagLib::FileRef f(filePath.c_str());
        if (!f.isNull() && f.tag()) {
            TagLib::Tag* tag = f.tag();
            TagLib::String title = tag->title();
            TagLib::String artist = tag->artist();
            int res = writeArtist(filePath, title, artist, separator, rearFlag);
            switch (res)
            {
            case -1:
                failureCount++;
                break;
            case 1:
                successCount++;
                break;
            case 0:
                skipCount++;
                break;
            default:
                break;
            }
            
          
        }
    }
    writeResults();
}


// @brief Iterates through all mp3 files (unless specific files are provided) in the given directory path
// and adds the artist to the title if the artist is in the provided list (or all artists if the list is empty).
// @param files List of file names
// @param artists  List of artist names (only those are processed)
// @param directoryPath  Directory path to search for mp3 files
// @param separator  Separator to use between artist and title
// @param rearFlag  If true, appends artist to the end of the title; otherwise, prepends it.
void addArtistsToTitles(std::vector<std::string>& files, std::vector<std::string>& artists, const std::string& directoryPath, const std::string& separator, bool rearFlag) {
    if(files.empty()){
        for (auto& entry : fs::directory_iterator(directoryPath)) {
            if (entry.path().extension() == ".mp3"
            && entry.exists() 
            && entry.is_regular_file()) {
                files.push_back(entry.path().string());
            }
        }
    }


    for (auto& filePath : files) {
        TagLib::FileRef f(filePath.c_str());
        if (!f.isNull() && f.tag()) {

            TagLib::Tag* tag = f.tag();
            TagLib::String title = tag->title();
            TagLib::String artist = tag->artist();
           
            switch (writeArtist(filePath, title, artist, separator, rearFlag))
            {
                case 1:
                    successCount++;
                    break;
                case -1:
                    failureCount++;
                    break;
                default:
                    break;
            }
        }
    }
    writeResults();
}

enum ArgumentType {
    HELP,            // -h --help      help message
    DIR,             // -d --dir       directory path
    SEP,             // -s --sep       separator
    FILES,           // -f --files     list of files
    ARTISTS,         // -a --artists   list of artists
    TOTAL,           // --total        total flag
    REAR,            // --rear -r      rear flag
    UNKNOWN
};



int main(int argc, char* argv[]){
    if(argc < 2) {
        std::cout<<"No arguments provided. Use --help for usage information.\n";
        return 0;
    }
    std::string separator = " - ";
    std::string directoryPath = fs::current_path().string();
    std::vector<std::string> files;
    std::vector<std::string> artists;
    bool totalFlag = false;
    bool rearFlag = false;

    for(int i=1; i<argc; ++i){
        ArgumentType argType;
        if(argv[i][0] != '-'){
            std::cout<<"Unknown argument: "<<argv[i]<<"\n";
            return 0;
        }else if (argv[i] == std::string("-h") || argv[i] == std::string("--help")) {
            argType = HELP;
        } else if (argv[i] == std::string("-d") || argv[i] == std::string("--dir")) {
            argType = DIR;
        } else if (argv[i] == std::string("-s") || argv[i] == std::string("--sep")) {
            argType = SEP;
        } else if (argv[i] == std::string("-f") || argv[i] == std::string("--files")) {
            argType = FILES;
        } else if (argv[i] == std::string("-a") || argv[i] == std::string("--artists")) {
            argType = ARTISTS;
        } else if (argv[i] == std::string("--total")) {
            argType = TOTAL;
        } else if (argv[i] == std::string("--rear") || argv[i] == std::string("-r")) {
            argType = REAR;
        } else {
            argType = UNKNOWN;
        }




        switch (argType){
            case HELP:
                displayHelp();
                return 0;
            case DIR:
                if(argc == i + 1){
                    std::cout<<"No directory path provided.";
                    return 0;
                }
                if(fs::exists(argv[i+1]) && fs::is_directory(argv[i+1])){
                    directoryPath = argv[i+1];
                    ++i;
                } else {
                    std::cout<<"Invalid directory path provided.";
                    return 0;
                }
                break;
            case SEP:
                if(argc == i + 1){
                    std::cout<<"No separator provided.";
                    return 0;
                }
                separator = argv[i+1];
                ++i;
                break;
            case FILES:
                if(totalFlag){
                    std::cout<<"--files option is incompatible with --total flag.";
                    return 0;
                }
                if(argc == i + 1){
                    std::cout<<"No files provided.";
                    return 0;
                }
                while(i + 1 < argc && std::string(argv[i+1]).rfind("-", 0) != 0){
                    files.push_back(argv[i+1]);
                    ++i;
                }
                break;
            case ARTISTS:
                if(totalFlag){
                    std::cout<<"--artists option is incompatible with --total flag.";
                    return 0;
                }
                if(argc == i + 1){
                    std::cout<<"No artists provided.";
                    return 0;
                }
                while(i + 1 < argc && std::string(argv[i+1]).rfind("-", 0) != 0){
                    artists.push_back(argv[i+1]);
                    ++i;
                }
                break;
            case TOTAL:
                //see if user wants total or not, but also check for incompatible options
                if(!files.empty() || !artists.empty()){
                    std::cout<<"--total flag is incompatible with --files and --artists options.";
                    return 0;
                }
                totalFlag = true;
                break;
            case REAR:
                rearFlag = true;
                break;
            default:
                std::cout<<"Unknown argument: "<<argv[i]<<"\n";
                return 0;
                break;
        }
    }

    if(totalFlag)
        addTotal(files, directoryPath, separator,rearFlag);
    else
        addArtistsToTitles(files, artists, directoryPath, separator,rearFlag);
    
}