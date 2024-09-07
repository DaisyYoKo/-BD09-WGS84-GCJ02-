//
//�ȷǸ߾��ȵĴ��������£����ǿ�����ΪWGS84����ϵ=CGCS2000����ϵ,2000����ϵ��WGS84����ϵƫ���
//��ΪCGCS2000����ϵ��WGS84����ϵ��ԭ�㡢�߶ȡ����򼰶����ݱ�Ķ��嶼����ͬ�ģ��ο�����Ĳ������в�ͬ���ѡ�
//��ͬ������㣬��CGCS2000��WGS84�£���������ͬ�ģ�ֻ��γ���ϴ���0.11mm���µ����𣬿��Ժ��Ե�...
//
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include "CoordTransform.h"

// ���ڴ洢����Ľṹ��
struct Coordinate {
    double lng;
    double lat;
};

// ��ӡ����
void printCoordinate(const std::string& label, const Coordinate& coord) {
    std::cout << std::fixed << std::setprecision(6);
    //std::cout << label << ": (" << coord.lng << ", " << coord.lat << ")" << std::endl;
    std::cout << coord.lng << " " << coord.lat << std::endl;

    std::ofstream newtxt("coordinates.txt", std::ios::app); 
    if (newtxt.is_open()) {
        newtxt << std::fixed << std::setprecision(6);
        newtxt << coord.lng << "    " << coord.lat << std::endl;
        newtxt.close(); // ȷ����д���ر��ļ�
    }
    else {
        std::cerr << "�޷����ļ�!" << std::endl;
    }
}

// �������ַ�����������
bool parseCoordinate(const std::string& input, Coordinate& coord) {
    std::istringstream stream(input);
    if (stream >> coord.lng >> coord.lat) {
        // ȷ��û�ж�����ַ�
        return stream.eof() || stream.peek() == std::istream::traits_type::eof();
    }
    return false;
}

int main() {
    CoordTransform::CoordTransform ct;
    std::vector<Coordinate> coordinates;
    std::string line;

    // ɾ���ɵ� coordinates.txt �ļ�
    if (std::remove("coordinates.txt") != 0) {
        std::cerr << "�޷�ɾ���ɵ��ļ�. �����ļ������ڻ�Ȩ�޲���." << std::endl;
    }

    // ��̬��������
    std::cout << "�����뾭γ�� (���� γ��), ÿ��һ�飬��γ��֮���ÿո����. ����س�ʱ�������벢����:" << std::endl;
    while (std::getline(std::cin, line)) {
        if (line.empty()) break; // ��������
        Coordinate coord;
        if (parseCoordinate(line, coord)) {
            coordinates.push_back(coord);
        }
        else {
            std::cout << "�������. ���ϸ��� '���� γ��'���� ." << std::endl;
            std::cout << "ע�⣺ '���� γ��'֮���пո� ." << std::endl;
        }
    }

    // �û�ѡ��ת��������ϵͳ
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

    // �����û����������
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
