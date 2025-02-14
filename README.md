# LFDEM-install
> ### Instructions to compile the Lubricated flow discrete element method (LF-DEM) code.

LF-DEM is a code written by Romain Mari and Ryohei Seto (Mari et al. 2024, https://doi.org/10.1122/1.4890747). <br>
LF-DEM simulates dense, overdamped, neutrally boyant suspensions with spherical, bidisperse particles under simple shear. Information on rheological and particle interation can be obtained from the LF-DEM code.<br>
Here I mention the LF-DEM installation and bebugging steps for _personal linux, personal mac, linux cluster systems_ and _AWS EC2 instances_.

### Compiling LF DEM

**Note:** The LF-DEM open-source code is available at the author's [Bitbucket repository](https://bitbucket.org/rmari/lf_dem.git). Access may require a request.  

However, it is **recommended** to pull from the current repository, which includes:  
- Custom modifications for compatibility across multiple operating systems.  
- A key algorithmic update:  
  - The event handler now identifies shear jamming events based on a **relative** shear rate threshold instead of an **absolute** shear rate threshold.  


## A. Personal Linux Machine

1. Download the LF-DEM code (`lf_dem` directory), by either retrieving the archive [lfdem.zip](https://github.com/rahul-pandare/LFDEM-install/blob/main/lfdem.zip) or cloning the repository:  
    ```bash
    $ git clone https://github.com/rahul-pandare/LFDEM-install.git
    ```

2. Download SuiteSparse 5.4.0 from [suitesparse-5.4.0](https://people.engr.tamu.edu/davis/SuiteSparse/index.html)
   (Note: do not use the latest version. Use the version mentioned above) or use the following command in terminal:
    ```bash
    $ wget https://people.engr.tamu.edu/davis/SuiteSparse/SuiteSparse-5.4.0.tar.gz
    ```

3. Go to the download directory and unzip the `lfdem.zip` and `SuiteSparse.tar.gz` files.

4. Go to: SuiteSparse/SuiteSparse_config and open SuiteSparse_config.mk - Edit this file. Update the CC and CXX compilers to gcc and g++ (respectively) instead of icc and icpc.

5. Install `make`, `cmake`, `g++`, `gcc`, `openblas`, and `lapack` (if not already installed).
   ```bash
   $ sudo apt update
   $ sudo apt install -y make cmake g++ gcc libopenblas-dev liblapack-dev
   ```

7. From the SuiteSparse folder in terminal, run:
    ```bash
    $ make config
    $ make
    $ make install
    ```
   **NOTE:** After `$ make config`, check if the compilers and flags are properly linked.

8. Make a folder named `opt` in the home directory.

9. Open and edit the LF-DEM config file: go to LF DEM folder/LF_DEM/config, open Makefile_config_Rahul_linux.mk (update the name of the file as you wish). Update the following:
   - `install_dir = path/to/opt` (directory created earlier)
   -  Compilers must be g++ and gcc
   - `SUITESPARSE_ROOT = path/to/suitesparse/folder`
   - `CXXFLAGS_EXTRA = -DGIT_VERSION="\"42ce875e-dirty\""`
   If the config file is not available at the location, edit the generic config file.

10. Update Makefile: go to LF DEM folder/LF_DEM and edit `Makefile`. Update the name of the config file to `makeconfig = config/Makefile_config_Rahul_linux.mk`. 

11. From the LF_DEM folder. Run:
    ```bash
    $ make
    $ make install
    ```
   **LF DEM is ready**

### Debugging:

1. **Issues while installing SuiteSparse (step 6)**
   - After `$ make config` command, check the output. The CC and CXX compilers must be gcc (or cc) and g++ respectively. Check if openblas and lapack flags are linked.
   - `$ make` is prone to see more errors. While trying to remake the SuiteSparse library, do:
     ```bash
     $ make clean
     $ make purge
     $ make
     ```
   - After the `$ make` command, we usually see some error regarding no make targets found in some directories. That should be fine, and we should still be able to install SuiteSparse.
   - While making SuiteSparse, we might encounter errors in certain libraries or directories. You can `cd` into the specific directory and tackle that directory itself.
   - One of the more common errors can occur due to the absence of gcc, g++, cmake, openblas, or lapack. Make sure you have all of them.
   - While performing reinstallation of SuiteSparse, in case `$ make install` spits out errors, do:
     ```bash
     $ make uninstall
     $ make install
     ```

2. **Issues while installing LF-DEM (step 9)**
   - This step is usually straightforward and not prone to much error. While remaking, do:
     ```bash
     $ make clean
     $ make
     ```

## B. Linux Cluster
1. Use of 'sshfs' on your personal machine is strongly recommended while working with clusters. It makes it easier to transfer small files accross from you machine to the cluster and vise versa with ease.
For personal linux machines you can do:
    ```bash
    $ sudo apt update
    $ sudo apt install sshfs
    ```
For personal mac you need to download macFUSE: [https://osxfuse.github.io/](https://osxfuse.github.io/)

2. Download the LF-DEM code (`lf_dem` directory) on your personal machine, by either retrieving the archive [lfdem.zip](https://github.com/rahul-pandare/LFDEM-install/blob/main/lfdem.zip) or cloning the repository:  
    ```bash
    $ git clone https://github.com/rahul-pandare/LFDEM-install.git
    ```

3. Download SuiteSparse 5.4.0 from [suitesparse-5.4.0](https://people.engr.tamu.edu/davis/SuiteSparse/index.html)
   (Note: do not use the latest version. Use the version mentioned above) or use the following command in terminal:
    ```bash
    $ wget https://people.engr.tamu.edu/davis/SuiteSparse/SuiteSparse-5.4.0.tar.gz
    ```
4. Copy the `lfdem.zip` and `SuiteSparse.tar.gz` files on the cluster.

5. Log into the cluster and unzip the `lfdem.zip` and `SuiteSparse.tar.gz` files.

6. Go to: SuiteSparse/SuiteSparse_config and open SuiteSparse_config.mk - Edit this file. Update the CC and CXX compilers to gcc and g++ (respectively) instead of icc and icpc.

7. In order to install Suitesparse and LF-DEM we require cmake, `g++`, `gcc`, `openblas` and `lapack`. We cannot install them on a cluster due to restrictions. We look for `gcc`, `cmake`, `intel` and `intel-mkl`. The `intel` modules have openblas and lapack inbuilt and it is optimized.
    ```bash
    $ module load gcc cmake intel intel-mkl
    ```
If case intel libraries are not present we look for `openblas` and `lapack`.
    ```bash
    $ module load gcc cmake openblas lapack
    ```

8. From the SuiteSparse folder terminal, run:
    ```bash
    $ make config
    $ make
    $ make install
    ```
   **NOTE:** After `make config`, check if the compilers and flags are properly linked.

9. Make a folder named `opt` in the home directory.

10. Open and edit the LF-DEM config file: go to LF DEM folder/LF_DEM/config, open Makefile_config_Rahul_linux.mk (update the name of the file as you wish). Update the following:
   - `install_dir = path/to/opt` (directory created earlier)
   -  Compilers ideally should be g++ and gcc (icpx and icx could work too depending on the linked modules) 
   - `SUITESPARSE_ROOT = path/to/suitesparse/folder`
   - `CXXFLAGS_EXTRA = -DGIT_VERSION="\"42ce875e-dirty\""`
   If the config file is not available at the location, edit the generic config file.

11. Update Makefile: go to LF DEM folder/LF_DEM and edit `Makefile`. Update the name of the config file to `makeconfig = config/Makefile_config_Rahul_linux.mk`. 

12. Open terminal from the LF_DEM folder. Run:
    ```bash
    $ make
    $ make install
    ```
   **LF DEM is ready**

### Debugging:

1. **Issues while installing SuiteSparse (step 6)**
   - After `$ make config` command, check the output. The CC and CXX compilers must be gcc (or cc) and g++ respectively. Check if `openblas` and `lapack` flags are linked.
   - `$ make` is prone to see more errors. While trying to remake the SuiteSparse library, do:
     ```bash
     $ make clean
     $ make purge
     $ make
     ```
   - After the `make` command, we usually see some error regarding no make targets found in some directories. That should be fine, and we should still be able to install SuiteSparse.
   - While making SuiteSparse, we might encounter errors in certain libraries or directories. You can `cd` into the specific directory and remake that directory itself to tackle errors.
   - One of the more common errors can occur due to the absence of gcc, g++, cmake, openblas, or lapack. Make sure you have all of them.
   - While performing reinstallation of SuiteSparse, in case `$ make install` spits out errors, do:
     ```bash
     $ make uninstall
     $ make install
     ```

2. **Issues while installing LF-DEM (step 9)**
   - This step is usually straightforward and not prone to much error. While remaking, do:
     ```bash
     $ make clean
     $ make
     ```

## C. Personal MacOS (M1 and M2) machine
1. Download the LF-DEM code (`lf_dem` directory) on your personal machine, by either retrieving the archive [lfdem.zip](https://github.com/rahul-pandare/LFDEM-install/blob/main/lfdem.zip) or cloning the repository:  
    ```bash
    $ git clone https://github.com/rahul-pandare/LFDEM-install.git
    ```
2. Install GCC, Clang and SuiteSparse:
   ```bash
    $ brew install gcc llvm suite-sparse
   ```
   Here, GCC and Clang both are C compilers. We install both incase one fails.
   
3. Make a folder named `opt` in the home directory.
   
4. Open and edit the LF-DEM config file: go to LF DEM folder/LF_DEM/config, open [Makefile_config_Rahul_m1.mk](https://github.com/rahul-pandare/LFDEM-install/blob/main/Makefile_config_Rahul_m1.mk) or [Makefile_config_Rahul_m2.mk](https://github.com/rahul-pandare/LFDEM-install/blob/main/Makefile_config_Rahul_m2.mk) (update the name of the file as you wish). Update the following (if not already updated):
   - `install_dir = path/to/opt` (directory created earlier)
   - Compilers must be g++ and gcc
   - `SUITESPARSE_ROOT = path/to/suitesparse/folder`
   - `CXXFLAGS_EXTRA = -DGIT_VERSION="\"42ce875e-dirty\""`
   If the config file is not available at the location, edit the generic config file.
**Note**: Different flags are used for M1 and M2 makefiles.

5. Update Makefile: go to LF DEM folder/LF_DEM and edit `Makefile`. Under 'include paths' update the cholmod path and set it to `Cholmod_path = -I $(SUITESPARSE_ROOT)/include/suitesparse/`. Next, update the name of the config file to `makeconfig = config/Makefile_config_Rahul_m1.mk` (or the updated file name).

6. Open terminal from the LF_DEM folder. Run:
    ```bash
    $ make
    $ make install
    ```
   **LF DEM is ready**
   
### Debugging:

1. **Issues while installing SuiteSparse (step 6)**
   - Installing Suitesparse is usually straightforward. Incase any error arises due to suitesparse sublibrary try uninstalling and installing it again or installing a previous version of suitesparse by mentioning the version of the suitesparse while installing.
     ```bash
     $ brew install suite-sparse 7.8.2 # Older version
     ```

2. **Issues while installing LF-DEM (step 9)**
   - This step is very prone to errors. Erros usually occur for C++ compilers, BLAS or LAPACK linking flags.
   - In case of compiler error. Mention the path for g++ and gcc (clang and clang++)
   - For other errors, looking up online forums for linking flags would be the best bet to make it work.
   - The config files alread mentioned have the necessary flags which copiled the code but which may not be the case for someone trying to install later with updated versions of libraries or different Apple processor.

## D. AWS (EC2 instance)
Installation on AWS VM is similar as on personal linux machine.
1. Make sure the AWS EC2 architecture uses ubuntu as OS and uses 64-bit x86 and not ARM. LF_DEM code compiles only on the x86 architecture.

2. LF-DEM installation is similar to that as on a personal linux machine. Download the open source LF-DEM code from [lfdem.zip](https://github.com/rahul-pandare/LFDEM-install/blob/main/lfdem.zip) or use the following command in terminal:
    ```bash
    $ git clone https://bitbucket.org/rmari/lf_dem.git
    ```

3. Download SuiteSparse 5.4.0 from [suitesparse-5.4.0](https://people.engr.tamu.edu/davis/SuiteSparse/index.html)
   (Note: do not use the latest version. Use the version mentioned above) or use the following command in terminal:
    ```bash
    $ wget https://people.engr.tamu.edu/davis/SuiteSparse/SuiteSparse-5.4.0.tar.gz
    ```
4. One could also upload the required files onto the AWS VM. Download the [putFiles.sh](https://github.com/rahul-pandare/LFDEM-install/blob/main/putFiles.sh) file. Run the putFile.sh to upload the required files on the AWS instance.

5. Download and run the [LF_DEM_installation_aws.sh](https://github.com/rahul-pandare/LFDEM-install/blob/main/LF_DEM_installation_aws.sh) file. This will install Suitesparse and LF-DEM onto the instance.

**LF DEM is ready**
   
### Debugging:

1. **Issues while uploading file**
   - You could upload the files manually or by using the `putFiles.sh` file. This file uses the scp command, make sure you have mentioned the correct IP and key paths. 

2. **Issues while installing LF-DEM (step 9)**
   - Often times the problem is with permisions. In case any error says `permission denied` or `cannot access directory` updated the permissions. Run the following command from home directory:
   ```bash
   $ sudo chmod -R 755 .
   ```
   
### Some additional points
1. **Cluster**
    - While my time working with LF-DEM so far I have used the [CUNY HPC](https://www.csi.cuny.edu/academics-and-research/research-centers/cuny-high-performance-computing-center), CCNY Excelsior Cluster, and via [ACCESS](https://access-ci.org/) I used [Darwin](https://docs.hpc.udel.edu/abstract/darwin/darwin) and [Anvil](https://www.rcac.purdue.edu/compute/anvil).
    - On most of the clusters installing Suitesparse is a challange due to cluster restrictions on installing libraries directly. One should try locating and adding the Lapack and openblas libraries to their environment. Using the intel and intel-mkl modules is ideal since they are faster.
    - Make sure you purge any other modules that may be present (even the sticky modules), so that they wont interfere with the necessary modules.
    - While installing LF-DEM, if one encounters compilation errors - try with different c compilers like `gcc & g++`, `icc & icpc` or `icx & icpx`

2. **AWS instance**
    - While using AWS - I used c7g, c7i, c5a, z1d. I found **c7i** to be the most efficient for the LF-DEM code.
    - To elaborate, c7g are the ARM gravitron chips. LF-DEM runs into error while compiling. c5a are the legacy older generation chips which are very slow. z1d are  fast but very expensive and the higher speed does not justify the cost hence not optimal.
    - After LF-DEM installation, while running jobs one can use the `screen` command from the terminal to make multiple ongoing terimal _screens_ and keep check on simulations.