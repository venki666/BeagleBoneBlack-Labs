# Installing Angstrom Image on BBB
Whether you have a BeagleBone or BeagleBone Black, you can use the Angstrom-Cloud9-IDE-GNOME-eglibc-ipk???.img.xz. Download the latest image form [here][1]. They both run ARM7 and the image has the NEON optimization.
## How to Unpack and Boot the Demo Image - easy way
- Download an img.gz or img.xz file from above e.g. Angstrom-Cloud9-IDE-eglibc-ipk-v2011.10-core-beaglebone-r0.img.gz (or a more recent version).
- Unpack the image to the raw BeagleBone/BBB SD card using the below commands. NOTE: superuser privileges are required when unpacking the image. Also, an SD card reader installed in the linux system running on VMware is needed. If your computer has a built in SD card reader/writer use it by installing the VMware drivers. Else, get an external USB SD card reader/writer and install the USB drivers for linux (you would not need one).
Note that all images with *eMMC* file names are flasher images that will flash the eMMC. All images with *ipk* file names are run from the SD card.
### In Linux:
~
$ sudo -s
(type in your password)
> zcat Angstrom-Cloud9-IDE-eglibc-ipk-v2011.10-core-beaglebone-r0.img.gz \> /dev/sdX
> exit
Or for the img.xz:
$ sudo -s
(type in your password)
> xz -dkc Angstrom-Cloud9-IDE-eglibc-ipk-v2011.10-core-beaglebone-r0.img.xz \> /dev/sdX # exit
~
PS: The SD card location of your device should be replacing the term /dev/sdX
---- 
## How to Unpack and Boot the Demo Image - the hard way 
(only follow this if you have problems with the pervious method - Linux OS needed)

- Format the SD card using mkcard.txt. For example: sh mkcard.txt /dev/sdX, where X is the drive letter of the SD card. On systems like Ubuntu that would look like 'sudo sh mkcard.txt /dev/sdX'.
- Copy MLO, u-boot.img from http://www.angstrom-distribution.org/demo/beaglebone/ to the first partition
- Unpack the tarball to the root partition of your BeagleBone SD card. NOTE: superuser privileges are required when unpacking the image so that device nodes can be created on the SD card filesystem.

### In Linux:
~
$ sudo tar -xjv -C /media/rootfs -f /path/to/Angstrom-BeagleBone-demo-image??rootfs.tar.bz2 
~
This assumes that the SD card has the root filesystem (ext3) partition mounted as /media/rootfs.
Ensure all SD card filesystem operations have completed (ie. filesystem cache has flushed to SD card) and eject the SD card from your development machine. Most operating systems have a "Safely Remove" action to perform this from the Desktop.
Insert SD card into BeagleBone and power it up.

### In Windows:
- Download the image file as above use eMMC or ipk files
Unzip the file you downloaded to a directory on your PC
- Download the DiskImager software to copy the unzipped image file to the microSD card from the directory on your PC where you unzipped the file.
- Unzip the above downloaded software into a folder on the PC. If you choose, you can use the same folder as the program to be flashed is in.
- Run the WinDiskImager application. A small window will appear.
- Select the drive number that corresponds to the SD card reader you plugged in by clicking on the Device box with a letter in it. MAKE SURE YOU SELECT THE CORRECT DRIVE LETTER.
- Select the file from the directory that you unzipped the new image to by clicking on the folder icon next to the Device box.
- Select the write option to start the writing process.
- When finished, remove the microSD card from the reader. Insert the newly created microSD card into the board.

## Burn SD card image to on-board eMMC:
### Boot your board off of the SD card
- Insert SD card into your (powered-down) board, hold switch S2 (Boot Switch) down by pressing on it and holding it while plugging in the power cable. Continue to hold the button until the first User LED comes on.
- If using BeagleBone Black and the image is meant to program your on-board eMMC, you'll need to wait while the programming occurs. When the flashing is complete, all 4 USRx LEDs will be lit solid.
- Note: This can take up to 45 minutes. Power-down your board, remove the SD card and apply power again to be complete.



[1]:	http://downloads.angstrom-distribution.org/demo/beaglebone/