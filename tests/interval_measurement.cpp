auto start = chrono::steady_clock::now();

auto end = chrono::steady_clock::now();

cout << "Elapsed time in nanoseconds : " 
    << chrono::duration_cast<chrono::nanoseconds>(end - start).count()
    << " ns" << endl;