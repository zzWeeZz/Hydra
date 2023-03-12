#pragma once


namespace Hydra
{
#pragma region RenderStates
	enum class DepthState
	{
		Off = 0,
		ReadWrite,
		ReadOnly,
		Count
	};

	enum class CullState
	{
		None,
		Front,
		Back,
	};

	enum class Topology
	{
		PointList = 0,
		LineList = 1,
		LineStrip = 2,
		TriangleList = 3,
		TriangleStrip = 4,
		TriangleFan = 5,
		LineListAdj = 6,
		LineStripAdj = 7,
		TriangleListAdj = 8,
		TriangleStripAdj = 9,
		PatchList = 10,
	};
#pragma endregion

#pragma region Samplers

	enum class Filter
	{
		Nearest = 0,
		Linear = 1,
	};
	enum class Address
	{
		Repeat = 0,
		MirroredRepeat = 1,
		ClampToEdge = 2,
		ClampToBorder = 3,
		MirrorClampToEdge = 4,
	};
	enum class MipmapMode
	{
		Nearest = 0,
		Linear = 1,
	};

#pragma endregion

#pragma region ImageFlags
	enum class ImageUsage : size_t
	{
		RenderTarget,
		DepthTarget,
	};

	enum class ImageFormat
	{
		R8UN,
		RGBA8UN,

		R32UI,
		R32F,
		R32S,

		RG32F,
		RG32S,
		RG32UI,

		RGB32F,
		RGB32S,
		RGB32UI,
	
		RGBA32F,
		RGBA32UI,
		RGBA32SI,
		
		RGBA16F,
		RGBA16UN,
		RGBA16UI,
		RGBA16SN,
		RGBA16SI,
		
		Depth32,
		Depth24,
	};

	enum class ImageType
	{
		Image2D,
		Image3D,
		Image2DArray,
		Image3DArray,
		Count,
	};
#pragma endregion
}