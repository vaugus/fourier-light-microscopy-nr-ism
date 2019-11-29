/**
 * @file csv.hpp
 *
 * @brief Header file for class CSVWriter.
 *
 * Definition of methods from the  CSVWriter 
 * class with methods to write data into csv
 * files.
 *
 * @author Victor Augusto 
 * @version 1.0
 * @date 2019-11-21
 */

#ifndef CSV_HPP
#define CSV_HPP

#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <iostream>
#include <numeric>

class CSVWriter {

	public:

        /** @brief Instantiates a new CSVWriter object. */
        CSVWriter();

        /** @brief Destroys a CSVWriter instance. */
        ~CSVWriter();

        /**
         * @brief Inserts the content of a vector into a csv row.
         * 
         * @tparam T        Data 
         * @param first 	Pointer to the beginning of the range.
         * @param last		Pointer to the end of the range.     
         */
        template <typename T> void add_data_in_row(T first, T last);

        /**
         * @brief Writes a column .csv file with the descriptor from a single image.
         * 
         * @param data      Vector with the data to be written.
         */
        void write_single_fft_coeff_csv(std::vector<double> const &data);

        /**
         * @brief Writes a .csv file with the descriptors from a batch of images.
         * 
         * @param data      Vector with the data to be written.
         */
        void write_fft_descriptor_dataset(std::vector<std::vector<double>> const &data);

        /**
         * @param filename      Name of the file to be written.
         */
        void set_filename(std::string const& filename);
        
        /**
         * @return      A string with the filename.
         */
        std::string get_filename();
        
        /**
         * @param delimiter     String element as the delimiter of values.
         */
        void set_delimiter(std::string const& delimiter);
        
        /**
         * @return      The string which represents the delimiter.
         */
        std::string get_delimiter();

        /**
         * @param linecount     Number of lines of the file.
         */
        void set_linecount(unsigned const& linecount);
        
        /**
         * @return      An integer with the number of lines of the file.
         */
        unsigned get_linecount();

	private:

        /** Name of the file to be written. */
		std::string filename;
        
        /** String delimiter for the values. */
        std::string delimiter;
        
        /** Number of lines in the file. */
        unsigned linecount;
};

#endif