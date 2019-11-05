#ifndef CSV_HPP
#define CSV_HPP

#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <numeric>

/**
 *Header file with CSVWrite class definition.
 *
 *@author  Victor Augusto
 *@version 1.0
 */
class CSVWriter {

	public:

        CSVWriter();
        ~CSVWriter();

		CSVWriter(std::string name, std::string delim = ","):
		filename(name), delimiter(delim), linecount(0)
		{}

        template <typename T> void add_data_in_row(T first, T last);

        void write_single_fft_energy_csv(std::vector<double> const &gray_data,
            std::vector<double> const &hsv_data);

        void write_single_fft_coeff_csv(std::vector<double> const &gray_data,
            std::vector<double> const &hsv_data);

        void write_single_fft_coeff_csv(std::vector<double> const &gray_data);

        void write_fft_descriptor_dataset(std::vector<std::vector<double>> const &gray_data);

        void write_batch_fft_energy_csv(std::vector<double> const &data);

        void set_filename(std::string const& filename);
        std::string get_filename();
        
        void set_delimiter(std::string const& delimiter);
        std::string get_delimiter();

        void set_linecount(unsigned const& linecount);
        unsigned get_linecount();

	private:

		std::string filename;
        std::string delimiter;
        unsigned linecount;
};

#endif