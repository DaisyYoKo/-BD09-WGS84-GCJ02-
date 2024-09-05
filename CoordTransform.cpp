//
// Created by YoKoDaisy on 2024-8-16.
// this a CoordTransform programs
// Coordinate system conversion between WGS84, GCJ02 and BD09.
//

#include <cmath>
#include "CoordTransform.h"

namespace CoordTransform {
    // BD09 -> GCJ02 ����ת��
    bool CoordTransform::BD09ToGCJ02(double bdLng, double bdLat, double& gcjLng, double& gcjLat)
    {
        // ��BD09�����ȥƫ����
        double x = bdLng - 0.0065;
        double y = bdLat - 0.006;

        // ����ת�����GCJ02����
        double z = sqrt(x * x + y * y) - 0.00002 * sin(y * x_PI);
        double theta = atan2(y, x) - 0.000003 * cos(x * x_PI);
        gcjLng = z * cos(theta);
        gcjLat = z * sin(theta);
        return true;
    }

    // GCJ02 -> BD09 ����ת��
    bool CoordTransform::GCJ02ToBD09(double gcjLng, double gcjLat, double& bdLng, double& bdLat)
    {
        // ����ת�����BD09����
        double z = sqrt(gcjLng * gcjLng + gcjLat * gcjLat) + 0.00002 * sin(gcjLat * x_PI);
        double theta = atan2(gcjLat, gcjLng) + 0.000003 * cos(gcjLng * x_PI);
        bdLng = z * cos(theta) + 0.0065;
        bdLat = z * sin(theta) + 0.006;
        return true;
    }

    // WGS84 -> GCJ02 ����ת��
    bool CoordTransform::WGS84ToGCJ02(double wgsLng, double wgsLat, double& gcjLng, double& gcjLat)
    {
        // ������겻���й���Χ�ڣ�ֱ�ӷ���false
        if (OutOfChina(wgsLng, wgsLat))
        {
            return false;
        }
        else
        {
            // ����ƫ����
            double dLat = TransformLat(wgsLng - 105.0, wgsLat - 35.0);
            double dLng = TransformLng(wgsLng - 105.0, wgsLat - 35.0);

            // ����WGS84����ϵ����ת��
            double radLat = wgsLat / 180.0 * PI;
            double magic = sin(radLat);
            magic = 1 - ee * magic * magic;
            double sqrtMagic = sqrt(magic);

            // �������յ�GCJ02����
            dLat = (dLat * 180.0) / ((a * (1 - ee)) / (magic * sqrtMagic) * PI);
            dLng = (dLng * 180.0) / (a / sqrtMagic * cos(radLat) * PI);
            gcjLat = wgsLat + dLat;
            gcjLng = wgsLng + dLng;
            return true;
        }
    }

    // GCJ02 -> WGS84 ����ת��
    bool CoordTransform::GCJ02ToWGS84(double gcjLng, double gcjLat, double& wgsLng, double& wgsLat)
    {
        // ������겻���й���Χ�ڣ�ֱ�ӷ���false
        if (OutOfChina(gcjLng, gcjLat))
        {
            return false;
        }
        else
        {
            // ����ƫ����
            double dLat = TransformLat(gcjLng - 105.0, gcjLat - 35.0);
            double dLng = TransformLng(gcjLng - 105.0, gcjLat - 35.0);

            // ����GCJ02����ϵ����ת��
            double radLat = gcjLat / 180.0 * PI;
            double magic = sin(radLat);
            magic = 1 - ee * magic * magic;
            double sqrtMagic = sqrt(magic);

            // �������ʱ��WGS84����
            dLat = (dLat * 180.0) / ((a * (1 - ee)) / (magic * sqrtMagic) * PI);
            dLng = (dLng * 180.0) / (a / sqrtMagic * cos(radLat) * PI);
            double mgsLat = gcjLat + dLat;
            double mgsLng = gcjLng + dLng;

            // �����õ����յ�WGS84����
            wgsLat = gcjLat * 2 - mgsLat;
            wgsLng = gcjLng * 2 - mgsLng;
            return true;
        }
    }

