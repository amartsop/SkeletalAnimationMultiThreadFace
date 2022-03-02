#pragma once

#include <iostream>
#include <filesystem>
#include <future>
#include <eigen3/Eigen/Dense>
#include "Python.h"

#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>


#include <filesystem>
#include "./nlohmann/json.hpp"
#include "euler_rotations.h"
#include "utils.h"


class Face
{

public:
    Face();
    
    // Update face data
    void update(void);

    // Get vertices
    Eigen::MatrixXd get_vertices(void);

    // Get surface indices
    Eigen::MatrixXi get_surface_indices(void) { return m_surf_indices; } 

private:

    // Absolute name of vertices connection files
    std::filesystem::path m_vertices_connections_abs_path;

    // Relative name of vertices connection files
    std::string m_vertices_connections_rel_filename =
        "share/vertices_connections.csv";   

    // Face landmarking indices matrix
    Eigen::MatrixXi m_faces;

private:
    // Setup communication with python server 
    void setup_tcp_communication(void);

    // Start tcp client
    std::string m_local_ip_address = "127.0.1.1";

    // Tcp port for socket communication
    const int m_tcp_port = 5052;

    // Socket handle
    int m_socket = 0;

private:
    // Buffer size
    const int m_buffer_size = 40000;

    // Number of vertices
    const int m_vertices_num = 468;

    // Volume mesh surface element indices(F)
    Eigen::MatrixXi m_surf_indices;

private:
    // Future function handle
    std::future<Eigen::MatrixXd> m_future_fun;

    // Termination flag for callback function
    bool m_return_value = 0;

    // Read incoming data
    Eigen::MatrixXd incoming_data_callback(void);

    // Volume mesh data
    Eigen::MatrixXd m_vol_mesh_vertices;

    // Face rotation matrix
    Eigen::Matrix3d m_rot_mat = Eigen::Matrix3d::Identity();

    // Face scale matrix
    double m_face_mesh_scale = 3.0;
    Eigen::Matrix3d m_scale_mat =
        m_face_mesh_scale * Eigen::Matrix3d::Identity();
};
