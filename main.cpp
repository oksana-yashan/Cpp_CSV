#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <set>
#include <chrono>
#include <unordered_map>
#include <future>
 
using namespace std;
using namespace std::chrono;


unordered_map<string, unordered_map<string, int>> csv_data;

vector<string> read_line_from_csv(string& line, char delimiter, int& n_columns, set<int> needed_columns ) {
	vector<string> row;
	string word;

	stringstream str(line);
	for (int column=0; column < n_columns; ++column) {
		getline(str, word, delimiter);
		if (needed_columns.count(column) > 0) 
			row.push_back(word);
	}
	return row;
}


void processing(vector<vector<string>>& content, int counter) {

	string curr_date = ""; 
	auto& row = content[counter];
	curr_date = row[1].substr(0, 7);
	if (csv_data[row[0]].count(curr_date) == 0)
		csv_data[row[0]][curr_date] = stoi(row[2]);
	else
		csv_data[row[0]][curr_date] += stoi(row[2]);
}


void read_from_csv(string& fname, char delimiter, int& n_columns, set<int>& needed_columns, vector<vector<string>>& content) {

	fstream file (fname, ios::in);
	string line;
	if(file.is_open())
	{
		int counter = 0;

		//title of csv file 
		getline(file, line);
		// get 0th line of data
		getline(file, line);
		future<vector<string>> future_row = std::async([&]() {return read_line_from_csv(line, delimiter, n_columns, needed_columns);});
		content.push_back(future_row.get());

		while(getline(file, line)) {
			future<vector<string>> future_row = std::async([&]() {return read_line_from_csv(line, delimiter, n_columns, needed_columns);});
			
			// processing  previous line while async future curr line of data
			processing(content, counter);
			++counter;
			// end of processing

			content.push_back(future_row.get());

		}
		// processing last row from future
		processing(content, counter);		
	}
	else
		cout<<"Could not open the file\n";
}


 
int main()
{
	string fname = "report.csv";
	char delimiter = ';';
	int n_columns = 8;
	set<int> needed_columns = {0, 6, 7};
	vector<vector<string>> content;	

	
    // auto start = high_resolution_clock::now();
	read_from_csv(fname, delimiter, n_columns, needed_columns, content);
	// auto stop = high_resolution_clock::now();
	// auto duration = duration_cast<milliseconds>(stop - start);
	// cout<<"Parsing Time: "<<duration.count()<<" ms"<<endl;

	for(auto& [name, map_] : csv_data) {
		for (auto& [date, hours] : map_) {
			cout<<name<<"\t"<<date<<"\t\t"<<hours<<endl;
		}
	}

 
	return 0;
}
 