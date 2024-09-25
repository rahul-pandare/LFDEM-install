# LFDEM-install
> ### Instructions to compile the Lubricated flow discrete element method (LF-DEM) code.

LF-DEM is a code written by Romain Mari and Ryohei Seto (Mari et al. 2024, https://doi.org/10.1122/1.4890747). <br>
LF-DEM simulates dense, overdamped, neutrally boyant suspensions with spherical, bidisperse particles under simple shear. Rheological and interation information can be obtained from the LF-DEM code.<br>
Here I mention the LF-DEM installation and bebugging steps for _personal linux, personal mac, linux cluster systems_ and _AWS EC2 instance_.

Compiling LF DEM

## A. Personal Linux Machine

1. Download the open source LF-DEM code from [lfdem.zip](https://github.com/rahul-pandare/LFDEM-install/blob/main/lfdem.zip) or use the following command in terminal
```bash
$ git clone https://bitbucket.org/rmari/lf_dem.git


2. Download SuiteSparse 5.4.0 from [suitesparse-5.4.0](https://people.engr.tamu.edu/davis/SuiteSparse/index.html)
  (Note: do not use the latest version. Use the version mentionedcd above)
  or use the following command in terminal
```bash
  wget https://people.engr.tamu.edu/davis/SuiteSparse/SuiteSparse-5.4.0.tar.gz

3. Go to the download directory and unzip the '''lfdem.zip''' and '''SuiteSparse.tar.gz''' files.

4. Go to: SuiteSparse/SuiteSparse_config and open SuiteSparse_config.mk - Edit this file.
Update the CC and CXX compilers to gcc and g++ (respectively) instead of icc and icpc.

5. Install make, cmake, g++, gcc, openblas and lapack (if not already installed). Look up the syntacs online to install these pacakages if need.

6. From SuiteSparse folder terminal run: '''make config''', '''make''' and then '''make install'''
NOTE: After make config check if the compilers and flags are properly linked
Make a folder named '''opt''' in the home directory

7. Open and edit the LF-DEM config file: go to LF DEM folder/LF_DEM/config open Makefile_config_Rahul_linux.mk (Update the name of the file as you wish).
update the following:
- install_dir = path/to/opt (directory created earlier)
- Compilers must be g++ and gcc
- SUITESPARSE_ROOT = path/to/suitesparse/folder
- CXXFLAGS_EXTRA = -DGIT_VERSION="\"42ce875e-dirty\""
If the config file is not available at the location, edit the generic config file.

8. Update Makefile: go to LF DEM folder/LF_DEM and edit '''Makefile'''. Update the name of the config file '''makeconfig = config/Makefile_config_Rahul_linux.mk'''. 

9. Open terminal from the LF_DEM folder. Run: '''make''' and '''make install'''.

              LF DEM is ready

Debugging:
1. Issues while installing suitesparse (step 6)
- After '''make config''' command check the output the CC and CXX compilers must be gcc (or cc) and g++ respectively. Check if openblas and lapack flags are linked.
- '''make''' is prone to see more errors. While trying to remake the suitesparse library do: '''make clean''', '''make purge''' and then '''make'''.
- After '''make''' command we usually see some error regarding no make targets found in some directories. That should be fine and we should still be able to install suitesparse.
- While making suitesparse we might encounter errors in certain libraries or directories. You can cd into the specific directory and tackle that directory itself.
- One of the more common errors can occur due to older versions or inexistance of gcc, g++, cmake, openblas or llapack. Make sure you have all of them.
- While performing reinstallation of suitesparse incase '''make install'' spits out errors do: '''make uninstall''' and then '''make install'''.

2. Issues while installing LF-DEM (step 9)
- This step is usually straight forward and not prone to much error. While remaking do: '''make clean''' and the '''make''' again

## A. Personal Linux Machine

1. Download the open source LF-DEM code from [lfdem.zip](https://github.com/rahul-pandare/LFDEM-install/blob/main/lfdem.zip) or use the following command in terminal
```bash
$ git clone https://bitbucket.org/rmari/lf_dem.git


2. Download SuiteSparse 5.4.0 from [suitesparse-5.4.0](https://people.engr.tamu.edu/davis/SuiteSparse/index.html)
  (Note: do not use the latest version. Use the version mentionedcd above)
  or use the following command in terminal
```bash
  wget https://people.engr.tamu.edu/davis/SuiteSparse/SuiteSparse-5.4.0.tar.gz

3. Go to the download directory and unzip the '''lfdem.zip''' and '''SuiteSparse.tar.gz''' files.

4. Go to: SuiteSparse/SuiteSparse_config and open SuiteSparse_config.mk - Edit this file.
Update the CC and CXX compilers to gcc and g++ (respectively) instead of icc and icpc.

5. Install make, cmake, g++, gcc, openblas and lapack (if not already installed). Look up the syntacs online to install these pacakages if need.

6. From SuiteSparse folder terminal run: '''make config''', '''make''' and then '''make install'''
NOTE: After make config check if the compilers and flags are properly linked
Make a folder named '''opt''' in the home directory

7. Open and edit the LF-DEM config file: go to LF DEM folder/LF_DEM/config open Makefile_config_Rahul_linux.mk (Update the name of the file as you wish).
update the following:
- install_dir = path/to/opt (directory created earlier)
- Compilers must be g++ and gcc
- SUITESPARSE_ROOT = path/to/suitesparse/folder
- CXXFLAGS_EXTRA = -DGIT_VERSION="\"42ce875e-dirty\""
If the config file is not available at the location, edit the generic config file.

8. Update Makefile: go to LF DEM folder/LF_DEM and edit '''Makefile'''. Update the name of the config file '''makeconfig = config/Makefile_config_Rahul_linux.mk'''. 

9. Open terminal from the LF_DEM folder. Run: '''make''' and '''make install'''.

              LF DEM is ready

Debugging:
1. Issues while installing suitesparse (step 6)
- After '''make config''' command check the output the CC and CXX compilers must be gcc (or cc) and g++ respectively. Check if openblas and lapack flags are linked.
- '''make''' is prone to see more errors. While trying to remake the suitesparse library do: '''make clean''', '''make purge''' and then '''make'''.
- After '''make''' command we usually see some error regarding no make targets found in some directories. That should be fine and we should still be able to install suitesparse.
- While making suitesparse we might encounter errors in certain libraries or directories. You can cd into the specific directory and tackle that directory itself.
- One of the more common errors can occur due to older versions or inexistance of gcc, g++, cmake, openblas or llapack. Make sure you have all of them.
- While performing reinstallation of suitesparse incase '''make install'' spits out errors do: '''make uninstall''' and then '''make install'''.

2. Issues while installing LF-DEM (step 9)
- This step is usually straight forward and not prone to much error. While remaking do: '''make clean''' and the '''make''' again

## A. Personal Linux Machine

1. Download the open source LF-DEM code from [lfdem.zip](https://github.com/rahul-pandare/LFDEM-install/blob/main/lfdem.zip) or use the following command in terminal
```bash
$ git clone https://bitbucket.org/rmari/lf_dem.git


2. Download SuiteSparse 5.4.0 from [suitesparse-5.4.0](https://people.engr.tamu.edu/davis/SuiteSparse/index.html)
  (Note: do not use the latest version. Use the version mentionedcd above)
  or use the following command in terminal
```bash
  wget https://people.engr.tamu.edu/davis/SuiteSparse/SuiteSparse-5.4.0.tar.gz

3. Go to the download directory and unzip the '''lfdem.zip''' and '''SuiteSparse.tar.gz''' files.

4. Go to: SuiteSparse/SuiteSparse_config and open SuiteSparse_config.mk - Edit this file.
Update the CC and CXX compilers to gcc and g++ (respectively) instead of icc and icpc.

5. Install make, cmake, g++, gcc, openblas and lapack (if not already installed). Look up the syntacs online to install these pacakages if need.

6. From SuiteSparse folder terminal run: '''make config''', '''make''' and then '''make install'''
NOTE: After make config check if the compilers and flags are properly linked
Make a folder named '''opt''' in the home directory

7. Open and edit the LF-DEM config file: go to LF DEM folder/LF_DEM/config open Makefile_config_Rahul_linux.mk (Update the name of the file as you wish).
update the following:
- install_dir = path/to/opt (directory created earlier)
- Compilers must be g++ and gcc
- SUITESPARSE_ROOT = path/to/suitesparse/folder
- CXXFLAGS_EXTRA = -DGIT_VERSION="\"42ce875e-dirty\""
If the config file is not available at the location, edit the generic config file.

8. Update Makefile: go to LF DEM folder/LF_DEM and edit '''Makefile'''. Update the name of the config file '''makeconfig = config/Makefile_config_Rahul_linux.mk'''. 

9. Open terminal from the LF_DEM folder. Run: '''make''' and '''make install'''.

              LF DEM is ready

Debugging:
1. Issues while installing suitesparse (step 6)
- After '''make config''' command check the output the CC and CXX compilers must be gcc (or cc) and g++ respectively. Check if openblas and lapack flags are linked.
- '''make''' is prone to see more errors. While trying to remake the suitesparse library do: '''make clean''', '''make purge''' and then '''make'''.
- After '''make''' command we usually see some error regarding no make targets found in some directories. That should be fine and we should still be able to install suitesparse.
- While making suitesparse we might encounter errors in certain libraries or directories. You can cd into the specific directory and tackle that directory itself.
- One of the more common errors can occur due to older versions or inexistance of gcc, g++, cmake, openblas or llapack. Make sure you have all of them.
- While performing reinstallation of suitesparse incase '''make install'' spits out errors do: '''make uninstall''' and then '''make install'''.

2. Issues while installing LF-DEM (step 9)
- This step is usually straight forward and not prone to much error. While remaking do: '''make clean''' and the '''make''' again

