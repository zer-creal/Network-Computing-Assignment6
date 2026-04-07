#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

struct AP {
    double x, y, z;
    int ch24, ch5;
};

vector<AP> generateAPs() {
    vector<AP> aps;
    double zs[] = {1.75, 5.25, 8.75};
    double x_offsets[] = {0.0, 0.5, -0.5};
    double y_offsets[] = {0.0, 1.0, -1.0};
    double ys[] = {7.5, 22.5, 37.5, 52.5, 67.5, 82.5};
    int ch24_seq[] = {1, 6, 11, 1, 6, 11};
    int ch5_seq[] = {36, 40, 44, 48, 52, 56};
    for (int z_idx = 0; z_idx < 3; ++z_idx) {
        double z = zs[z_idx];
        double x_offset = x_offsets[z_idx];
        double y_offset = y_offsets[z_idx];
        for (int i = 0; i < 6; ++i) {
            AP ap;
            ap.x = 41.0 + x_offset;
            ap.y = ys[i] + y_offset;
            ap.z = z;
            ap.ch24 = ch24_seq[i];
            ap.ch5 = ch5_seq[i];
            aps.push_back(ap);
        }
    }
    return aps;
}

int calculateWallAttenuation(double apX, double pointX, double apY, double pointY, double apZ, double pointZ) {
    int attenuation = 0;
    if ((apX < 40.0 && pointX > 42.0) || (apX > 42.0 && pointX < 40.0)) {
        attenuation += 6;
    }
    if ((apY < 5.0 && pointY > 5.0) || (apY > 95.0 && pointY < 95.0)) {
        attenuation += 12;
    }
    if ((apX < 5.0 && pointX > 5.0) || (apX > 75.0 && pointX < 75.0)) {
        attenuation += 12;
    }
    int floorDiff = abs(static_cast<int>((apZ - pointZ) / 3.5));
    attenuation += floorDiff * 12;
    return attenuation;
}

double calculateSignalStrength(double d, int wallAttenuation, int freq) {
    double txPower = 20.0;
    double n = 3.5;
    double d0 = 1.0;
    double l0 = 40.0;
    double pl = l0 + 10 * n * log10(d / d0);
    double indoorAttenuation = 0.0;
    if (freq == 5000) {
        pl += 10.0;
    }
    return txPower - pl - wallAttenuation;
}

void generateHeatMap(const vector<AP>& aps) {
    for (double z : {1.75, 5.25, 8.75}) {
        cout << "Floor " << (z == 1.75 ? 1 : z == 5.25 ? 2 : 3) << " Heat Map:" << endl;
        for (int y = 0; y <= 100; y += 5) {
            for (int x = 0; x <= 80; x += 5) {
                double maxSignal = -1000.0;
                for (const AP& ap : aps) {
                    double dx = ap.x - x;
                    double dy = ap.y - y;
                    double dz = ap.z - z;
                    double d = sqrt(dx*dx + dy*dy + dz*dz);
                    if (d < 0.1) d = 0.1;
                    int wallAtten = calculateWallAttenuation(ap.x, x, ap.y, y, ap.z, z);
                    double signal24 = calculateSignalStrength(d, wallAtten, 2400);
                    double signal5 = calculateSignalStrength(d, wallAtten, 5000);
                    double signal = max(signal24, signal5);
                    if (signal > maxSignal) {
                        maxSignal = signal;
                    }
                }
                if (maxSignal >= -65) {
                    cout << setw(4) << "+++";
                } else {
                    cout << setw(4) << "---";
                }
            }
            cout << endl;
        }
        cout << endl;
    }
}

int main() {
    vector<AP> aps = generateAPs();
    cout << "AP Deployment Results:" << endl;
    cout << "AP\tX\tY\tZ\t2.4GHz Channel\t5GHz Channel" << endl;
    for (int i = 0; i < aps.size(); ++i) {
        cout << i+1 << "\t" << aps[i].x << "\t" << aps[i].y << "\t" << aps[i].z << "\t" << aps[i].ch24 << "\t\t" << aps[i].ch5 << endl;
    }
    generateHeatMap(aps);
    cout<<"Press Enter to continue ...."<<endl;
    getchar();
    return 0;
}