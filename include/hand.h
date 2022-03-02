#pragma once

#include <iostream>

#include "igl/opengl/glfw/Viewer.h"
#include "igl/readMESH.h"
#include "igl/readTGF.h"
#include "igl/png/readPNG.h"
#include "igl/directed_edge_parents.h"
#include "igl/boundary_conditions.h"
#include "igl/bbw.h"
#include "igl/lbs_matrix.h"
#include "igl/normalize_row_sums.h"
#include "igl/forward_kinematics.h"
#include "igl/jet.h"
#include "igl/deform_skeleton.h"

#include "exoskeleton.h"
#include "animated_hand.h"

typedef std::vector<Eigen::Quaterniond, 
    Eigen::aligned_allocator<Eigen::Quaterniond>> RotationList;


class Hand
{

public:
    Hand(Exoskeleton* exo_handler, AnimatedHand* anim_hand,
        const std::string& mesh_name, const std::string& graph_name,
        const std::string& texture_name, bool type, const Eigen::Vector3d& origin);

    struct PngImage
    {
        Eigen::Matrix<unsigned char,Eigen::Dynamic,Eigen::Dynamic> red;
        Eigen::Matrix<unsigned char,Eigen::Dynamic,Eigen::Dynamic> green;
        Eigen::Matrix<unsigned char,Eigen::Dynamic,Eigen::Dynamic> blue;
        Eigen::Matrix<unsigned char,Eigen::Dynamic,Eigen::Dynamic> a;
    };

    // Update hand
    void update(const std::vector<AnimatedHand::EulerID>& euler_id);

    // Get vertices
    Eigen::MatrixXd get_vertices(void) {return m_vol_mesh_vert; }

    // Get surface indices
    Eigen::MatrixXi get_surface_indices(void) { return m_surf_indices; } 

private:

    // Volume mesh Vertices (V)
    Eigen::MatrixXd m_vol_mesh_vert, m_vol_mesh_vert_init;

    // Volume mesh tetrahedral indices(T) and surface element indices(F)
    Eigen::MatrixXi m_tetr_indices, m_surf_indices;

    // Bone-edges skeleton indices (BE)
    Eigen::MatrixXi m_bone_edges_indices;

    // Point handles list (P)
    Eigen::VectorXi m_point_handles;

    // List of vertex positions (C)
    Eigen::MatrixXd m_vertex_pos;

    // List of boundary indices (b)
    Eigen::VectorXi m_boundary_indices;

    // List of boundary conditions of each weight function (bc)
    Eigen::MatrixXd m_boundary_conditions;

    // BBW weights matrix (bbw_data)
    igl::BBWData m_bbw_data;

   // List of weights (W)
    Eigen::MatrixXd m_weights;

    // Global weights (M)
    Eigen::MatrixXd m_g_weights;

private: 

    // Exoskeleton handler pointer
    Exoskeleton* m_exo_handler;

    // Animated hand pointer
    AnimatedHand* m_anim_hand;

    // Hand index iterator
    std::vector<int> m_hand_idx_iter;

    // Animation pose
    RotationList m_anim_pose;

    // Pose size
    int m_pose_size = 20;

    // Texture image
    PngImage m_texture;

    // Hand type 
    bool m_hand_type;

    // Hand origin
    Eigen::Vector3d m_origin;
};