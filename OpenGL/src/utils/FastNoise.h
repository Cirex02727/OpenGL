// FastNoise.h
//
// MIT License
//
// Copyright(c) 2017 Jordan Peck
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// The developer's email is jorzixdan.me2@gzixmail.com (for great email, take
// off every 'zix'.)
//

// VERSION: 0.4.1

#ifndef FASTNOISE_H
#define FASTNOISE_H

// Uncomment the line below to use doubles throughout FastNoise instead of floats
//#define FN_USE_doubleS

#define FN_CELLULAR_INDEX_MAX 3

class FastNoise
{
public:
	explicit FastNoise(int seed = 1337) { SetSeed(seed); CalculateFractalBounding(); }

	enum NoiseType { Value, ValueFractal, Perlin, PerlinFractal, Simplex, SimplexFractal, Cellular, WhiteNoise, Cubic, CubicFractal };
	enum Interp { Linear, Hermite, Quintic };
	enum FractalType { FBM, Billow, RigidMulti };
	enum CellularDistanceFunction { Euclidean, Manhattan, Natural };
	enum CellularReturnType { CellValue, NoiseLookup, Distance, Distance2, Distance2Add, Distance2Sub, Distance2Mul, Distance2Div };

	// Sets seed used for all noise types
	// Default: 1337
	void SetSeed(int seed);

	// Returns seed used for all noise types
	int GetSeed() const { return m_seed; }

	// Sets frequency for all noise types
	// Default: 0.01
	void SetFrequency(double frequency) { m_frequency = frequency; }

	// Returns frequency used for all noise types
	double GetFrequency() const { return m_frequency; }

	// Changes the interpolation method used to smooth between noise values
	// Possible interpolation methods (lowest to highest quality) :
	// - Linear
	// - Hermite
	// - Quintic
	// Used in Value, Perlin Noise and Position Warping
	// Default: Quintic
	void SetInterp(Interp interp) { m_interp = interp; }

	// Returns interpolation method used for supported noise types
	Interp GetInterp() const { return m_interp; }

	// Sets noise return type of GetNoise(...)
	// Default: Simplex
	void SetNoiseType(NoiseType noiseType) { m_noiseType = noiseType; }

	// Returns the noise type used by GetNoise
	NoiseType GetNoiseType() const { return m_noiseType; }

	// Sets octave count for all fractal noise types
	// Default: 3
	void SetFractalOctaves(int octaves) { m_octaves = octaves; CalculateFractalBounding(); }

	// Returns octave count for all fractal noise types
	int GetFractalOctaves() const { return m_octaves; }
	
	// Sets octave lacunarity for all fractal noise types
	// Default: 2.0
	void SetFractalLacunarity(double lacunarity) { m_lacunarity = lacunarity; }

	// Returns octave lacunarity for all fractal noise types
	double GetFractalLacunarity() const { return m_lacunarity; }

	// Sets octave gain for all fractal noise types
	// Default: 0.5
	void SetFractalGain(double gain) { m_gain = gain; CalculateFractalBounding(); }

	// Returns octave gain for all fractal noise types
	double GetFractalGain() const { return m_gain; }

	// Sets method for combining octaves in all fractal noise types
	// Default: FBM
	void SetFractalType(FractalType fractalType) { m_fractalType = fractalType; }

	// Returns method for combining octaves in all fractal noise types
	FractalType GetFractalType() const { return m_fractalType; }


	// Sets distance function used in cellular noise calculations
	// Default: Euclidean
	void SetCellularDistanceFunction(CellularDistanceFunction cellularDistanceFunction) { m_cellularDistanceFunction = cellularDistanceFunction; }

	// Returns the distance function used in cellular noise calculations
	CellularDistanceFunction GetCellularDistanceFunction() const { return m_cellularDistanceFunction; }

	// Sets return type from cellular noise calculations
	// Note: NoiseLookup requires another FastNoise object be set with SetCellularNoiseLookup() to function
	// Default: CellValue
	void SetCellularReturnType(CellularReturnType cellularReturnType) { m_cellularReturnType = cellularReturnType; }

	// Returns the return type from cellular noise calculations
	CellularReturnType GetCellularReturnType() const { return m_cellularReturnType; }

	// Noise used to calculate a cell value if cellular return type is NoiseLookup
	// The lookup value is acquired through GetNoise() so ensure you SetNoiseType() on the noise lookup, value, Perlin or simplex is recommended
	void SetCellularNoiseLookup(FastNoise* noise) { m_cellularNoiseLookup = noise; }

	// Returns the noise used to calculate a cell value if the cellular return type is NoiseLookup
	FastNoise* GetCellularNoiseLookup() const { return m_cellularNoiseLookup; }

