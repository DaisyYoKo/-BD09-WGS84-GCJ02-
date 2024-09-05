//
// Created by YoKoDaisy on 2024-8-16.
// this a CoordTransform programs
// Coordinate system conversion between WGS84, GCJ02 and BD09.
//

#ifndef COORDTRANSFORM_H
#define COORDTRANSFORM_H

namespace CoordTransform {

    // CoordTransform������ִ�и�������ϵ֮���ת��
    class CoordTransform {
    private:
        // ����һЩ��������������ת���е���ѧ����
        const double x_PI = 3.14159265358979324 * 3000.0 / 180.0; // 3000/180 �ĳ˻�����
        const double PI = 3.1415926535897932384626; // Բ����
        const double a = 6378137.0; // WGS84����ϵ�µĵ��򳤰���
        const double ee = 0.00669437999013; // WGS84����ϵ�µĵ�һƫ����ƽ��
        const double e_e = 0.006739496742227; // WGS84����ϵ�µĵڶ�ƫ����ƽ��

         // �жϸ����ľ�γ���Ƿ����й���Χ��
        bool OutOfChina(double lng, double lat);

        // ����ת��γ�ȵĸ�������
        double TransformLat(double lng, double lat);

        // ����ת�����ȵĸ�������
        double TransformLng(double lng, double lat);

        // ̩�ռ���չ����������ĳһγ���µ������߻���������84ת2000��γ��
        double Merdian(double a, double ee, double wgsLat);

    public:
        // BD09 -> GCJ02����ϵת��������ֵ��ʾת���Ƿ�ɹ�
        bool BD09ToGCJ02(double bdLng, double bdLat, double& gcjLng, double& gcjLat);

        // GCJ02 -> BD09����ϵת��������ֵ��ʾת���Ƿ�ɹ�
        bool GCJ02ToBD09(double gcjLng, double gcjLat, double& bdLng, double& bdLat);

        // WGS84 -> GCJ02����ϵת��������ֵ��ʾת���Ƿ�ɹ�
        bool WGS84ToGCJ02(double wgsLng, double wgsLat, double& gcjLng, double& gcjLat);

        // GCJ02 -> WGS84����ϵת��������ֵ��ʾת���Ƿ�ɹ�
        bool GCJ02ToWGS84(double gcjLng, double gcjLat, double& wgsLng, double& wgsLat);

        // BD09 -> WGS84����ϵת��������ֵ��ʾת���Ƿ�ɹ�
        bool BD09ToWGS84(double bdLng, double bdLat, double& wgsLng, double& wgsLat);

        // WGS84-> BD09 ����ϵת��������ֵ��ʾת���Ƿ�ɹ�
        bool WGS84ToBD09(double wgsLng, double wgsLat, double& bdLng, double& bdLat);

        // GWGS84 -> CGCS2000����ϵת��������ֵ��ʾת���Ƿ�ɹ�
       //bool WGS84ToCGCS2000(double wsgLng, double wsgLat, double& cgcsX, double& cgcsY);
    };

} // CoordTransform

#endif //COORDTRANSFORM_COORDTRANSFORM_H