    // BD09 -> WGS84 ����ת��
    bool CoordTransform::BD09ToWGS84(double bdLng, double bdLat, double& wgsLng, double& wgsLat)
    {
        double gcjLng, gcjLat;
        // BD09 -> GCJ02
        if (!BD09ToGCJ02(bdLng, bdLat, gcjLng, gcjLat)) {
            return false; // ת��ʧ��
        }
        // GCJ02 -> WGS84
        return GCJ02ToWGS84(gcjLng, gcjLat, wgsLng, wgsLat);
    }

    // WGS84 -> BD09 ����ת��
    bool CoordTransform::WGS84ToBD09(double wgsLng, double wgsLat, double& bdLng, double& bdLat)
    {
        double gcjLng, gcjLat;
        // WGS84 -> GCJ02
        if (!WGS84ToGCJ02(wgsLng, wgsLat, gcjLng, gcjLat)) {
            return false; // ת��ʧ��
        }
        // GCJ02 -> BD09
        return GCJ02ToBD09(gcjLng, gcjLat, bdLng, bdLat);
    }

    // WGS84 -> CGCS2000  ����ת��
    //bool CoordTransform::WGS84ToCGCS2000(double wsgLng, double wsgLat, double& cgcsX, double& cgcsY)
    //{
    //    // ���뾭�߾��ȣ��й�����һ��ȡ111�ȣ�
    //    double MerLon = 111;

    //    // ����γ�ȴӽǶ�ת��Ϊ����
    //    double EthD2R = 0.0174532925199433; // �� / 180
    //    double Lat = wsgLat * EthD2R;
    //    double Lon = wsgLng * EthD2R - MerLon * EthD2R;

    //    // ����î��Ȧ�뾶���������ʰ뾶��
    //    double RN = a / sqrt(1 - ee * sin(Lat) * sin(Lat));

    //    // ����Lon��Lat�Ķ���ʽչ����
    //    double Lon2 = Lon * Lon;             // ���ȵ�ƽ��
    //    double Lon4 = Lon2 * Lon2;           // ���ȵ��Ĵη�
    //    double tnLat = tan(Lat);             // γ�ȵ�����ֵ
    //    double tn2Lat = tnLat * tnLat;       // γ�ȵ�����ֵ��ƽ��
    //    double tn4Lat = tn2Lat * tn2Lat;     // γ�ȵ�����ֵ���Ĵη�
    //    double csLat = cos(Lat);             // γ�ȵ�����ֵ
    //    double cs2Lat = csLat * csLat;       // γ�ȵ�����ֵ��ƽ��
    //    double cs4Lat = cs2Lat * cs2Lat;     // γ�ȵ�����ֵ���Ĵη�


    //    double Eta2 = e_e * cs2Lat;       // �ڶ�ƫ����������ƽ���ĳ˻�
    //    double Eta22 = Eta2 * Eta2;        // Eta2��ƽ��

    //    // ����X���꣨��˹ͶӰ�����꣩
    //    // ʹ�������߻�����ʽ��Merdian���������ϸ�˹ͶӰ���㹫ʽ���������
    //    double x = Merdian(a, ee, Lat)
    //        + (RN * tnLat * cs2Lat * Lon2) *
    //        (0.5 + (5 - tn2Lat + 9 * Eta2 + 4 * Eta22) * cs2Lat * Lon2 / 24 + (61 - 58 * tn2Lat + tn4Lat) * cs4Lat * Lon4 / 720);
    //    // ����Y���꣨��˹ͶӰ�����꣩
    //    double y = RN * csLat * Lon *
    //        (1 + (1 - tn2Lat + Eta2) * cs2Lat * Lon2 / 6 + (5 - 18 * tn2Lat + tn4Lat + 14 * Eta2 - 58 * tn2Lat * Eta2) * cs4Lat * Lon4 / 120)
    //        + 500000; // ��500000����Ϊ��˹ͶӰ�������궼��500000��ʼ

