// //Library_Imports
//
//
//
// //#include "src/globals.h"
// #include <iostream>
// #include <cmath>
// #include <random>
// #include <string>
// #include <fstream>
// #include <vector>
// #include "src/Coord.h"
//
// std::vector<float> splitAndConvertToFloats(const std::string &input, char delimiter) {
//     std::vector<float> floats;
//     std::istringstream stream(input);
//     std::string token;
//
//     while (std::getline(stream, token, delimiter)) {
//         // Convert hex string to float
//         double d = std::strtod(token.c_str(), nullptr);
//         floats.push_back(static_cast<float>(d));
//     }
//
//     return floats;
// }
//
// std::vector<std::string> readFileLines(const std::string& filename) {
//     std::vector<std::string> lines;
//     std::ifstream file(filename);
//
//     if (!file.is_open()) {
//         std::cerr << "Error: Unable to open file " << filename << std::endl;
//         return lines;
//     }
//     std::string line;
//     while (std::getline(file, line)) {
//         lines.push_back(line);
//     }
//
//     file.close();
//     return lines;
// }
//
// void printFloats(const std::vector<float>& floats) {
//     for (const auto& f : floats) {
//         std::cout << f << std::endl;
//     }
// }
//
// void testFun() {
//     std::string filename = "cam.txt"; // Replace with your file name
//     std::vector<std::string> lines = readFileLines(filename);
//     std::vector<float> floats;
//     std::vector<Coord> poses;
//     std::vector<Coord> tgts;
//     // Print the lines
//     for (const auto& line : lines) {
//         std::cout << line << std::endl;
//         floats = splitAndConvertToFloats(line, ',');
//         printFloats(floats);
//         poses.push_back(Coord(floats[0], floats[1], floats[2]));
//         std::cout << "Pose: " << poses.back().toString() << std::endl;
//         tgts.push_back(Coord(floats[3], floats[4], floats[5]));
//         std::cout << "Tgt: " << tgts.back().toString() << std::endl;
//     }
//
//     for (int i = 0; i < poses.size(); i++) {
//         std::cout << "Pose " << i << ": " << poses[i] << std::endl;
//         std::cout << "Target " << i << ": " << tgts[i] << std::endl;
//     }
//
// }


int main(int argc, char **argv) {
    // testFun();
    return 0;
}