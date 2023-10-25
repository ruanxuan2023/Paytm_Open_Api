# paytm_soundbox_sdk

## Clone and Build the Project

1. Clone the project to your local machine:

2. Navigate to the `code/sdk` directory:

3. Double-click `cmd.exe` to open a command prompt window.

4. Run `./build.bat` to start compiling the project. The compilation process will perform a series of actions, and when it's completed, the following message will be printed: 
        APP sign DONE!


## Firmware Update

For the first-time usage, follow these steps to update the firmware:

1. Run the command `download.bat -ALL`.

2. Put the device in firmware update mode (bootloader mode).

3. Wait for the following message to be printed, indicating that the firmware update is completed:
   aboot download engine stopped successfully.

## Generate new RSA key pairs
.\arelease.exe -c . -k --key-alg=rsa

## Generate efuse data(root key and Public key hash)
arelease.exe -c . -g --fuse-only -p ASR1602_EVB -v ASR1602_OPEN_04MB

## CZIP tool
czip.exe ./resource resource.czip
