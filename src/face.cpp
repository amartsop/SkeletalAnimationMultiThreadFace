#include "../include/face.h"

/**
 * @brief Construct a new Face:: Face object
 * It initializes the tcp communcations and the asychronous functionalities. 
 * The indices of the face mesh are stored in the "vertices_connections.csv"
 * and they are converted to an Eigen::MatrixXi.
 */
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
/**
 * @brief It is the point of entry that feeds the animation
 * loop with the face vertices data. It also manages the asynchronous threading.
 * @return Eigen::MatrixXd The face vertices. 
 */
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

/**
 * @brief This is the callback function for reading asynchronously the incoming 
 * data (vertices) from the server. The data is sent as a json file, so 
 * a parser is also used for reading the x, y and z coordinates of each vertex
 * of the face mesh.
 * @return Eigen::MatrixXd The vertex pointcloud
 */
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

            // Rotate and scale vertices
            m_vol_mesh_vertices = (m_scale_mat * m_rot_mat *
                vol_mesh_vertices.transpose()).transpose();

            // Find centre of geometry
            double x_cg = m_vol_mesh_vertices.col(0).sum() / m_vertices_num;
            double y_cg = m_vol_mesh_vertices.col(1).sum() / m_vertices_num;
            double z_cg = m_vol_mesh_vertices.col(2).sum() / m_vertices_num;

            // Reset face offset
            Eigen::Vector3d face_offset = m_face_offset +
                Eigen::Vector3d({x_cg, y_cg, z_cg});

            m_vol_mesh_vertices -= (translation_matrix(face_offset,
                m_vertices_num));
        }
    }

    return m_vol_mesh_vertices; 
}

/**
 * @brief Since the libraries for face landmarking are all written in 
 * Python, we establish an Inter-process communication (IPC), to 
 * get all the data in our C++ program. In this case the IPC is implemented 
 * with the help of sockets. The python side sets up a server and it 
 * transmits all the data to our C++ client.
 * This method sets the tcp communication and initializes the socket and 
 * the client that listens to incoming data.
 */
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

/**
 * @brief This is not a homogenous transformation matrix. Instead it is 
 * used for shifting a matrix of vertices (as defined by libigl) by a given 
 * offset. 
 * @param offset The offset to shift the matrix of vertices.
 * @param vert_num The number of matrices.
 * @return Eigen::MatrixXd The output translation matrix.
 */
Eigen::MatrixXd Face::translation_matrix(const Eigen::Vector3d& offset,
    size_t vert_num)
{
    // Initialize matrix 
    Eigen::MatrixXd t_mat = Eigen::MatrixXd(vert_num, offset.rows());

    // Ones vector
    Eigen::VectorXd ones_vec = Eigen::VectorXd::Ones(vert_num);

    for(size_t i = 0; i < offset.rows(); i++)
    {
        t_mat.col(i) = offset(i) * ones_vec;
    }

    return t_mat;
}