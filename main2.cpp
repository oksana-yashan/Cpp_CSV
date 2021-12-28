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


void read_from_csv(string& fname, char delimiter, int& n_columns, set<int>& needed_columns, vector<vector<string>>& content) {
	vector<string> row;
	string line, word;

	fstream file (fname, ios::in);
	if(file.is_open())
	{
		while(getline(file, line))
		{
			row.clear();
 
			stringstream str(line);
			for (int column=0; column < n_columns; ++column) {
				getline(str, word, delimiter);
				if (needed_columns.count(column) > 0) 
					row.push_back(word);
			}
			content.push_back(row);
		}
		
		content.erase(content.begin());
	}
	else
		cout<<"Could not open the file\n";
 
	// for(int i=0;i<content.size();i++)
	// {
	// 	for(int j=0;j<content[i].size();j++)
	// 	{
	// 		cout<<content[i][j]<<" ";
	// 	}
	// 	cout<<"\n";
	// }
}


 
int main()
{
	string fname = "report.csv";
	char delimiter = ';';
	int n_columns = 8;
	set<int> needed_columns = {0, 6, 7};
	vector<vector<string>> content;	

	
    auto start = high_resolution_clock::now();
	read_from_csv(fname, delimiter, n_columns, needed_columns, content);
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
	cout<<"Parsing Time: "<<duration.count()<<" ms"<<endl;


	unordered_map<string, unordered_map<string, int>> data;
	string curr_date = "";
	for ( auto& row : content ) {
		curr_date = row[1].substr(0, 7);
		// cout<<curr_date<<endl;
		if (data[row[0]].count(curr_date) == 0)
			data[row[0]][curr_date] = stoi(row[2]);
		else
			data[row[0]][curr_date] += stoi(row[2]);
	}

	for(auto& [name, map_] : data) {
		for (auto& [date, hours] : map_) {
			cout<<name<<"\t"<<date<<"\t\t"<<hours<<endl;
		}
	}

 
	return 0;
}
 