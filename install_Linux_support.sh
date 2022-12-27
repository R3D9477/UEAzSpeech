#!/bin/bash

CURR_DIR="$(dirname "$(realpath -s "$0")")"

pushd "${CURR_DIR}"

    find . -type f -name "*.cpp" -print0 | xargs -0 sed -i '' -e 's/PlatformFileManager/PlatformFilemanager/g'

    sed -i 's/\t\"Win64\"/\t\"Linux\", \"Win64\"/g' "AzSpeech.uplugin"

    # Debian/Ubuntu based
    sudo apt install build-essential libssl-dev libasound2 wget
    wget -O "SpeechSDK-Linux.tar.gz" "https://aka.ms/csspeech/linuxbinary"
    mkdir "Source/ThirdParty/AzureWrapper/libs/Linux"
    tar --strip 1 -xzf "SpeechSDK-Linux.tar.gz" -C "Source/ThirdParty/AzureWrapper/libs/Linux"
    rm "SpeechSDK-Linux.tar.gz"

    patch "Source/ThirdParty/AzureWrapper/AzureWrapper.Build.cs" < "AzureWrapper.Build.diff"

popd