    //    // ������õ���X��Y��ֵ���������
    //    cgcsX = x;
    //    cgcsY = y;

    //    return true; // ����true��ʾת���ɹ�
    //}

    // �ж��Ƿ񳬳��й��߽�
    bool CoordTransform::OutOfChina(double lng, double lat)
    {
        // �����γ�Ȳ����й���Χ�ڣ�����true
        return !(lng > 73.66 && lng < 135.05 && lat > 3.86 && lat < 53.55);
    }

    // ��������������ת��γ��
    double CoordTransform::TransformLat(double lng, double lat)
    {
        double ret = -100.0 + 2.0 * lng + 3.0 * lat + 0.2 * lat * lat + 0.1 * lng * lat + 0.2 * sqrt(abs(lng));
        ret += (20.0 * sin(6.0 * lng * PI) + 20.0 * sin(2.0 * lng * PI)) * 2.0 / 3.0;
        ret += (20.0 * sin(lat * PI) + 40.0 * sin(lat / 3.0 * PI)) * 2.0 / 3.0;
        ret += (160.0 * sin(lat / 12.0 * PI) + 320 * sin(lat * PI / 30.0)) * 2.0 / 3.0;
        return ret;
    }

    // ��������������ת������
    double CoordTransform::TransformLng(double lng, double lat)
    {
        double ret = 300.0 + lng + 2.0 * lat + 0.1 * lng * lng + 0.1 * lng * lat + 0.1 * sqrt(abs(lng));
        ret += (20.0 * sin(6.0 * lng * PI) + 20.0 * sin(2.0 * lng * PI)) * 2.0 / 3.0;
        ret += (20.0 * sin(lng * PI) + 40.0 * sin(lng / 3.0 * PI)) * 2.0 / 3.0;
        ret += (150.0 * sin(lng / 12.0 * PI) + 300.0 * sin(lng / 30.0 * PI)) * 2.0 / 3.0;
        return ret;
    }

    //// ̩�ռ���չ����������ĳһγ���µ������߻���������84ת2000��γ��
    //double CoordTransform::Merdian(double a, double ee, double wgsLat)
    //{
    //    // ���������߻�����ʽ�ļ򻯽�� S0
    //    // S0 �ǻ������򳤰뾶 a �͵�һƫ���� ee �ļ�����
    //    double S0 = a * (1 - ee);

    //    // ����ƫ���ʵĸ��η���e2, e4, e6, e8, e10, e12��
    //    // ��Щֵ�ں���̩��չ��ʽ������ʹ��
    //    double e2 = ee;       // ��һƫ���ʵ�ƽ��
    //    double e4 = e2 * e2;    // ��һƫ���ʵ��Ĵη�
    //    double e6 = e4 * e2;    // ��һƫ���ʵ����η�
    //    double e8 = e6 * e2;    // ��һƫ���ʵİ˴η�
    //    double e10 = e8 * e2;   // ��һƫ���ʵ�ʮ�η�
    //    double e12 = e10 * e2;  // ��һƫ���ʵ�ʮ���η�

    //    // ����̩��չ��ʽ�ĸ���ϵ����A1, B1, C1, D1, E1, F1, G1��
    //    // ��Щϵ�����������߻������㣬����ƫ���ʵĸ��η�չ���õ���
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

    //    // ���������߻��� X0
    //    // ���������߻�����ʽ��̩��չ��ʽ��ʹ�ø���ϵ����γ�ȼ���������߻���
    //    double X0 = S0 * (A1 * wgsLat
    //        - B1 * sin(2 * wgsLat)
    //        + C1 * sin(4 * wgsLat)
    //        - D1 * sin(6 * wgsLat)
    //        + E1 * sin(8 * wgsLat)
    //        - F1 * sin(10 * wgsLat)
    //        + G1 * sin(12 * wgsLat));

    //    // ���ؼ�����������߻���
    //    return X0;
    //}

}

