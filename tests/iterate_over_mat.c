cv::MatConstIterator_<double> it = gray.begin<double>(), it_end = gray.end<double>();
for (; it != it_end; ++it) {
    std::cout << *it << std::endl;
}