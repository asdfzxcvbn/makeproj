import std/[os, rdstdin, strutils, strformat]

var
    name: string
    nameNS: string
    createdDir: bool = false

    # SERIOUSLYY?? NEW SCOPE FOR TRY BLOCKS????
    bundle: string
    desc: string
    app: string
    appBundle: string
    author: string

const tweakContent: string = """#import <Foundation/Foundation.h>
%config(generator=internal);


"""

proc leftEarly(): void =
    echo "oh, okay"
    if createdDir:  # YES this is required bc idk how to except a NameError like in python lol
        removeDir(name)
    quit()

try:
    name = readLineFromStdin("enter the project name: ")
    if dirExists(name):
        stderr.writeLine("that folder already exists! pick a different name next time.")
        quit(1)
    try:
        createDir(name)
        createdDir = true
    except OSError:
        stderr.writeLine("couldn't create directory. are you sure you have write permission here?")
        quit(1)

    nameNS = name.splitWhitespace.join("")
    bundle = readLineFromStdin("enter package bundle id: ")
    desc = readLineFromStdin("enter package description: ")
    app = readLineFromStdin("enter the name of the app you tweaked: ")
    appBundle = readLineFromStdin("enter the bundle id of the app you tweaked: ")
    author = readLineFromStdin("enter the name of the tweak author: ")
except IOError:
    leftEarly()

setCurrentDir(name)

# file io is surprisingly great :D
writeFile("control", fmt"""Package: {bundle}
Name: {nameNS}
Version: 1.0.0
Architecture: iphoneos-arm
Description: {desc}
Author: {author}
Maintainer: {author}
Section: Tweaks
Depends: firmware (>= 11.0)
""")

writeFile("Tweak.xm", tweakContent)

writeFile("Makefile", fmt"""TARGET := iphone:clang:latest:11.0
INSTALL_TARGET_PROCESSES = {app}
ARCHS = arm64

include $(THEOS)/makefiles/common.mk

TWEAK_NAME = {nameNS}

$(TWEAK_NAME)_FILES = Tweak.xm
$(TWEAK_NAME)_CLAGS = -fobjc-arc
$(TWEAK_NAME)_LOGOS_DEFAULT_GENERATOR = internal

include $(THEOS_MAKE_PATH)/tweak.mk
""")

writeFile(fmt"{nameNS}.plist", fmt"""{{ Filter = {{ Bundles = ( "{appBundle}" ); }}; }}
""")  # files should have line endings :D

echo fmt"wrote all files to {name}!"
