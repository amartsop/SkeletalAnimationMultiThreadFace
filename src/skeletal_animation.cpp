#include "../include/skeletal_animation.h"

// Initialize skeletal animation
void SkeletalAnimation::initialize(igl::opengl::glfw::Viewer* viewer, 
    Exoskeleton* left_exo, AnimatedHand* anim_hand, MenuHandler* menu_handler)
{
    // Get exoskeleton handler pointer
    m_left_exo = left_exo;

    // Get animation hand pointer
    m_anim_hand = anim_hand;

    // Get menu handler pointer
    m_menu_handler = menu_handler;
}

// Main animation loop
bool SkeletalAnimation::animation_loop(igl::opengl::glfw::Viewer& viewer)
{
    if (viewer.core().is_animating)
    {
        // Initialize if ports are set
        if (m_menu_handler->are_ports_set() && m_initialize_animation)
        {
            // Setup meshes
            setup_meshes(viewer);

            // Dont initialize animations again
            m_initialize_animation = 0;
        }

        if(m_menu_handler->are_ports_set())
        {
            // Get euler angles
            auto euler_id =
                m_anim_hand->get_hand_angles(m_left_exo->get_joint_angles());
        
            // Update left hand
            m_left_hand->update(euler_id);
            viewer.data_list.at(0).set_vertices(m_left_hand->get_vertices());

            // Update right hand
            m_right_hand->update(euler_id);
            viewer.data_list.at(1).set_vertices(m_right_hand->get_vertices());

            // Update face
            viewer.data_list.at(2).set_vertices(m_face->get_vertices());
        }
    } 
    
    return false;
}

void SkeletalAnimation::setup_meshes(igl::opengl::glfw::Viewer& viewer)
{
    // Resize viewer data list
    viewer.data_list.resize(3);

    // Set mesh for face
    m_face = std::make_shared<Face>();
    
    viewer.data_list.at(2).set_mesh(m_face->get_vertices(),
        m_face->get_surface_indices());

   // Define serial COM for left exoskeleton
    std::string serial_com_left = m_menu_handler->get_left_exoskeleton_port();

   // Define serial COM for right exoskeleton
    std::string serial_com_right = m_menu_handler->get_right_exoskeleton_port();

    // Define baudrate
    unsigned int baud_rate = 115200;
    
    // Initialize left exoskeleton
    m_left_exo->initialize(serial_com_left, baud_rate);

    // Initialize right exoskeleton (to be done)

    // Initialize left hand
    m_left_hand = std::make_shared<Hand>(m_left_exo, m_anim_hand,
        m_hand_mesh_name, m_hand_graph_name, m_hand_texture_name, 0, 
        m_left_origin);
    
    // Set mesh for left hand
    viewer.data_list.at(0).set_mesh(m_left_hand->get_vertices(), 
        m_left_hand->get_surface_indices());

    // Initialze right hand
    m_right_hand = std::make_shared<Hand>(m_left_exo, m_anim_hand,
        m_hand_mesh_name, m_hand_graph_name, m_hand_texture_name, 1,
        m_right_origin);

    // Set mesh for right hand
    viewer.data_list.at(1).set_mesh(m_right_hand->get_vertices(), 
        m_right_hand->get_surface_indices());
}