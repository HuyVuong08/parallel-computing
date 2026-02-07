### MPI Template for CS 4170/5170 ###
This is a basic template for MPI in CS 4170/5170

## Starting your own Repo ##

Follow these steps:

1. Create your appropriately named repo on Gitlab.

2. On your local computer: 
    - if using SSH, run  `git clone -b Template --single-branch git@gitlab.com:cs4540-spring-2019/cs-4170-5170-fall-2025/mpi.git <DIRECTORY_NAME>`. 


    - If using HTTPS, run `git clone -b Template --single-branch git@gitlab.com:cs4540-spring-2019/cs-4170-5170-fall-2025/mpi.git <DIRECTORY_NAME>`

3. `cd <DIRECTORY_NAME>`

4. `git remote remove origin`. This removes the current remote named `origin`.

5. `git remote add origin <URL_OF_YOUR_REPO>`. This adds a new remote that has the address of your repo.

6. `git branch -m master`. This changes the name of the current branch (mpi) to master.

6. `git push -u origin master`. This pushes your changes.

## Running ##
To compile and run from command line if you are not on windows:
```
cd src
mpicxx main.cpp CStopWatch.cpp
./a.out
```
or
```
cd Default && make all
```

To compile and run with Docker issue the following commands from the root of the project (not in the Default folder):
```
docker run --rm -v ${PWD}:/tmp -w /tmp/Default rgreen13/alpine-mpi-boost make all
docker run --rm -v ${PWD}:/tmp -w /tmp/Default rgreen13/alpine-mpi-boost mpiexec --allow-run-as-root -n X MPI
```
where `X` is the number of nodes

## Using OSC ##
Move all the files to the Ohio Supercomputing Center (OSC) server of your choice. Make sure to build your code, using `make OSC` and then modify the `jobScript.slurm` accordingly. Submit from inside the `Default` directory using 
```
sbatch jobScript.slurm
```

You may also do this using http://ondemand.osc.edu
