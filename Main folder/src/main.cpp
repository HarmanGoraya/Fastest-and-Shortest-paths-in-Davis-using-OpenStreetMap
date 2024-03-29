#include "CSVReader.h"
#include "CSVWriter.h"
#include "XMLReader.h"
#include "XMLWriter.h"
#include "StringUtils.h"
#include "MapRouter.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <utility>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>


int main(int argc, char* argv[]) {
	std::string ResultsPath;
	std::string DataPath;
	size_t start1;
	size_t end1 = 0;
	for(int i = 0; i < argc; i++) {
		std::string str(argv[i]);
		std::vector<std::string> arg;
		while ((start1 = str.find_first_not_of("/", end1)) != std::string::npos) {
			end1 = str.find(" ", start1);
			printf("Line %d\n", __LINE__);
			arg.push_back(str.substr(start1, end1 - start1));
			std::cout << "Line "<<__LINE__<<" " << arg.back() << std::endl;
			printf("Line %d\n", __LINE__);
		}
		for (int j = 0; j < arg.size(); j++) {
			if (arg[j] == "--results=") {
				for (int k = 0; k < arg.size(); k++) {
					ResultsPath = ResultsPath + arg[k];
				}
			}
		}

	}

	size_t start2;
	size_t end2 = 0;
	for(int i = 0; i < argc; i++) {
		std::string str2(argv[i]);
		std::vector<std::string> arg2;
		while ((start2 = str2.find_first_not_of("/", end2)) != std::string::npos) {
			end2 = str2.find(" ", start2);
			printf("Line %d\n", __LINE__);
			arg2.push_back(str2.substr(start2, end2 - start2));
			std::cout << "Line "<<__LINE__<<" " << arg2.back() << std::endl;
			printf("Line %d\n", __LINE__);
		}
		for (int j = 0; j < arg2.size(); j++) {
			if (arg2[j] == "--data=") {
				for (int k = 0; k < arg2.size(); k++) {
					DataPath = DataPath + arg2[k];
				}
			}
		}

	}



    //load in maps and routes
    CMapRouter MapRouter;

    DataPath = "data/";
	ResultsPath = "results/";

    std::ifstream osm(DataPath + "davis.osm");
    std::ifstream stops(DataPath + "stops.csv");
    std::ifstream routes(DataPath + "routes.csv");

    MapRouter.LoadMapAndRoutes(osm, stops, routes);

    std::cout << ">";

	//printf("Line %d\n", __LINE__);

	std::string input2;

	//printf("Line %d\n", __LINE__);

	std::vector<CMapRouter::TPathStep> FastestPath;
	std::vector<CMapRouter::TNodeID> ShortestPath;

	//printf("Line %d\n", __LINE__);

	int PathType = 0;
	CMapRouter::TNodeID node1;
	CMapRouter::TNodeID node2;
	double DistTime;

	//printf("Line %d\n", __LINE__);

	std::getline(std::cin, input2);
	//std::cout << "Line "<<__LINE__<<" " << input2 << std::endl;
	std::vector<std::string> input;
	size_t start;
	size_t end = 0;

	while ((start = input2.find_first_not_of(" ", end)) != std::string::npos) {
		end = input2.find(" ", start);
		//printf("Line %d\n", __LINE__);
		input.push_back(input2.substr(start, end - start));
		//std::cout << "Line "<<__LINE__<<" " << input.back() << std::endl;
		//printf("Line %d\n", __LINE__);
	}
	//printf("Line %d\n", __LINE__);


    while (input[0] != "exit") {
		//printf("Line %d\n", __LINE__);


        if (input[0] == "help"){

			input.clear();
			for (int i = 0; i < input.size(); i++) {
				//std::cout << "Line "<<__LINE__<<" " << input[i] << std::endl;
			}
            std::cout << "findroute [--data=path | --resutls=path] \n"
"------------------------------------------------------------------------\n"
"help     Display this help menu\n"
"exit     Exit the program\n"
"count    Output the number of nodes in the map/\n"
"node     Syntax \"node [0, count)\"\n"
"         Will output node ID and Lat/Lon for node\n"
"fastest  Syntax \"fastest start end\"\n"
"         Calculates the time for fastest path from start to end\n"
"shortest Syntax \"shortest start end\"\n"
"         Calculates the distance for the shortest path from start to end\n"
"save     Saves the last calculated path to file\n"
"print    Prints the steps for the last calculated path\n"
 << std::endl;
        }

        else if (input[0] == "count") {
            std::cout << MapRouter.NodeCount() << std::endl;
        }

        else if (input[0] == "node") {
            std::stringstream i(input[1]);
            int index = 0;
            i >> index;
			CMapRouter::TNodeID nodeID = MapRouter.GetSortedNodeIDByIndex(index);
			CMapRouter::TLocation Location = MapRouter.GetSortedNodeLocationByIndex(index);

            std::cout << "Node " << index << ": " << "id = " << nodeID << " is at "
            << Location.first << ", " << Location.second << std::endl;

        }
        else if (input[0] == "fastest"){
            std::stringstream s(input[1]);
            CMapRouter::TNodeID src = 0;
            s >> src;

            std::stringstream d(input[2]);
            CMapRouter::TNodeID dest = 0;
            d >> dest;

            double fastest = 0;
            fastest = MapRouter.FindFastestPath(src, dest, FastestPath);
			if (fastest == 0) {
				std::cout << "Unable to find fastest path" << std::endl;
			}
			else {
				DistTime = fastest;
				PathType = 0;
            	std::cout << "Fastest path takes " << fastest << " hours"<< std::endl;
			}

        }
        else if (input[0] == "shortest") {
            std::stringstream s(input[1]);
            CMapRouter::TNodeID src = 0;
            s >> src;
			node1 = src;

            std::stringstream d(input[2]);
            CMapRouter::TNodeID dest = 0;
            d >> dest;
			node2 = src;

            double shortest = 0;
            shortest = MapRouter.FindShortestPath(src, dest, ShortestPath);
			if (shortest == 0) {
				std::cout << "Unable to find shortest path" << std::endl;
			}
			else {
				DistTime = shortest;
				PathType = 1;
				std::cout << "Shortest path is " << shortest << " miles" << std::endl;
			}


        }
        else if (input[0] == "save") {

			//output should look like: "/home/juaco28/Proj5/62264034_95712808_0.833378hr.csv".
			std::vector< std::string > desc;
			if (PathType == 0) {
            	MapRouter.GetPathDescription(FastestPath, desc);
			}
			else {
				MapRouter.GetPathShortest(ShortestPath, desc);
			}
			std::string filename;
            filename = ResultsPath + std::to_string(node1) + "_" + std::to_string(node2) + "_" + std::to_string(DistTime) + ".csv";

			std::ofstream myfile;
            myfile.open(filename);
			CCSVWriter writer(myfile);
			std::vector<std::string> headers;
			headers.push_back("mode");
			headers.push_back("node_id");
			writer.WriteRow(headers);

			//for (int i = 0; i < desc.size(); i++) {
			writer.WriteRow(desc);

			std::cout << "Saved path to " << filename << std::endl;



        }
        else if (input[0] == "print") {
            std::vector< std::string > desc;
			if (PathType == 0) {
            	MapRouter.GetPathDescription(FastestPath, desc);
				for (int i = 0; i < desc.size(); i++) {
					std::cout << desc[i] << std::endl;
				}
			}
			else {
				MapRouter.GetPathShortest(ShortestPath, desc);
				for (int i = 0; i < desc.size(); i++) {
					std::cout << desc[i] << std::endl;
				}

			}

        }
		else {
			std::cout << "Invalid input" << std::endl;
		}

		std::cout << ">";
		std::getline(std::cin, input2);
		//std::cout << "Line "<<__LINE__<<" " << input2 << std::endl;

		input.clear();

		//StringUtils::Split(input2);
		//printf("Line %d\n", __LINE__);
		end = 0;
		while ((start = input2.find_first_not_of(" ", end)) != std::string::npos) {
			end = input2.find(" ", start);
			input.push_back(input2.substr(start, end - start));
			//printf("Line %d\n", __LINE__);
			//std::cout << "Line "<<__LINE__<<" " << input2.substr(start, end - start) << std::endl;
			//printf("Line %d\n", __LINE__);
			//std::cout << "Line "<<__LINE__<<" " << input.back() << std::endl;
			//printf("Line %d\n", __LINE__);
		}
		//printf("Line %d\n", __LINE__);
    }
	return 0;
}
