#!/bin/bash
# build-ubuntu.sh
# Install dependencies, downloads the EverGreenCoin source code, compile, and executes as current user (or sudo for root) on Ubuntu as of Ubuntu 18.04.3 LTS
# Copyright EverGreenCoin Developers
# https://github.com/EverGreenCoinDev/EverGreenCoin/build-ubuntu.sh
#
# Prompts to build one of the following: An EverGreenCoin Daemon Command Line Interface (CLI, advanced) or an EverGreenCoin Qt Graphical User Interface (GUI, easiest). 
# If your using a desktop or laptop, VM or otherwise, you most-likely will want the Graphical User Interface (EverGreenCoin-qt) and it's easier to use.
# If your using a VPS or server, you most-likely want a Command Line Interface (evergreencoind). This is more advanced to use.
#
# To open a shell terminal in Ubuntu desktop press "CTRL + ALT + T" at the same time on your keyboard. 
# Input or copy and paste the following command line (excluding the "#" at the beginning) to get this bash script and execute it:
# bash <(wget -qO- https://raw.githubusercontent.com/EverGreenCoinDev/EverGreenCoin/master/build-ubuntu.sh)
#
# Or if you already have this file, execute "chmod +x build-ubuntu.sh" on the file to give it executable permissions. Then "./build-ubuntu.sh" to execute it.
#
# After this script exits, you can delete the ~/EverGreenCoin directory if you wish. The resulting binary is copied to /usr/local/bin and ~/.evergreencoin is created for the user, containing the irreplaceable wallet.dat file.

echo "build-ubuntu.sh - An EverGreenCoin jumpstart bash script for Ubuntu!"
echo "==============="
echo "This executes all the steps of installing all dependencies, downloading the source code, compiling it, copying the resulting executable to /usr/local/bin, and launches the EverGreenCoin wallet (either Daemon CLI or Qt GUI) as the current user or sudo for root."
echo "You'll be prompted for your super user password to install dependencies and copy the executable. Please enter the password when prompted."

read -p "Choose EverGreenCoin Qt (Graphical User Interface (GUI) wallet) or EverGreenCoin Daemon (shell Command Line Interface (CLI) wallet) 
(press 'G' for Qt GUI or press 'C' for Daemon CLI then enter)? " answer
case ${answer:0:1} in
    g|G )
        sudo apt-get update
        sudo apt-get install build-essential -y
        sudo apt-get install libssl-dev -y
        sudo apt-get install libdb++-dev -y
        sudo apt-get install libboost-all-dev -y
        sudo apt-get install libminiupnpc-dev -y
        sudo apt-get install libqrencode-dev -y
        sudo apt-get install qt5-default -y
        sudo apt-get install qttools5-dev-tools -y
        sudo apt-get install git -y
        cd ~
        git clone https://github.com/EverGreenCoinDev/EverGreenCoin
        cd EverGreenCoin
        qmake
        make
        strip EverGreenCoin-qt
        sudo cp ~/EverGreenCoin/EverGreenCoin-qt /usr/local/bin
        if
           EverGreenCoin-qt &
        then
           echo "Your GUI EverGreenCoin wallet (EverGreenCoin-qt) has been started! The binary was copied to /usr/local/bin and your irreplaceable wallet.dat is stored in your ~/.evergreencoin directory. You can safely delete the created ~/EverGreenCoin directory if you wish. Please visit us at EverGreenCoin.org!"
        else
           echo "Sorry, something failed. Correct the error(s) above, starting with the first error output."
        fi
    ;;
    c|C )
        sudo apt-get update
        sudo apt-get install build-essential -y
        sudo apt-get install libssl-dev -y
        sudo apt-get install libdb++-dev -y
        sudo apt-get install libboost-all-dev -y
        sudo apt-get install libminiupnpc-dev -y
        sudo apt-get install git -y
        cd ~
        git clone https://github.com/EverGreenCoinDev/EverGreenCoin
        cd EverGreenCoin/src/
        make -f makefile.unix
        strip evergreencoind
        mkdir ~/.evergreencoin
        echo rpcuser=TrickyUsernameevergreencoinEGCrpc > ~/.evergreencoin/evergreencoin.conf
        echo rpcpassword=FancyPasswordxvcbxcvhR6fLvtjpMkcQ28D6npwi7 >> ~/.evergreencoin/evergreencoin.conf
        echo server=1 >> ~/.evergreencoin/evergreencoin.conf
        echo daemon=1 >> ~/.evergreencoin/evergreencoin.conf
        echo listen=1 >> ~/.evergreencoin/evergreencoin.conf
        sudo cp ~/EverGreenCoin/src/evergreencoind /usr/local/bin
        if
           evergreencoind &
        then
           echo "Your CLI EverGreenCoin wallet (evergreencoind) has been started! The binary was copied to /usr/local/bin and your irreplaceable wallet.dat is stored in your ~/.evergreencoin directory. You can safely delete the created ~/EverGreenCoin directory if you wish. Please visit us at EverGreenCoin.org!"
        else
           echo "Sorry, something failed. Correct the error(s) above, starting with the first error output."
        fi
    ;;
    * )
        echo Exit
    ;;
esac
