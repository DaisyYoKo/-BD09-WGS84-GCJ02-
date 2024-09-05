//
// Created by YoKoDaisy on 2024-8-16.
// this a CoordTransform programs
// Coordinate system conversion between WGS84, GCJ02 and BD09.
//

#include <cmath>
#include "CoordTransform.h"

namespace CoordTransform {
    // BD09 -> GCJ02 坐标转换
    bool CoordTransform::BD09ToGCJ02(double bdLng, double bdLat, double& gcjLng, double& gcjLat)
    {
        // 将BD09坐标减去偏移量
        double x = bdLng - 0.0065;
        double y = bdLat - 0.006;

        // 计算转换后的GCJ02坐标
        double z = sqrt(x * x + y * y) - 0.00002 * sin(y * x_PI);
        double theta = atan2(y, x) - 0.000003 * cos(x * x_PI);
        gcjLng = z * cos(theta);
        gcjLat = z * sin(theta);
        return true;
    }

    // GCJ02 -> BD09 坐标转换
    bool CoordTransform::GCJ02ToBD09(double gcjLng, double gcjLat, double& bdLng, double& bdLat)
    {
        // 计算转换后的BD09坐标
        double z = sqrt(gcjLng * gcjLng + gcjLat * gcjLat) + 0.00002 * sin(gcjLat * x_PI);
        double theta = atan2(gcjLat, gcjLng) + 0.000003 * cos(gcjLng * x_PI);
        bdLng = z * cos(theta) + 0.0065;
        bdLat = z * sin(theta) + 0.006;
        return true;
    }

    // WGS84 -> GCJ02 坐标转换
    bool CoordTransform::WGS84ToGCJ02(double wgsLng, double wgsLat, double& gcjLng, double& gcjLat)
    {
        // 如果坐标不在中国范围内，直接返回false
        if (OutOfChina(wgsLng, wgsLat))
        {
            return false;
        }
        else
        {
            // 计算偏移量
            double dLat = TransformLat(wgsLng - 105.0, wgsLat - 35.0);
            double dLng = TransformLng(wgsLng - 105.0, wgsLat - 35.0);

            // 基于WGS84坐标系进行转换
            double radLat = wgsLat / 180.0 * PI;
            double magic = sin(radLat);
            magic = 1 - ee * magic * magic;
            double sqrtMagic = sqrt(magic);

            // 计算最终的GCJ02坐标
            dLat = (dLat * 180.0) / ((a * (1 - ee)) / (magic * sqrtMagic) * PI);
            dLng = (dLng * 180.0) / (a / sqrtMagic * cos(radLat) * PI);
            gcjLat = wgsLat + dLat;
            gcjLng = wgsLng + dLng;
            return true;
        }
    }

    // GCJ02 -> WGS84 坐标转换
    bool CoordTransform::GCJ02ToWGS84(double gcjLng, double gcjLat, double& wgsLng, double& wgsLat)
    {
        // 如果坐标不在中国范围内，直接返回false
        if (OutOfChina(gcjLng, gcjLat))
        {
            return false;
        }
        else
        {
            // 计算偏移量
            double dLat = TransformLat(gcjLng - 105.0, gcjLat - 35.0);
            double dLng = TransformLng(gcjLng - 105.0, gcjLat - 35.0);

            // 基于GCJ02坐标系进行转换
            double radLat = gcjLat / 180.0 * PI;
            double magic = sin(radLat);
            magic = 1 - ee * magic * magic;
            double sqrtMagic = sqrt(magic);

            // 计算出暂时的WGS84坐标
            dLat = (dLat * 180.0) / ((a * (1 - ee)) / (magic * sqrtMagic) * PI);
            dLng = (dLng * 180.0) / (a / sqrtMagic * cos(radLat) * PI);
            double mgsLat = gcjLat + dLat;
            double mgsLng = gcjLng + dLng;

            // 调整得到最终的WGS84坐标
            wgsLat = gcjLat * 2 - mgsLat;
            wgsLng = gcjLng * 2 - mgsLng;
            return true;
        }
    }

