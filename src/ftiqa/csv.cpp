/**
 * @file csv.cpp
 *
 * @brief Source file for the implementation of the
 * CSVWriter class.
 *
 * Implementation of CSVWriter class
 * with methods to write data into csv files.
 *
 * @author Victor Augusto 
 * @version 1.0
 * @date 2019-11-21
 */

#include "ftiqa/csv.hpp"

#include <numeric>


CSVWriter::CSVWriter() {}

CSVWriter::~CSVWriter() {}

/**
 * Accepts a range and appends all the elements in the range
 * to the last row, seperated by delimeter (default is comma).
 */
void CSVWriter::write_single_fft_coeff_csv(std::vector<double> const &data) {
	// Adding header to CSV File
	const std::vector<std::string> header = {"grayscale"};
	add_data_in_row(header.begin(), header.end());

	std::vector<double> tmp;
	for (unsigned i = 0; i < data.size(); i++) {
		tmp = {data[i]};

		add_data_in_row(tmp.begin(), tmp.end());
		tmp.clear();
	}
}

/**
 * Accepts a range and appends all the elements in the range
 * to the last row, seperated by delimeter (default is comma).
 */
void CSVWriter::write_fft_descriptor_dataset(std::vector<std::vector<double>> const &data) {
	// Adding header to CSV File
	std::vector<int> header(data[0].size());
    std::iota(header.begin(), header.end(), 0);

	add_data_in_row(header.begin(), header.end());

	for (auto const& elem : data) {
		add_data_in_row(elem.begin(), elem.end());
	}
}


/*******************************************************************
 *******************************************************************
 * Getter and Setter methods.
 *******************************************************************
 *******************************************************************
 */
void CSVWriter::set_filename(std::string const& filename) {
	this->filename = filename;
}

std::string CSVWriter::get_filename() {
	return this->filename;
}

void CSVWriter::set_delimiter(std::string const& delimiter) {
	this->delimiter = delimiter;
}

std::string CSVWriter::get_delimiter() {
	return this->delimiter;
}

void CSVWriter::set_linecount(unsigned const& linecount) {
	this->linecount = linecount;
}

unsigned CSVWriter::get_linecount() {
	return this->linecount;
}
