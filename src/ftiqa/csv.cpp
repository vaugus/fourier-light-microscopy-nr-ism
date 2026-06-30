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
void CSVWriter::writeSingleFFTCoeffCsv(std::vector<double> const &data) {
	// Adding header to CSV File
	const std::vector<std::string> header = {"grayscale"};
	addDataInRow(header.begin(), header.end());

	std::vector<double> tmp;
	for (unsigned i = 0; i < data.size(); i++) {
		tmp = {data[i]};

		addDataInRow(tmp.begin(), tmp.end());
		tmp.clear();
	}
}

/**
 * Accepts a range and appends all the elements in the range
 * to the last row, seperated by delimeter (default is comma).
 */
void CSVWriter::writeFFTDescriptorDataset(std::vector<std::vector<double>> const &data) {
	// Adding header to CSV File
	std::vector<int> header(data[0].size());
    std::iota(header.begin(), header.end(), 0);

	addDataInRow(header.begin(), header.end());

	for (auto const& elem : data) {
		addDataInRow(elem.begin(), elem.end());
	}
}


/*******************************************************************
 *******************************************************************
 * Getter and Setter methods.
 *******************************************************************
 *******************************************************************
 */
void CSVWriter::setFilename(std::string const& filename) {
	this->filename = filename;
}

std::string CSVWriter::getFilename() {
	return this->filename;
}

void CSVWriter::setDelimiter(std::string const& delimiter) {
	this->delimiter = delimiter;
}

std::string CSVWriter::getDelimiter() {
	return this->delimiter;
}

void CSVWriter::setLineCount(unsigned const& linecount) {
	this->linecount = linecount;
}

unsigned CSVWriter::getLineCount() {
	return this->linecount;
}
