#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>
namespace fs = std::filesystem;
void renameFiles(const std::string& directoryPath,const std::string& whatToRemove) {
    int count = 0;
    try {
        for (const auto& entry : fs::directory_iterator(directoryPath)) {
            if (fs::is_regular_file(entry.status())
             && entry.path().extension() == ".mp3" 
             && entry.path().filename().string().find(whatToRemove) != std::string::npos
             && fs::exists(entry.path())
            ) {
                count++;
                std::cout << entry.path().filename().string() << std::endl;
                fs::remove(entry.path()); 
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error accessing directory: " << e.what() << std::endl;
    }
    std::cout << "Deletion completed for " << whatToRemove << ". Total files deleted: " << count << std::endl;
}
int main(int argc, char* argv[]) {
    std::string directoryPath = ".";
    if (argc == 1) {
        std::string defaultRemove = "Instrumental";
        std::cout<<"Usage: ./deleteMp3 [<what to remove>]\n";
        std::cout<<"Example: ./deleteMp3 Tzanca Parody \"Michael Jackson\"\n";
        std::cout<<"This will delete the mp3's in the current directory.\n";
        std::cout<<"Defaulting to : \""<<defaultRemove<<"\"\n";
        renameFiles(directoryPath, defaultRemove);
    }

    for(int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--help" || std::string(argv[i]) == "-h") {
            std::cout << "Usage:\n./deleteMp3 [<what to delete>]\n";
            return 0;
        } 
    }

    for(int i = 1; i< argc; ++i) {
        renameFiles(directoryPath, argv[i]);
    }


    return 0;
}