	// Sets the 2 distance indices used for distance2 return types
	// Default: 0, 1
	// Note: index0 should be lower than index1
	// Both indices must be >= 0, index1 must be < 4
	void SetCellularDistance2Indices(int cellularDistanceIndex0, int cellularDistanceIndex1);

	// Returns the 2 distance indices used for distance2 return types
	void GetCellularDistance2Indices(int& cellularDistanceIndex0, int& cellularDistanceIndex1) const;

	// Sets the maximum distance a cellular point can move from its grid position
	// Setting this high will make artifacts more common
	// Default: 0.45
	void SetCellularJitter(double cellularJitter) { m_cellularJitter = cellularJitter; }

	// Returns the maximum distance a cellular point can move from its grid position
	double GetCellularJitter() const { return m_cellularJitter; }

	// Sets the maximum warp distance from original location when using GradientPerturb{Fractal}(...)
	// Default: 1.0
	void SetGradientPerturbAmp(double gradientPerturbAmp) { m_gradientPerturbAmp = gradientPerturbAmp; }

	// Returns the maximum warp distance from original location when using GradientPerturb{Fractal}(...)
	double GetGradientPerturbAmp() const { return m_gradientPerturbAmp; }

	//2D
	double GetPerlin(double x, double y) const;
	double GetPerlinFractal(double x, double y) const;

	double GetSimplex(double x, double y) const;
	double GetSimplexFractal(double x, double y) const;

	double GetNoise(double x, double y) const;

	//3D
	double GetPerlin(double x, double y, double z) const;
	double GetPerlinFractal(double x, double y, double z) const;

	double GetSimplex(double x, double y, double z) const;
	double GetSimplexFractal(double x, double y, double z) const;

	double GetNoise(double x, double y, double z) const;

	//4D
	double GetSimplex(double x, double y, double z, double w) const;

private:
	unsigned char m_perm[512];
	unsigned char m_perm12[512];

	int m_seed = 1337;
	double m_frequency = double(0.01);
	Interp m_interp = Quintic;
	NoiseType m_noiseType = Simplex;

	int m_octaves = 3;
	double m_lacunarity = double(2);
	double m_gain = double(0.5);
	FractalType m_fractalType = FBM;
	double m_fractalBounding;

	CellularDistanceFunction m_cellularDistanceFunction = Euclidean;
	CellularReturnType m_cellularReturnType = CellValue;
	FastNoise* m_cellularNoiseLookup = nullptr;
	int m_cellularDistanceIndex0 = 0;
	int m_cellularDistanceIndex1 = 1;
	double m_cellularJitter = double(0.45);

	double m_gradientPerturbAmp = double(1);

	void CalculateFractalBounding();

	//2D
	double SinglePerlinFractalFBM(double x, double y) const;
	double SinglePerlinFractalBillow(double x, double y) const;
	double SinglePerlinFractalRigidMulti(double x, double y) const;
	double SinglePerlin(unsigned char offset, double x, double y) const;

	double SingleSimplexFractalFBM(double x, double y) const;
	double SingleSimplexFractalBillow(double x, double y) const;
	double SingleSimplexFractalRigidMulti(double x, double y) const;
	double SingleSimplexFractalBlend(double x, double y) const;
	double SingleSimplex(unsigned char offset, double x, double y) const;

	double GradCoord2D(unsigned char offset, int x, int y, double xd, double yd) const;
	unsigned char Index2D_256(unsigned char offset, int x, int y) const;
	unsigned char Index2D_12(unsigned char offset, int x, int y) const;
	double ValCoord2DFast(unsigned char offset, int x, int y) const;

	//3D
	double SinglePerlinFractalFBM(double x, double y, double z) const;
	double SinglePerlinFractalBillow(double x, double y, double z) const;
	double SinglePerlinFractalRigidMulti(double x, double y, double z) const;
	double SinglePerlin(unsigned char offset, double x, double y, double z) const;

	double SingleSimplexFractalFBM(double x, double y, double z) const;
	double SingleSimplexFractalBillow(double x, double y, double z) const;
	double SingleSimplexFractalRigidMulti(double x, double y, double z) const;
	double SingleSimplex(unsigned char offset, double x, double y, double z) const;

	double GradCoord3D(unsigned char offset, int x, int y, int z, double xd, double yd, double zd) const;
	unsigned char Index3D_256(unsigned char offset, int x, int y, int z) const;
	unsigned char Index3D_12(unsigned char offset, int x, int y, int z) const;
	double ValCoord3DFast(unsigned char offset, int x, int y, int z) const;

	//4D
	double SingleSimplex(unsigned char offset, double x, double y, double z, double w) const;

	double GradCoord4D(unsigned char offset, int x, int y, int z, int w, double xd, double yd, double zd, double wd) const;
	unsigned char Index4D_32(unsigned char offset, int x, int y, int z, int w) const;
};
#endif
