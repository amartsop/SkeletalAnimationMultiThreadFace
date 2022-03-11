# Skeletal Animation for Three-finger Exoskeleton and Face Landmarking
This application combines our
[Skeletal Animation application](https://github.com/amartsop/SkeletalAnimationMultiThread) for the real-time
rendering of the [three-finger exoskeleton](https://github.com/amartsop/Exoskeleton) with real-time face landmarking and rendering.
For this, we have used the MediaPipe library, which empleys machine learning
algorithms for identifying 468 unique points that are scattered across the users face. The points are
generated in real-time using a camera stream and they are sent to our rendering application (LibIGL) through means of interprocess communication.

<img src="./images/hands_face.svg" width="100%"/>

For more information on the software please see the reference document 
[reference document](https://github.com/amartsop/ExoskeletonHandover/blob/master/main.pdf), while for the full documentation please visit the [API referece](https://amartsop.github.io/SkeletalAnimationMultiThreadFace/index.html).

## Installation
1. Git clone the repository.

2. Install the appropriate dependencies. This can be done manually or with the help of the inculded installation bash script [.installlation.sh](./.installation.sh). If the user wants to install dependencies
manually, the full list of them is included in the provided [.installlation.sh](./.installation.sh) file. To execute the installation file:
    1.  Login as root and navigate to the directory where the installation file is located.
    2. Run 
    ```
    $ source ./.installation.sh 
    ```
    > **Warning**: This will install quite a few libraries and packages in your computer. Please make sure that you are aware of the packages that are installed and with the location in which they are installed. Both settings can be changed by editing the [.installlation.sh](./.installation.sh) file.

3.  Build the software: 
    1. Navigate to the home location of the directory you have cloned.
    2. Generate a build directory and navigate to it:
    ```
    $ mkdir build && cd build 
    ```
    3. Execute the CMake file:
    ```
    $ cmake ..
    ```
    4. Build the executable:
    ```
    $ make -j4
    ```
    > **Note**: Using the command ”-j4” we ask the compiler to use 4 threads for build the executable. The user can use as many as they prefer. The first time that you build the executable, LibIGL fetches its files from github and builds them as a static library. Since LigIGL is a quite heavy library this will take a while. However, all the following build commands will not build the LibIGL again but only the files that the user has added. If you want to update your build directory we recommend deleting the ”CMakeCache.txt” file in it instead of deleting the whole build directory (as this will mean that the user will have To build LibIGL from scratch).