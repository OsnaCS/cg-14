#include "FPSCounter.hpp"
#include "lumina/lumina.hpp"

#include <iomanip>

using namespace lumina;

FPSCounter::FPSCounter() {
	m_sum = 0.f;
}

void FPSCounter::siftValues() {

	for (int i = 0; i < m_values.size() - 1; i++) {
		m_values[i] = m_values[i + 1];
	}

	m_values.pop_back();
}

void FPSCounter::printFPS(float delta) {

	if (m_values.size() == 100) {
		siftValues();
	}

	m_values.push_back(delta);
	m_sum += delta;

	// only print fps after two seconds
  if(m_sum > 2) {

  	// get average
		float average = 0;
		for (float value : m_values) {
			average += value;
		}

  	// calc fps
  	float fps = 1 / (average / m_values.size());	

  	cout << setprecision(4);
    slog("FPS: ", fps);
    m_sum -= 2;
  }
}