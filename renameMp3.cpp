#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>
namespace fs = std::filesystem;
void renameFiles(const std::string& directoryPath,const std::string& whatToRemove) {
    int count = 0;
    int failed = 0;
    try {
        for (const auto& entry : fs::directory_iterator(directoryPath)) {
            if (fs::is_regular_file(entry.status())
             && entry.path().extension() == ".mp3" 
             && entry.path().filename().string().find(whatToRemove) != std::string::npos
             && fs::exists(entry.path())
            ) {
                count++;
                std::string original = entry.path().filename().string();
                std::cout << "Old: \"" << original << "\"" << std::endl;
                int poz =  original.find(whatToRemove);
                std::string pre = original.substr(0, poz);
                std::string post = original.substr(poz + whatToRemove.length());
                std::string newName = pre+post;
                std::cout << "New: \"" << newName << "\"" << std::endl;
                fs::rename(entry.path(), entry.path().parent_path() / newName);
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error accessing directory: " << e.what() << std::endl;
        failed++;
    }
    std::cout << "Renaming completed.\n"<<"Total files renamed: " << count << std::endl;
    std::cout << "Failed operations: " << failed << std::endl;
}


void displayHelp(){
    std::cout<<"Usage: ./renameMp3 [-d directory] [<what to remove>]\n";
    std::cout<<"    -h --help : display this help message\n";
    std::cout<<"    -d --directory : specify the directory containing mp3 files (default is current directory)\n";
    std::cout<<"    <what to remove> : specify the strings to remove from mp3 filenames\n\n";
    std::cout<<"Example: \n./renameMp3 -d ../SweetSorrow 'SpotiDown - ' Youtube 'Michael Jackson'\n";
    std::cout<<"This will remove each of those 3 specified strings from all song in the directory:\n";
    std::cout<<"1: 'Spotidown - ' , 2: Youtube , 3: 'Michael Jackson' \n";
}

int main(int argc, char* argv[]) {
    std::string directoryPath = fs::current_path().string();
    if (argc == 1) {
       displayHelp();
       return 0;
    }

    for(int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--help" || std::string(argv[i]) == "-h") {
            displayHelp();
            return 0;
        } 
    }

    if(std::string(argv[1]) == "-d" || std::string(argv[1]) == "--directory") {

        if(argc < 3) {
            std::cout << "Error: directory flag specified but no directory path given." << std::endl;
            return 0;
        }

        if(!(fs::exists(argv[2]) && fs::is_directory(argv[2]))) {
            std::cout << "Error: Specified path is not a valid directory." << std::endl;
            return 0;
        }
      
        if(argc < 4) {
            std::cout << "Error: No strings specified to remove. See [what to remove]" << std::endl;
            return 0;
        }

        directoryPath = argv[2];

        for(int i = 3; i< argc; ++i) {
            renameFiles(directoryPath, argv[i]);
            return 0;
        }

    }
    for(int i = 1; i< argc; ++i) {
        renameFiles(directoryPath, argv[i]);
    }


    return 0;
}
