#pragma once

class SimplexNoise
{
public:	
	// 2D simplex noise
	static double noise(double m_xin, double m_yin, int m_seed);

private:
	// Inner class to speed up gradient computations
	// (array access is a lot slower than member access)
	class Grad {
	public:
		double m_x, m_y, m_z, m_w;
	
	public:
		Grad(double m_x, double m_y, double m_z) {
			Grad::m_x = m_x;
			Grad::m_y = m_y;
			Grad::m_z = m_z;
		}

	};

	// Method to calculate 
	static double dot(Grad g, double m_x, double m_y) {
		return g.m_x*m_x + g.m_y*m_y;
	};

	// This method is a *lot* faster than using (int)Math.floor(x)
	static int fastfloor(double m_x) {
		int m_xi = (int)m_x;
		if(m_x < m_xi) {
			return m_xi - 1;
		}
		else {
			return m_xi;
		}
	}
	
};