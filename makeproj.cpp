#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <csignal>
#include <unistd.h>
// im so sorry
std::string folder, bundle_id, desc, app, app_bundle, author;

void signalHandler(int signal) {
    if (rmdir(folder.c_str()) == 0) {
        std::cout << "\nokay, bye!\n";
    } else {
        std::cout << "\noh, okay\n";
    }

    exit(signal);
}

int main() {
    // info
    char n = '\n';
    std::signal(SIGINT, signalHandler);
    std::cout << "enter the project name: ";
    std::getline(std::cin, folder);

    // creating the folder
    if (mkdir(folder.c_str(), 0764)) {
        std::cout << "couldn't create folder! it probably already exists.\n";
        return 1;
    }

    // pkg info
    std::cout << "enter package bundle id: ";
    std::getline(std::cin, bundle_id);

    std::cout << "enter package description: ";
    std::getline(std::cin, desc);

    std::cout << "enter name of the app that you tweaked: ";
    std::getline(std::cin, app);

    std::cout << "enter the bundle id of the app that you tweaked: ";
    std::getline(std::cin, app_bundle);

    std::cout << "enter the name of the tweak author and maintainer: ";
    std::getline(std::cin, author);

    // writing control
    std::string control = folder + "/control";
    std::ofstream control_file(control);

    if (control_file.is_open()) {
        control_file << "Package: " << bundle_id << n;
        control_file << "Name: " << folder << n;
        control_file << "Version: 1.0.0\nArchitecture: iphoneos-arm\n";
        control_file << "Description: " << desc << n;
        control_file << "Author: " << author << n;
        control_file << "Maintainer: " << author << n;
        control_file << "Section: Tweaks\nDepends: mobilesubstrate (>= 0.9.5000), firmware (>= 11.0)";
    }

    // writing Tweak.xm
    std::string tweakxm = folder + "/Tweak.xm";
    std::ofstream tweakxm_file(tweakxm);

    if (tweakxm_file.is_open()) {
        tweakxm_file << "#import <Foundation/Foundation.h>\n#import <UIKit/UIKit.h>\n\n\n";
    }

    // writing Makefile
    std::string makefile = folder + "/Makefile";
    std::ofstream makefile_f(makefile);

    if (makefile_f.is_open()) {
        makefile_f << "TARGET := iphone:clang:latest:10.0\n";
        makefile_f << "INSTALL_TARGET_PROCESSES = " << app << n;
        makefile_f << "ARCHS = arm64\n\ninclude $(THEOS)/makefiles/common.mk\n\n";
        makefile_f << "TWEAK_NAME = " << folder << n << n;
        makefile_f << "$(TWEAK_NAME)_FILES = Tweak.xm\n$(TWEAK_NAME)_CFLAGS = -fobjc-arc\n\ninclude $(THEOS_MAKE_PATH)/tweak.mk";
    }

    // writing plist
    std::string plist = folder + "/" + folder + ".plist";
    std::ofstream plist_file(plist);

    if (plist_file.is_open()) {
        plist_file << "{ Filter = { Bundles = ( \"" << app_bundle << "\" ); }; }";
    }

    std::cout << "finished writing all project files to " << folder << "!\n";
}
