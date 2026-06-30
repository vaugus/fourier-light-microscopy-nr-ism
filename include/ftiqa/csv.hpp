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
#include <iomanip>
#include <string>
#include <vector>

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
        /** @brief Writes a column .csv file with the descriptor from a single image. */
        void writeSingleFFTCoeffCsv(std::vector<double> const &data);

        /**
         * @brief Writes a .csv file with the descriptors from a batch of images.
         * 
         * @param data      Vector with the data to be written.
         */
        void writeFFTDescriptorDataset(std::vector<std::vector<double>> const &data);

        /**
         * @param filename      Name of the file to be written.
         */
        void setFilename(std::string const& filename);
        
        /**
         * @return      A string with the filename.
         */
        std::string getFilename();
        
        /**
         * @param delimiter     String element as the delimiter of values.
         */
        void setDelimiter(std::string const& delimiter);
        
        /**
         * @return      The string which represents the delimiter.
         */
        std::string getDelimiter();

        /**
         * @param linecount     Number of lines of the file.
         */
        void setLineCount(unsigned const& linecount);
        
        /**
         * @return      An integer with the number of lines of the file.
         */
        unsigned getLineCount();

	private:
        /**
         * @brief Inserts the content of a vector into a csv row.
         * 
         * @tparam T        Data 
         * @param first 	Pointer to the beginning of the range.
         * @param last		Pointer to the end of the range.     
         */
        template <typename T> void addDataInRow(T first, T last);

        /** Name of the file to be written. */
		std::string filename;
        
        /** String delimiter for the values. */
        std::string delimiter;
        
        /** Number of lines in the file. */
        unsigned linecount;
};

template <typename T>
void CSVWriter::addDataInRow(T first, T last) {
	std::fstream file;

	file.open(filename, std::ios::out | (linecount ? std::ios::app : std::ios::trunc));

	for (; first != last; ) {
        file << std::setprecision(20) << *first;

		if (++first != last) {
			file << std::setprecision(20) << delimiter;
        }
	}

	file << "\n";
	linecount++;

	file.close();
}

#endif
