#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <boost/process.hpp>
#include <filesystem>

#include "./include/rendering.h"

int main()
{
    // Process cmd
    auto process_cmd = boost::process::search_path("python3").string();

    // Process name
    std::string process_name = (std::filesystem::current_path() /
        "face_mesh_server.py").string();
    
    // Begin forking
    int fork_id = fork();
    
    // Initialize child errors
    int err;

    if (fork_id == 0)
    {
        // Execute child process
        err = execlp(process_cmd.c_str(), process_cmd.c_str(), process_name.data(), NULL);
        if (err == -1)
        {
            std::cout << "Could not execute python server" << std::endl;
        }
    }
    else
    {
        // Sleep for a bit to make sure that the server has woken up
        sleep(1);

        // Initialize cpp client and render in the parent process
        Rendering rendering; 

        wait(NULL);
    }
}
