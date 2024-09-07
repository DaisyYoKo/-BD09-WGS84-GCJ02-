//
//既非高精度的大多数情况下，我们可以认为WGS84坐标系=CGCS2000坐标系,2000坐标系与WGS84坐标系偏差不大，
//因为CGCS2000坐标系与WGS84坐标系的原点、尺度、定向及定向演变的定义都是相同的，参考椭球的参数略有不同而已。
//相同的坐标点，在CGCS2000与WGS84下，经度是相同的，只在纬度上存有0.11mm上下的区别，可以忽略掉...
//
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include "CoordTransform.h"

// 用于存储坐标的结构体
struct Coordinate {
    double lng;
    double lat;
};

// 打印坐标
void printCoordinate(const std::string& label, const Coordinate& coord) {
    std::cout << std::fixed << std::setprecision(6);
    //std::cout << label << ": (" << coord.lng << ", " << coord.lat << ")" << std::endl;
    std::cout << coord.lng << " " << coord.lat << std::endl;

    std::ofstream newtxt("coordinates.txt", std::ios::app); 
    if (newtxt.is_open()) {
        newtxt << std::fixed << std::setprecision(6);
        newtxt << coord.lng << "    " << coord.lat << std::endl;
        newtxt.close(); // 确保在写入后关闭文件
    }
    else {
        std::cerr << "无法打开文件!" << std::endl;
    }
}

// 从输入字符串解析坐标
bool parseCoordinate(const std::string& input, Coordinate& coord) {
    std::istringstream stream(input);
    if (stream >> coord.lng >> coord.lat) {
        // 确保没有多余的字符
        return stream.eof() || stream.peek() == std::istream::traits_type::eof();
    }
    return false;
}

int main() {
    CoordTransform::CoordTransform ct;
    std::vector<Coordinate> coordinates;
    std::string line;

    // 删除旧的 coordinates.txt 文件
    if (std::remove("coordinates.txt") != 0) {
        std::cerr << "无法删除旧的文件. 可能文件不存在或权限不足." << std::endl;
    }

    // 动态输入坐标
    std::cout << "请输入经纬度 (经度 纬度), 每行一组，经纬度之间用空格隔开. 输入回车时结束输入并介绍:" << std::endl;
    while (std::getline(std::cin, line)) {
        if (line.empty()) break; // 结束输入
        Coordinate coord;
        if (parseCoordinate(line, coord)) {
            coordinates.push_back(coord);
        }
        else {
            std::cout << "输入错误. 请严格按照 '经度 纬度'输入 ." << std::endl;
            std::cout << "注意： '经度 纬度'之间有空格 ." << std::endl;
        }
    }

    // 用户选择转换的坐标系统
    int choice;
    std::cout << "Select conversion type:\n";
    std::cout << "1: WGS84 to GCJ02\n";
    std::cout << "2: GCJ02 to WGS84\n";
    std::cout << "3: GCJ02 to BD09\n";
    std::cout << "4: BD09 to GCJ02\n";
    std::cout << "5: BD09 to WGS84\n";
    std::cout << "6: WGS84 to BD09\n";
   // std::cout << "7: WGS84 to CGCS2000\n";
    std::cin >> choice;

    // 处理用户输入的坐标
    for (const auto& coord : coordinates) {
        double resultLng, resultLat;
        double cgcsX, cgcsY;

        switch (choice) {
        case 1: // WGS84 to GCJ02
            ct.WGS84ToGCJ02(coord.lng, coord.lat, resultLng, resultLat);
            printCoordinate("GCJ02", { resultLng, resultLat });
            break;
        case 2: // GCJ02 to WGS84
            ct.GCJ02ToWGS84(coord.lng, coord.lat, resultLng, resultLat);
            printCoordinate("WGS84", { resultLng, resultLat });
            break;
        case 3: // GCJ02 to BD09
            ct.GCJ02ToBD09(coord.lng, coord.lat, resultLng, resultLat);
            printCoordinate("BD09", { resultLng, resultLat });
            break;
        case 4: // BD09 to GCJ02
            ct.BD09ToGCJ02(coord.lng, coord.lat, resultLng, resultLat);
            printCoordinate("GCJ02", { resultLng, resultLat });
            break;
        case 5: // BD09 to WGS84
            ct.BD09ToWGS84(coord.lng, coord.lat, resultLng, resultLat);
            printCoordinate("WGS84", { resultLng, resultLat });
            break;
        case 6: // WGS84 to BD09
            ct.WGS84ToBD09(coord.lng, coord.lat, resultLng, resultLat);
            printCoordinate("BD09", { resultLng, resultLat });
            break;
        //case 7: // WGS84 to CGCS2000
        //    ct.WGS84ToCGCS2000(coord.lng, coord.lat, cgcsX, cgcsY);
        //    std::cout << std::fixed << std::setprecision(6);
        //    std::cout << "CGCS2000: (" << cgcsX << ", " << cgcsY << ")" << std::endl;
        //    break;
        default:
            std::cout << "Invalid choice." << std::endl;
        }
    }

    system("pause");
    return 0;
}