    // BD09 -> WGS84 坐标转换
    bool CoordTransform::BD09ToWGS84(double bdLng, double bdLat, double& wgsLng, double& wgsLat)
    {
        double gcjLng, gcjLat;
        // BD09 -> GCJ02
        if (!BD09ToGCJ02(bdLng, bdLat, gcjLng, gcjLat)) {
            return false; // 转换失败
        }
        // GCJ02 -> WGS84
        return GCJ02ToWGS84(gcjLng, gcjLat, wgsLng, wgsLat);
    }

    // WGS84 -> BD09 坐标转换
    bool CoordTransform::WGS84ToBD09(double wgsLng, double wgsLat, double& bdLng, double& bdLat)
    {
        double gcjLng, gcjLat;
        // WGS84 -> GCJ02
        if (!WGS84ToGCJ02(wgsLng, wgsLat, gcjLng, gcjLat)) {
            return false; // 转换失败
        }
        // GCJ02 -> BD09
        return GCJ02ToBD09(gcjLng, gcjLat, bdLng, bdLat);
    }

    // WGS84 -> CGCS2000  坐标转换
    //bool CoordTransform::WGS84ToCGCS2000(double wsgLng, double wsgLat, double& cgcsX, double& cgcsY)
    //{
    //    // 中央经线经度（中国区域一般取111度）
    //    double MerLon = 111;

    //    // 将经纬度从角度转换为弧度
    //    double EthD2R = 0.0174532925199433; // π / 180
    //    double Lat = wsgLat * EthD2R;
    //    double Lon = wsgLng * EthD2R - MerLon * EthD2R;

    //    // 计算卯酉圈半径（横向曲率半径）
    //    double RN = a / sqrt(1 - ee * sin(Lat) * sin(Lat));

    //    // 计算Lon和Lat的多项式展开项
    //    double Lon2 = Lon * Lon;             // 经度的平方
    //    double Lon4 = Lon2 * Lon2;           // 经度的四次方
    //    double tnLat = tan(Lat);             // 纬度的正切值
    //    double tn2Lat = tnLat * tnLat;       // 纬度的正切值的平方
    //    double tn4Lat = tn2Lat * tn2Lat;     // 纬度的正切值的四次方
    //    double csLat = cos(Lat);             // 纬度的余弦值
    //    double cs2Lat = csLat * csLat;       // 纬度的余弦值的平方
    //    double cs4Lat = cs2Lat * cs2Lat;     // 纬度的余弦值的四次方


    //    double Eta2 = e_e * cs2Lat;       // 第二偏心率与余弦平方的乘积
    //    double Eta22 = Eta2 * Eta2;        // Eta2的平方

    //    // 计算X坐标（高斯投影横坐标）
    //    // 使用子午线弧长公式（Merdian函数）加上高斯投影正算公式计算横坐标
    //    double x = Merdian(a, ee, Lat)
    //        + (RN * tnLat * cs2Lat * Lon2) *
    //        (0.5 + (5 - tn2Lat + 9 * Eta2 + 4 * Eta22) * cs2Lat * Lon2 / 24 + (61 - 58 * tn2Lat + tn4Lat) * cs4Lat * Lon4 / 720);
    //    // 计算Y坐标（高斯投影纵坐标）
    //    double y = RN * csLat * Lon *
    //        (1 + (1 - tn2Lat + Eta2) * cs2Lat * Lon2 / 6 + (5 - 18 * tn2Lat + tn4Lat + 14 * Eta2 - 58 * tn2Lat * Eta2) * cs4Lat * Lon4 / 120)
    //        + 500000; // 加500000是因为高斯投影的纵坐标都从500000开始

    //    // 将计算得到的X和Y赋值给输出变量
    //    cgcsX = x;
    //    cgcsY = y;

    //    return true; // 返回true表示转换成功
    //}

    // 判断是否超出中国边界
    bool CoordTransform::OutOfChina(double lng, double lat)
    {
        // 如果经纬度不在中国范围内，返回true
        return !(lng > 73.66 && lng < 135.05 && lat > 3.86 && lat < 53.55);
    }

