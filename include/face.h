#pragma once

#include <iostream>
#include <filesystem>
#include <future>
#include <eigen3/Eigen/Dense>

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

/**
 * @brief This class runs the client that handles the vertices coming from 
 * the python server face_mesh_server.py to render the face landmarking.
 */
class Face
{

public:
    /// Constructor.
    Face();
    
    // Update face data.
    void update(void);

    /// Get face vertices.
    Eigen::MatrixXd get_vertices(void);

    /**
     * @brief Get the surface indices of the face mesh.
     * 
     * @return Eigen::MatrixXi The surface indices matrix.
     */
    Eigen::MatrixXi get_surface_indices(void) { return m_surf_indices; } 

    /// This function offset a set of vertices by a given offset.
    Eigen::MatrixXd translation_matrix(const Eigen::Vector3d& offset,
        size_t vert_num);

private:

    /// Absolute name of vertices connection files.
    std::filesystem::path m_vertices_connections_abs_path;

    /// Relative name of vertices connection files.
    std::string m_vertices_connections_rel_filename =
        "share/vertices_connections.csv";   

    /// Face landmarking indices matrix
    Eigen::MatrixXi m_faces;

private:
    /// Setup communication with python server.
    void setup_tcp_communication(void);

    /// Start tcp client at this ip adress (localhost).
    std::string m_local_ip_address = "127.0.1.1";

    /// Tcp port for socket communication (this might need to be changed if 
    /// it already occupied.)
    const int m_tcp_port = 5052;

    /// Socket handle.
    int m_socket = 0;

private:

    /// Buffer size for reading the vertices message. The number of vertices is 
    /// constant and this buffer size is enough.
    const int m_buffer_size = 40000;

    /// Number of vertices provided by the mediapipe. This is constant.
    const int m_vertices_num = 468;

    /// The surface element indices of the incoming mesh.
    Eigen::MatrixXi m_surf_indices;

private:
    /// Future function handle.
    std::future<Eigen::MatrixXd> m_future_fun;

    /// Termination flag for callback function.
    bool m_return_value = 0;

    /// Read incoming data.
    Eigen::MatrixXd incoming_data_callback(void);

    /// Volume mesh data.
    Eigen::MatrixXd m_vol_mesh_vertices;

    /// Face rotation matrix.
    Eigen::Matrix3d m_rot_mat = Eigen::Matrix3d::Identity();

    /// Face scale matrix.
    double m_face_mesh_scale = 6.0;
    Eigen::Vector3d m_face_offset = Eigen::Vector3d({0.0, -2.0, 0.0});
    Eigen::Matrix3d m_scale_mat =
        m_face_mesh_scale * Eigen::Matrix3d::Identity();
};
