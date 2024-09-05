//
// Created by YoKoDaisy on 2024-8-16.
// this a CoordTransform programs
// Coordinate system conversion between WGS84, GCJ02 and BD09.
//

#ifndef COORDTRANSFORM_H
#define COORDTRANSFORM_H

namespace CoordTransform {

    // CoordTransform类用于执行各种坐标系之间的转换
    class CoordTransform {
    private:
        // 定义一些常量，用于坐标转换中的数学计算
        const double x_PI = 3.14159265358979324 * 3000.0 / 180.0; // 3000/180 的乘积常量
        const double PI = 3.1415926535897932384626; // 圆周率
        const double a = 6378137.0; // WGS84坐标系下的地球长半轴
        const double ee = 0.00669437999013; // WGS84坐标系下的第一偏心率平方
        const double e_e = 0.006739496742227; // WGS84坐标系下的第二偏心率平方

         // 判断给定的经纬度是否在中国范围内
        bool OutOfChina(double lng, double lat);

        // 用于转换纬度的辅助函数
        double TransformLat(double lng, double lat);

        // 用于转换经度的辅助函数
        double TransformLng(double lng, double lat);

        // 泰勒级数展开，计算了某一纬度下的子午线弧长，计算84转2000的纬度
        double Merdian(double a, double ee, double wgsLat);

    public:
        // BD09 -> GCJ02坐标系转换，返回值表示转换是否成功
        bool BD09ToGCJ02(double bdLng, double bdLat, double& gcjLng, double& gcjLat);

        // GCJ02 -> BD09坐标系转换，返回值表示转换是否成功
        bool GCJ02ToBD09(double gcjLng, double gcjLat, double& bdLng, double& bdLat);

        // WGS84 -> GCJ02坐标系转换，返回值表示转换是否成功
        bool WGS84ToGCJ02(double wgsLng, double wgsLat, double& gcjLng, double& gcjLat);

        // GCJ02 -> WGS84坐标系转换，返回值表示转换是否成功
        bool GCJ02ToWGS84(double gcjLng, double gcjLat, double& wgsLng, double& wgsLat);

        // BD09 -> WGS84坐标系转换，返回值表示转换是否成功
        bool BD09ToWGS84(double bdLng, double bdLat, double& wgsLng, double& wgsLat);

        // WGS84-> BD09 坐标系转换，返回值表示转换是否成功
        bool WGS84ToBD09(double wgsLng, double wgsLat, double& bdLng, double& bdLat);

        // GWGS84 -> CGCS2000坐标系转换，返回值表示转换是否成功
       //bool WGS84ToCGCS2000(double wsgLng, double wsgLat, double& cgcsX, double& cgcsY);
    };

} // CoordTransform

#endif //COORDTRANSFORM_COORDTRANSFORM_H