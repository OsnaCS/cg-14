#pragma once

#include <vector>

using namespace std;

class FPSCounter {
public:
	FPSCounter();
  void printFPS(float delta);

private:
	vector<float> m_values;
	float m_sum;

	void siftValues();
};
