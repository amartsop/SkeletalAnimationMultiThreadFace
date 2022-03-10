#include "../include/utils.h"

/**
 * @brief Return a vector of integers from a comma-delimited string.
 *  (could have been templated if you can template stoi and stod).
 * @param str The comma-delimited string.
 * @return std::vector<int> The output vector of integers.
 */
std::vector<int> Utils::analog_str_buf_to_int_vec(const std::string& str)
{
    // Initialize data vector
    std::vector<int> data_vec;

    // Initialize string stream
    std::stringstream ss(str);

    // Get data and convert to analog
    while (ss.good())
    {
        std::string substr;
        std::getline(ss, substr, ',');
        data_vec.push_back(std::stoi(substr));
    }

    return data_vec;
}

/**
 * @brief Return a vector of doubles from a comma-delimited string.
 * 
 * @param str The comma-delimited string.
 * @return std::vector<int> The output vector of doubles.
 */
std::vector<double> Utils::analog_str_buf_to_double_vec(const std::string& str)
{
    // Initialize data vector
    std::vector<double> data_vec;

    // Initialize string stream
    std::stringstream ss(str);

    // Get data and convert to analog
    while (ss.good())
    {
        std::string substr;
        std::getline(ss, substr, ',');
        data_vec.push_back(std::stod(substr));
    }

    return data_vec;
}

/**
 * @brief Load Eigen Matrix from csv file
 * 
 * @param filename 
 * @return Eigen::MatrixXd The matrix to be read.
 */
Eigen::MatrixXd Utils::load_matrix_data(std::string filename)
{
  // Matrix entries
  std::vector<double> matrix_entries;

  // In this object we store the data from the matrix
  std::ifstream matrix_data_file(filename);
 
  // This variable is used to store the row of the matrix that contains commas 
  std::string matrix_row_string;
 
  // This variable is used to store the matrix entry;
  std::string matrix_entry;
 
  // This variable is used to track the number of rows
  int matrix_row_number = 0;
 
  /* We read a row by row of matrix_data_file and store every line into the
    lstring variable matrix_row_string */
  while (std::getline(matrix_data_file, matrix_row_string)) 
  {
    // Convert matrixRowString that is a string to a stream variable.
    std::stringstream matrix_row_string_stream(matrix_row_string);
 
    /* here we read pieces of the stream matrix_row_string_stream until every
      comma, and store the resulting character into the matrix_entry */

      while (std::getline(matrix_row_string_stream, matrix_entry, ',')) 
      {
        /* Here we convert the string to double and fill in the row vector
        storing all the matrix entries */
        matrix_entries.push_back(stod(matrix_entry));   
      }

      // Update the column numbers
      matrix_row_number++; 
  }
 
    /* Here we convet the vector variable into the matrix and return the
    resulting object, note that matrix_entris.data() is the pointer to the
    first memory location at which the entries of the vector matrixEntries are
    stored */

    return Eigen::Map< Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic,
      Eigen::RowMajor>>(matrix_entries.data(), matrix_row_number,
      matrix_entries.size() / matrix_row_number);
}