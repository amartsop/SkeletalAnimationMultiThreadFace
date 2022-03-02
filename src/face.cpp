#include "../include/face.h"

Face::Face()
{
    // Get absolute file path
    std::filesystem::path absolute_path = std::filesystem::current_path();

    // Define absolute path of hand configuration file
    m_vertices_connections_abs_path =
        absolute_path / m_vertices_connections_rel_filename;

    // Read indices matrix
    Eigen::MatrixXd tmp_faces =
        Utils::load_matrix_data(m_vertices_connections_abs_path);
    m_surf_indices = tmp_faces.cast<int>();

    // Setup communication with python server 
    setup_tcp_communication();

    // Initialize asychronous function
    m_future_fun = std::async(&Face::incoming_data_callback, this);

    // Initialize volume vertices
    m_vol_mesh_vertices = Eigen::MatrixXd::Zero(m_vertices_num, 3);

    // Set rotation matrix
    m_rot_mat = EulerRotations::rotation(0, M_PI, M_PI);

}



// Get vertices
Eigen::MatrixXd Face::get_vertices(void)
{
    // Set termination flag for asychronous function
    m_return_value = 1;

    // Get vertices
    Eigen::MatrixXd vertices =  m_future_fun.get();

    // Reinitialize async function
    m_return_value = 0;
    m_future_fun = std::async(&Face::incoming_data_callback, this);

    return vertices;
}

// Incoming data callback
Eigen::MatrixXd Face::incoming_data_callback(void)
{
    while(!m_return_value)
    {
        
        // Initialize buffer
        char buffer[m_buffer_size] = {0};

        // Read value
        int val_read = read(m_socket, buffer, m_buffer_size);

        // Correct parsing flag         
        bool correct_parsing;        

        // Initialzie volume mesh
        Eigen::MatrixXd vol_mesh_vertices;
  
        // Initialize point cloud
        nlohmann::basic_json<>::value_type x_pc, y_pc, z_pc;
  
        try
        {
            // Parse json file 
            nlohmann::json incoming_data = nlohmann::json::parse(buffer);

            // Pointcloud x
            x_pc = incoming_data["x"];
            y_pc = incoming_data["y"];
            z_pc = incoming_data["z"];
        
            vol_mesh_vertices.resize(x_pc.size(), 3);
        
            // Check if the parsing is correct
            correct_parsing = (x_pc.size() == m_vertices_num);
        }
        catch(nlohmann::json::parse_error& e)
        {
            // Parsing incorrect
            correct_parsing = 0;
        }
        
        if (correct_parsing) 
        {
            for (size_t i = 0; i < x_pc.size(); i++)
            {
                vol_mesh_vertices(i, 0) = x_pc[i];
                vol_mesh_vertices(i, 1) = y_pc[i];
                vol_mesh_vertices(i, 2) = z_pc[i];
            }

            m_vol_mesh_vertices = (m_scale_mat * m_rot_mat *
                vol_mesh_vertices.transpose()).transpose();
        }
    }

    return m_vol_mesh_vertices; 
}

// Start listening to python data
void Face::setup_tcp_communication(void)
{
    // Sock addr struct
	struct sockaddr_in serv_addr;

	if ((m_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(m_tcp_port);
	
	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, m_local_ip_address.c_str(), &serv_addr.sin_addr)<=0)
	{
		printf("\nInvalid address/ Address not supported \n");
	}

	if (connect(m_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
	}
}