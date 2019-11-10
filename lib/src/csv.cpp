#include "../include/csv.hpp"

/**
 * Implementation of CSVWriter class
 * with methods to write data into csv files.
 *
 * @author Victor Augusto
 * @version 1.0
 */

CSVWriter::CSVWriter() {}

CSVWriter::~CSVWriter() {}

/*
 * Accepts a range and appends all the elements in the range
 * to the last row, seperated by delimeter (default is comma).
 *
 * @param first 	pointer to the beginning of the range.
 *
 * @param last		pointer to the end of the range.
 */
template<typename T>
void CSVWriter::add_data_in_row(T first, T last) {

	std::fstream file;

	// open the file in truncate mode if first line else in Append Mode
	file.open(filename, std::ios::out | (linecount ? std::ios::app : std::ios::trunc));

	// iterate over the range and add each element to file seperated by delimeter.
	for (; first != last; ) {
        file << std::setprecision(20) << *first;

		if (++first != last) {
			file << std::setprecision(20) << delimiter;
        }
	}

	file << "\n";
	linecount++;

	// close the file
	file.close();
}

void CSVWriter::write_single_fft_energy_csv(std::vector<double> const& gray_data,
											std::vector<double> const& hsv_data) {
	// Adding header to CSV File
	const std::vector<std::string> header = {"mid", "high", "highest"};
	add_data_in_row(header.begin(), header.end());

	add_data_in_row(gray_data.begin(), gray_data.end());
	add_data_in_row(hsv_data.begin(), hsv_data.end());
}

void CSVWriter::write_single_fft_coeff_csv(std::vector<double> const& gray_data,
										   std::vector<double> const& hsv_data) {
	// Adding header to CSV File
	const std::vector<std::string> header = {"grayscale", "hsv"};
	add_data_in_row(header.begin(), header.end());

	std::vector<double> tmp;
	for (unsigned i = 0; i < gray_data.size(); i++) {
		tmp = {gray_data[i], hsv_data[i]};

		add_data_in_row(tmp.begin(), tmp.end());
		tmp.clear();
	}
}

void CSVWriter::write_single_fft_coeff_csv(std::vector<double> const &gray_data) {
	// Adding header to CSV File
	const std::vector<std::string> header = {"grayscale"};
	add_data_in_row(header.begin(), header.end());

	std::vector<double> tmp;
	for (unsigned i = 0; i < gray_data.size(); i++) {
		tmp = {gray_data[i]};

		add_data_in_row(tmp.begin(), tmp.end());
		tmp.clear();
	}
}

void CSVWriter::write_fft_descriptor_dataset(std::vector<std::vector<double>> const &gray_data) {
	// Adding header to CSV File
	std::vector<int> header(gray_data[0].size());
    std::iota(header.begin(), header.end(), 0);

	add_data_in_row(header.begin(), header.end());

	for (auto const& elem : gray_data) {
		add_data_in_row(elem.begin(), elem.end());
	}
}

void CSVWriter::write_batch_fft_energy_csv(std::vector<double> const& data) {
	std::vector<double> tmp;
	for (unsigned i = 0; i < data.size(); i++) {
		tmp = {data[i]};

		add_data_in_row(tmp.begin(), tmp.end());
		tmp.clear();
	}
}


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