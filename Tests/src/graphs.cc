
class entry {
public:
	double x = 0, y =0 , x_error =0 , y_error =0;
};

class graph {
public:
	graph(size_t size_) {
		entries.resize(size_);
	}
	string name, title;
	int color = 0, linestyle = 0, lineWidth = 0;
	vector<entry> entries;
};


double convert2double(const string& line, size_t start_, size_t end_) {
	return atof(line.substr(start_, end_).c_str());
}
array<double,3> get_param(const string& line) {
	array<double, 3> ret;
	auto firs_brace = line.find_first_of('(');
	auto firs_comma = line.find_first_of(',');
	auto last_comma = line.find_last_of(',');
	auto last_brace = line.find_last_of(')');

	ret[0] = convert2double(line, firs_brace + 1, firs_comma - firs_brace - 1);
	ret[1] = convert2double(line, firs_comma + 1, last_comma - firs_comma - 1);
	ret[2] = convert2double(line, last_comma + 1, last_brace - last_comma - 1);

	return ret;
}

vector<graph> get_graphs( ifstream& in) {
	vector<graph> graphs;
	bool TGraph_section = false;
	string line;
	while (getline(in, line)) {

//		cout << line << endl;

		if (line.find("TGraphErrors") != string::npos) {

			auto firs_ = line.find_first_of('(');
			auto last_ = line.find_last_of(')');
			auto size_ = atoi(line.substr(firs_ + 1, last_ - firs_ - 1).c_str());
			graphs.push_back(graph(size_));
			//cout << "TGraph\n";
			TGraph_section = true;
			continue;
		}

		if (line.find_first_not_of(' ') == std::string::npos) {
			TGraph_section = false;
		}
		if (!TGraph_section) {
			continue;
		}



		if (line.find("SetName") != string::npos) {

			auto firs_ = line.find_first_of('"');
			auto last_ = line.find_last_of('"');
			graphs.back().name = line.substr(firs_ + 1, last_ - firs_ - 1);
			continue;
		}
		if (line.find("SetTitle") != string::npos) {

			auto firs_ = line.find_first_of('"');
			auto last_ = line.find_last_of('"');
			graphs.back().title = line.substr(firs_ + 1, last_ - firs_ - 1);
			continue;
		}
		if (line.find("SetPoint(") != string::npos) {



			array<double, 3>  r = get_param(line);

			graphs.back().entries.at(r[0]).x = r[1];
			graphs.back().entries.at(r[0]).y = r[2];

			continue;
		}
		if (line.find("SetPointError") != string::npos) {



			array<double, 3>  r = get_param(line);

			graphs.back().entries.at(r[0]).x_error = r[1];
			graphs.back().entries.at(r[0]).y_error = r[2];

			continue;
		}
	}
	return graphs;
}

vector<graph> get_graphs1(ifstream& in) {
	vector<graph> graphs;

	auto loop = [](auto&& next_ ,auto&& in) {
		string line;
		while (getline(in, line)) {
			next_(line);
		}
		return success;
	};


	auto searchForTGraph = [&graphs, TGraph_section = false](auto&& next_ ,  auto&& line)mutable {
		if (line.find("TGraphErrors") != string::npos) {

			auto firs_ = line.find_first_of('(');
			auto last_ = line.find_last_of(')');
			auto size_ = atoi(line.substr(firs_ + 1, last_ - firs_ - 1).c_str());
			graphs.push_back(graph(size_));
			//cout << "TGraph\n";
			TGraph_section = true;
			return success;
		}

		if (line.find_first_not_of(' ') == std::string::npos) {
			TGraph_section = false;
			return success;
		}
		if (!TGraph_section) {
			
			return success;
		}

		return next_(line);
		
	};


	auto setName = [&graphs](auto&& next ,auto&& line) {

		if (line.find("SetName") != string::npos) {

			auto firs_ = line.find_first_of('"');
			auto last_ = line.find_last_of('"');
			graphs.back().name = line.substr(firs_ + 1, last_ - firs_ - 1);
			return success;
		}
		return next(line);
	};
	auto setEntries = [&graphs](auto&& next, auto&& line) {
		if (line.find("SetPoint(") != string::npos) {



			array<double, 3>  r = get_param(line);

			graphs.back().entries.at(r[0]).x = r[1];
			graphs.back().entries.at(r[0]).y = r[2];

			return success;
		}
		if (line.find("SetPointError") != string::npos) {



			array<double, 3>  r = get_param(line);

			graphs.back().entries.at(r[0]).x_error = r[1];
			graphs.back().entries.at(r[0]).y_error = r[2];


			return next(graphs.back().entries.at(r[0]).x, graphs.back().entries.at(r[0]).y);
		}
		return success;
	};

	auto use = [](auto && next, auto x,auto y) {
		cout << x << "  " << y << endl;
		return next(x,y);
	};

	auto filter1 = [](auto && next, auto x, auto y) {

		if (x > 10) {
			return next(x, y);
		}
		return success;
	};
	auto filter2 = [](auto && next, auto x, auto y) {

		if (y > 4) {
			return next(x, y);
		}
		return success;
	};

	auto converter = [](auto && next, auto x, auto y) {

		for (int i = 0;i < 10; ++i) {

			next(x + i, y);
		}
		return success;
	};


	in | proc() >> loop   
		>> searchForTGraph 
		>> setName 
		>> setEntries 
		>> filter1
		>> filter2
		>> converter
		>> use;

	return graphs;
}

