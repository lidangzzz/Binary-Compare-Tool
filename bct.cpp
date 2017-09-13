#include <iostream>
#include <fstream>

using namespace::std;
#pragma warning(disable:4996)

const __int64 sz = 50000000;

void compare(char* s1, char* s2) {
	FILE *f1, *f2;
	f1 = fopen(s1, "rb");
	f2 = fopen(s2, "rb");
	unsigned char* m1 = (unsigned char*)calloc(sz, sizeof(unsigned char));
	unsigned char* m2 = (unsigned char*)calloc(sz, sizeof(unsigned char));

	_fseeki64(f1, 0, SEEK_END);
	__int64 size1 = _ftelli64(f1);
	_fseeki64(f1, 0, SEEK_SET);

	_fseeki64(f2, 0, SEEK_END);
	__int64 size2 = _ftelli64(f2);
	_fseeki64(f2, 0, SEEK_SET);

	if (size1 != size2) {
		cout << "Size doesn't match" << endl;
		return;
	}

	int i = 0;
	__int64 z = _ftelli64(f1);
	while (z < size1) {
		fread(m1, sizeof(unsigned char), sz, f1);
		fread(m2, sizeof(unsigned char), sz, f2);
		for (long i = 0; i < sz; i++) {
			if (m1[i] != m2[i]) {
				cout << "Error in " << _ftelli64(f1) + i - sz << endl;
				break;
			}
		}
		cout << "Progress: " << 100.0* z / size1 << "%"<< endl;
		z = _ftelli64(f1);
	}
}

int main(int nNumberofArgs, char* pszArgs[]) {
	if (nNumberofArgs != 3) {
		cout << "Must 2 files" << endl;
		return 0;
	}
	compare(pszArgs[1], pszArgs[2]);
	cout << "The two binary are exactly the same." <<endl;
	return 0;
}