    // 辅助函数，用于转换纬度
    double CoordTransform::TransformLat(double lng, double lat)
    {
        double ret = -100.0 + 2.0 * lng + 3.0 * lat + 0.2 * lat * lat + 0.1 * lng * lat + 0.2 * sqrt(abs(lng));
        ret += (20.0 * sin(6.0 * lng * PI) + 20.0 * sin(2.0 * lng * PI)) * 2.0 / 3.0;
        ret += (20.0 * sin(lat * PI) + 40.0 * sin(lat / 3.0 * PI)) * 2.0 / 3.0;
        ret += (160.0 * sin(lat / 12.0 * PI) + 320 * sin(lat * PI / 30.0)) * 2.0 / 3.0;
        return ret;
    }

    // 辅助函数，用于转换经度
    double CoordTransform::TransformLng(double lng, double lat)
    {
        double ret = 300.0 + lng + 2.0 * lat + 0.1 * lng * lng + 0.1 * lng * lat + 0.1 * sqrt(abs(lng));
        ret += (20.0 * sin(6.0 * lng * PI) + 20.0 * sin(2.0 * lng * PI)) * 2.0 / 3.0;
        ret += (20.0 * sin(lng * PI) + 40.0 * sin(lng / 3.0 * PI)) * 2.0 / 3.0;
        ret += (150.0 * sin(lng / 12.0 * PI) + 300.0 * sin(lng / 30.0 * PI)) * 2.0 / 3.0;
        return ret;
    }

    //// 泰勒级数展开，计算了某一纬度下的子午线弧长，计算84转2000的纬度
    //double CoordTransform::Merdian(double a, double ee, double wgsLat)
    //{
    //    // 计算子午线弧长公式的简化结果 S0
    //    // S0 是基于椭球长半径 a 和第一偏心率 ee 的计算结果
    //    double S0 = a * (1 - ee);

    //    // 计算偏心率的各次方（e2, e4, e6, e8, e10, e12）
    //    // 这些值在后续泰勒展开式计算中使用
    //    double e2 = ee;       // 第一偏心率的平方
    //    double e4 = e2 * e2;    // 第一偏心率的四次方
    //    double e6 = e4 * e2;    // 第一偏心率的六次方
    //    double e8 = e6 * e2;    // 第一偏心率的八次方
    //    double e10 = e8 * e2;   // 第一偏心率的十次方
    //    double e12 = e10 * e2;  // 第一偏心率的十二次方

    //    // 计算泰勒展开式的各项系数（A1, B1, C1, D1, E1, F1, G1）
    //    // 这些系数用于子午线弧长计算，是由偏心率的各次方展开得到的
    //    double A1 = 1 + 3 * e2 / 4 + 45 * e4 / 64 + 175 * e6 / 256 +
    //        11025 * e8 / 16384 + 43659 * e10 / 65536 +
    //        693693 * e12 / 1048576;
    //    double B1 = 3 * e2 / 8 + 15 * e4 / 32 + 525 * e6 / 1024 +
    //        2205 * e8 / 4096 + 72765 * e10 / 131072 +
    //        297297 * e12 / 524288;
    //    double C1 = 15 * e4 / 256 + 105 * e6 / 1024 + 2205 * e8 / 16384 +
    //        10395 * e10 / 65536 + 1486485 * e12 / 8388608;
    //    double D1 = 35 * e6 / 3072 + 105 * e8 / 4096 + 10395 * e10 / 262144 +
    //        55055 * e12 / 1048576;
    //    double E1 = 315 * e8 / 131072 + 3465 * e10 / 524288 +
    //        99099 * e12 / 8388608;
    //    double F1 = 693 * e10 / 1310720 + 9009 * e12 / 5242880;
    //    double G1 = 1001 * e12 / 8388608;

    //    // 计算子午线弧长 X0
    //    // 基于子午线弧长公式的泰勒展开式，使用各项系数和纬度计算出子午线弧长
    //    double X0 = S0 * (A1 * wgsLat
    //        - B1 * sin(2 * wgsLat)
    //        + C1 * sin(4 * wgsLat)
    //        - D1 * sin(6 * wgsLat)
    //        + E1 * sin(8 * wgsLat)
    //        - F1 * sin(10 * wgsLat)
    //        + G1 * sin(12 * wgsLat));

    //    // 返回计算出的子午线弧长
    //    return X0;
    //}

}

