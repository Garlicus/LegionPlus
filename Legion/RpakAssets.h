#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "ListBase.h"
#include <d3dcommon.h>
#include "Utils.h"

// Game structures
// APEX
#pragma pack(push, 1)
union RPakPtr
{
	struct {
		uint32_t Index;
		uint32_t Offset;
	};
	uint64_t Value;
};

enum class CompressionType : uint8_t
{
	DEFAULT = 0x0,
	PAKFILE = 0x1,
	SNOWFLAKE = 0x2,
	OODLE = 0x3
};

struct TextureHeader
{
	uint64_t NameHash;
	uint32_t NameIndex;
	uint32_t NameOffset;

	uint16_t Width;
	uint16_t Height;

	uint8_t Un1;
	uint8_t Un2;
	uint8_t Format;				  // used as an index into an array of DXGI formats
	uint8_t Un3;

	uint32_t DataSize;	          // total data size across all mips
	uint8_t Unknown2;
	uint8_t MipLevelsStreamedOpt; // mips stored in .opt.starpak
	uint8_t ArraySize;
	uint8_t LayerCount;
	uint8_t Unknown4;
	uint8_t MipLevelsPermanent;   // mips stored in .rpak
	uint8_t MipLevelsStreamed;    // mips stored in .starpak

	uint8_t UnknownPad[0x15];
};

struct TextureHeaderV9
{
	uint32_t NameIndex;
	uint32_t NameOffset;
	uint16_t Format;
	uint16_t Width;
	uint16_t Height;
	uint16_t Un1;
	uint8_t ArraySize;
	uint8_t byte11;
	uint8_t gap12;
	uint8_t char13;
	uint32_t DataSize;
	uint8_t MipLevels;
	uint8_t MipLevelsStreamed;
	uint8_t MipLevelsStreamedOpt;
};

// --- txan ---
struct TextureAnimatedHeader
{
	uint32_t Index1;
	uint32_t Offset1;

	uint32_t Index2;
	uint32_t Offset2;

	uint32_t Index3;
	uint32_t Offset3;
};

// --- uiia ---
struct UIIAFlags
{
	uint8_t HasLowTable : 1;
	uint8_t LowTableBc7 : 1;
	uint8_t HasHighTable : 1;
	uint8_t HighTableBc7 : 1;
	// 0x1 = RpakDecompress
	// 0x2 = RpakDecompressSnowflake
	uint8_t CompressionType : 2;
	uint8_t SizeShift : 2;
};

struct UIIAHeader
{
	uint64_t Zero;
	uint64_t NegativeOne;

	uint16_t AtlasCount;

	UIIAFlags Flags;
	uint8_t Padding;
	uint16_t Unknown;
	uint16_t NegativeOne2;

	uint16_t Width;
	uint16_t Height;

	uint32_t Padding2;

	float FloatTable[0x8];
};

struct RUIImage
{
	uint32_t Zero[0x8];

	uint16_t HighResolutionWidth;
	uint16_t HighResolutionHeight;

	uint16_t LowResolutionWidth;
	uint16_t LowResolutionHeight;

	uint32_t BufferIndex;
	uint32_t BufferOffset;

	uint32_t NameIndex;
	uint32_t NameOffset;

	uint32_t Zero2[0x2];
};

// --- dtbl ---
enum DataTableColumnDataType
{
	Bool,
	Int,
	Float,
	Vector,
	StringT,
	Asset,
	AssetNoPrecache
};

struct DataTableHeader
{
	uint32_t ColumnCount;
	uint32_t RowCount;

	uint32_t ColumnHeaderBlock;
	uint32_t ColumnHeaderOffset;
	uint32_t RowHeaderBlock;
	uint32_t RowHeaderOffset;
	uint32_t UnkHash;
	
	uint16_t Un1;
	uint16_t Un2;

	uint32_t RowStride;	// Number of bytes per row
	uint32_t Padding;
};

struct DataTableColumn
{
	uint64_t Unk0Seek;
	uint64_t Unk8;
	uint32_t Type;
	uint32_t RowOffset;
};

struct DataTableColumnData
{
	DataTableColumnDataType Type;
	bool bValue = 0;
	int iValue = -1;
	float fValue = -1;
	Math::Vector3 vValue;
	string stringValue;
	string assetValue;
	string assetNPValue;
};

// --- subt ---
struct SubtitleHeader
{
	char padding[0x10];

	uint32_t EntriesIndex;
	uint32_t EntriesOffset;
};

struct SubtitleEntry
{
	Math::Vector3 Color;
	string SubtitleText;
};

// settings structs are only tested on season 3 for now
enum class SettingsFieldType : uint16_t
{
	ST_Bool,
	ST_Int,
	ST_Float,
	ST_Float2,
	ST_Float3,
	ST_String,
	ST_Asset,
	ST_Asset_2,
	ST_Array,
	ST_Array_2,
};

// --- stgs ---
struct SettingsHeader
{
	uint64_t LayoutGUID;

	RPakPtr Values;

	RPakPtr Name;

	RPakPtr StringBuf;

	uint64_t Unk1;

	RPakPtr ModNames;

	RPakPtr Unk2;

	uint32_t KvpBufferSize;

	uint8_t Unk3[0xC];
};
ASSERT_SIZE(SettingsHeader, 0x48);

struct SettingsKeyValue
{
	uint32_t BlockIndex;
	uint32_t BlockOffset;
};

struct SettingsKeyValuePair
{
	SettingsKeyValue Key;
	SettingsKeyValue Value;
};

struct SettingsLayoutHeader
{
	RPakPtr pName;
	RPakPtr pItems;
	RPakPtr unk2;
	uint32_t unk3;
	uint32_t itemsCount;
	uint32_t unk4;
	uint32_t unk5;
	uint32_t unk6;
	uint32_t unk7;
	uint32_t unk8;
	uint32_t unk9;
	RPakPtr pStringBuf;
	RPakPtr unk11;
};

struct SettingsLayoutItem
{
	SettingsFieldType type = SettingsFieldType::ST_String;
	string name;
	uint32_t valueOffset; // offset from start of stgs value buffer
};

struct SettingsLayout
{
	string name;
	unsigned int itemsCount;
	List<SettingsLayoutItem> items;
};

// --- Ptch ---
struct PatchHeader
{
	uint32_t Version;
	uint32_t Count;

	uint32_t EntriesIndex;
	uint32_t EntriesOffset;

	uint32_t PatchesIndex;
	uint32_t PatchesOffset;
};

// ANIMATIONS
// --- aseq ---
struct AnimHeader
{
	uint32_t AnimationIndex;
	uint32_t AnimationOffset;

	uint32_t NameIndex;
	uint32_t NameOffset;

	uint8_t Unknown[0x18];

	uint32_t ModelHashIndex;
	uint32_t ModelHashOffset;
};

// --- arig ---
struct AnimRigHeader
{
	uint32_t SkeletonIndex;
	uint32_t SkeletonOffset;

	uint32_t NameIndex;
	uint32_t NameOffset;

	uint32_t Unk1;
	uint32_t AnimationReferenceCount;

	uint32_t AnimationReferenceIndex;
	uint32_t AnimationReferenceOffset;

	uint32_t Unk3;
	uint32_t Unk4;
};

struct RAnimHeader
{
	uint32_t Zero;
	uint32_t NameOffset;
	uint32_t SourceOffset;
	uint32_t Flags;

	uint64_t Unknown;

	uint32_t NotetrackCount;
	uint32_t NotetrackOffset;

	float Mins[3];
	float Maxs[3];

	uint32_t AnimationCount;
	uint32_t AnimationOffset;
};

struct RAnimTitanfallHeader
{
	uint32_t Zero;
	uint32_t NameOffset;

	float Framerate;

	uint32_t Flags;
	uint32_t FrameCount;

	uint32_t Zero1;
	uint32_t Zero2;

	uint32_t UnknownOffset;
	uint32_t FirstChunkOffset;

	uint32_t UnknownCount2;
	uint32_t UnknownOffset2;

	uint32_t Zero3;
	uint32_t Zero4;

	uint32_t OffsetToChunkOffsetsTable;
	uint32_t FrameSplitCount;

	uint8_t UnknownZero[0x20];
};

struct RAnimSequenceHeader
{
	uint32_t Zero;
	uint32_t NameOffset;

	float Framerate;

	uint32_t Flags;
	uint32_t FrameCount;

	uint64_t UnknownZero;

	uint32_t FloatTableOffset;
	uint32_t FirstChunkOffset;
	uint32_t Flags2;
	uint32_t UnknownTableOffset;
	uint32_t OffsetToChunkOffsetsTable;
	uint32_t FrameSplitCount;
	uint32_t FrameMedianCount;
	uint64_t Padding;
	uint64_t SomeDataOffset;
};

struct RAnimBoneFlag
{
	uint16_t Size : 12;
	uint16_t bAdditiveCustom : 1;
	uint16_t bDynamicScale : 1;			// If zero, one per data set
	uint16_t bDynamicRotation : 1;		// If zero, one per data set
	uint16_t bDynamicTranslation : 1;	// If zero, one per data set
};

struct RAnimTitanfallBoneFlag
{
	uint8_t Unused : 1;
	uint8_t bStaticTranslation : 1;		// If zero, one per data set
	uint8_t bStaticRotation : 1;		// If zero, one per data set
	uint8_t bStaticScale : 1;			// If zero, one per data set
	uint8_t Unused2 : 1;
	uint8_t Unused3 : 1;
	uint8_t Unused4 : 1;
};

struct RAnimBoneHeader
{
	float TranslationScale;

	uint8_t BoneIndex;
	RAnimTitanfallBoneFlag BoneFlags;
	uint8_t Flags2;
	uint8_t Flags3;
	
	union
	{
		struct
		{
			uint16_t OffsetX;
			uint16_t OffsetY;
			uint16_t OffsetZ;
			uint16_t OffsetL;
		};

		uint64_t PackedRotation;
	} RotationInfo;

	uint16_t TranslationX;
	uint16_t TranslationY;
	uint16_t TranslationZ;

	uint16_t ScaleX;
	uint16_t ScaleY;
	uint16_t ScaleZ;

	uint32_t DataSize;
};

// MODELS
// --- mdl_ ---
struct ModelHeaderS50
{
	// .rmdl
	uint32_t SkeletonIndex;
	uint32_t SkeletonOffset;

	uint32_t NameIndex;
	uint32_t NameOffset;

	uint64_t Padding3;

	// .phy
	uint32_t PhyIndex;
	uint32_t PhyOffset;

	// .vvd
	uint32_t BlockIndex1;
	uint32_t BlockOffset1;
	uint64_t Padding4;

	uint32_t Padding5;
	uint32_t StreamedDataSize;
	uint32_t DataFlags;
	uint64_t Padding6;

	uint32_t AnimSequenceCount;

};

struct ModelHeaderS68
{
	// .rmdl
	uint32_t SkeletonIndex;
	uint32_t SkeletonOffset;
	uint64_t Padding;

	uint32_t NameIndex;
	uint32_t NameOffset;
	uint64_t Padding2;

	// .phy
	uint32_t PhyIndex;
	uint32_t PhyOffset;
	uint64_t Padding3;

	// .vvd
	uint32_t BlockIndex1;
	uint32_t BlockOffset1;
	uint64_t Padding4;

	uint32_t Padding5;
	uint32_t StreamedDataSize;
	uint32_t DataFlags;
	uint64_t Padding6;

	uint32_t AnimSequenceCount;
	uint32_t AnimSequenceIndex;
	uint32_t AnimSequenceOffset;

	uint64_t Padding7;
};

struct ModelHeaderS80
{
	// .rmdl
	uint32_t SkeletonIndex;
	uint32_t SkeletonOffset;
	uint64_t Padding;

	uint32_t NameIndex;
	uint32_t NameOffset;
	uint64_t Padding2;

	// .phy
	uint32_t PhyIndex;
	uint32_t PhyOffset;
	uint64_t Padding3;

	// .vvd
	// this pointer is not always registered
	// similar data will often be streamed from a mandatory starpak
	uint32_t VGIndex1;
	uint32_t VGOffset1;
	uint64_t Padding4;

	uint32_t Padding5;
	uint32_t StreamedDataSize;
	uint32_t DataFlags;

	float Box[6];
	uint64_t Padding6;

	uint32_t AnimSequenceCount;
	uint32_t AnimSequenceIndex;
	uint32_t AnimSequenceOffset;

	uint64_t Padding7;
};

// MATERIALS
// --- matl ---
struct MaterialHeader
{
	uint8_t Unknown[0x10];
	uint64_t Hash;

	uint32_t NameIndex;
	uint32_t NameOffset;
	uint32_t TypeIndex;
	uint32_t TypeOffset;
	uint32_t SurfaceIndex;
	uint32_t SurfaceOffset;

	uint8_t Unknown2[0x28];
	uint64_t ShaderSetHash;

	uint32_t TexturesIndex;
	uint32_t TexturesOffset;
	// Also Texture pointer, but less relevant for now.
	uint32_t StreamableTexturesIndex;
	uint32_t StreamableTexturesOffset;

	int16_t StreamableTextureCount;
	int16_t Width;
	int16_t Height;
	int16_t Unused;
	uint32_t ImageFlags; // Image Flags, they decide the tiling, stretching etc.

	uint8_t Unknown3[0x1C];

	uint32_t TexturesTFIndex;
	uint32_t TexturesTFOffset;
	uint32_t UnknownTFIndex;
	uint32_t UnknownTFOffset;
};

// structs taken from repak - thanks Rika
struct UnknownMaterialSectionV12
{
	// not sure how these work but 0xF0 -> 0x00 toggles them off and vice versa.
	// they seem to affect various rendering filters, said filters might actually be the used shaders.
	// the duplicate one is likely for the second set of textures which (probably) never gets used.
	uint32_t UnkRenderLighting;
	uint32_t UnkRenderAliasing;
	uint32_t UnkRenderDoF;
	uint32_t UnkRenderUnknown;

	uint32_t UnkRenderFlags; // this changes sometimes.
	uint16_t VisibilityFlags; // different render settings, such as opacity and transparency.
	uint16_t FaceDrawingFlags; // how the face is drawn, culling, wireframe, etc.

	uint64_t Padding;
};

struct MaterialHeaderV12
{
	uint64_t m_VtblReserved; // Gets set to CMaterialGlue vtbl ptr
	uint8_t m_Padding[0x8]; // unused

	uint64_t m_nGUID; // guid of this material asset

	RPakPtr m_pszName; // pointer to partial asset path
	RPakPtr m_pszSurfaceProp; // pointer to surfaceprop (as defined in surfaceproperties.txt)
	RPakPtr m_pszSurfaceProp2; // pointer to surfaceprop2 

	// IDX 1: DepthShadow
	// IDX 2: DepthPrepass
	// IDX 3: DepthVSM
	// IDX 4: ColPass
	// Titanfall is does not have 'DepthShadowTight'

	uint64_t m_GUIDRefs[4]; // Required to have proper textures.

	// these blocks dont seem to change often but are the same?
	// these blocks relate to different render filters and flags. still not well understood.
	UnknownMaterialSectionV12 m_UnknownSections[2];

	uint64_t m_pShaderSet; // guid of the shaderset asset that this material uses

	RPakPtr m_pTextureHandles; // TextureGUID Map 1

	// should be reserved - used to store the handles for any textures that have streaming mip levels
	RPakPtr m_pStreamingTextureHandles;

	int16_t m_nStreamingTextureHandleCount; // Number of textures with streamed mip levels.
	uint32_t m_Flags; // see ImageFlags in the apex struct.
	int16_t m_Unk1; // might be "m_Unknown2"

	uint64_t m_Padding1; // haven't observed anything here, however I really doubt this is actually padding.

	// seems to be 0xFBA63181 for loadscreens
	uint32_t m_Unknown3; // name carried over from apex struct.

	uint32_t m_Unk2; // this might actually be "m_Unknown4"

	uint32_t m_Flags2;
	uint32_t something2; // seems mostly unchanged between all materials, including apex, however there are some edge cases where this is 0x0.

	int16_t m_nWidth;
	int16_t m_nHeight;
	uint32_t m_Unk3; // might be padding but could also be something else such as "m_Unknown1"?.

	/* ImageFlags
	0x050300 for loadscreens, 0x1D0300 for normal materials.
	0x1D has been observed, seems to invert lighting? used on some exceptionally weird materials.*/
};

// Credits to IJARika
// the following two structs are found in the ""cpu data"", they are very much alike to what you would use in normal source materials.
// apex probably has these and more stuff.
struct UVTransformMatrix
{
	// this section is actually broken up into three parts.
	// c_uvRotScaleX
	float uvScaleX = 1.0;
	float uvRotationX = 0.0; // rotation, but w e i r d.
	// c_uvRotScaleY
	float uvRotationY = -0.0; //counter clockwise, 0-1, exceeding one causes Weird Stuff to happen.
	float uvScaleY = 1.0;
	// c_uvTranslate
	float uvTranslateX = 0.0;
	float uvTranslateY = 0.0;
};

// Credits to IJARika
struct MaterialCPUData
{
	UVTransformMatrix c_uv1; // detail
	UVTransformMatrix c_uv2; // 1st texture (unconfirmed)
	UVTransformMatrix c_uv3; // 2nd texture (unconfirmed)
	UVTransformMatrix c_uv4;
	UVTransformMatrix c_uv5;

	Vector2 c_uvDistortionIntensity;
	Vector2 c_uvDistortion2Intensity;

	float c_L0_scatterDistanceScale = 0.166667;

	float c_layerBlendRamp = 0.0;

	float c_opacity = 0.0;

	float c_useAlphaModulateSpecular = 0.0;
	float c_alphaEdgeFadeExponent = 0.0;
	float c_alphaEdgeFadeInner = 0.0;
	float c_alphaEdgeFadeOuter = 0.0;

	float c_useAlphaModulateEmissive = 1.0;
	float c_emissiveEdgeFadeExponent = 0.0;
	float c_emissiveEdgeFadeInner = 0.0;
	float c_emissiveEdgeFadeOuter = 0.0;

	float c_alphaDistanceFadeScale = 10000.0;
	float c_alphaDistanceFadeBias = -0.0;
	float c_alphaTestReference = 0.0;

	float c_aspectRatioMulV = 1.778;

	float c_shadowBias = 0.0;
	float c_shadowBiasStatic = 0.0;

	float c_dofOpacityLuminanceScale = 1.0;

	float c_tsaaDepthAlphaThreshold = 0.0;
	float c_tsaaMotionAlphaThreshold = 0.9;
	float c_tsaaMotionAlphaRamp = 10.0;
	uint32_t c_tsaaResponsiveFlag = 0x0; // this is 0 or 1 I think.

	Vector3 c_outlineColorSDF = { 0.0, 0.0, 0.0 };
	float c_outlineWidthSDF = 0.0;

	Vector3 c_shadowColorSDF = { 0.0, 0.0, 0.0 };
	float c_shadowWidthSDF = 0.0;

	Vector3 c_insideColorSDF = { 0.0, 0.0, 0.0 };

	float c_outsideAlphaScalarSDF = 0.0;

	float c_glitchStrength = 0.0;

	float c_vertexDisplacementScale = 0.0;

	float c_innerFalloffWidthSDF = 0.0;
	float c_innerEdgeOffsetSDF = 0.0;

	Vector2 c_dropShadowOffsetSDF = { 0.0, 0.0 };

	float c_normalMapEdgeWidthSDF = 0.0;

	float c_shadowFalloffSDF = 0.0;

	Vector2 c_L0_scatterAmount = { 0.0, 0.0 };
	float c_L0_scatterRatio = 0.0;

	float c_L0_transmittanceIntensityScale = 1.0;

	Vector2 c_vertexDisplacementDirection = { 0.0, 0.0 };

	float c_L0_transmittanceAmount = 0.0;
	float c_L0_transmittanceDistortionAmount = 0.5;

	float c_zUpBlendingMinAngleCos = 1.0;
	float c_zUpBlendingMaxAngleCos = 1.0;
	float c_zUpBlendingVertexAlpha = 0.0;

	Vector3 c_L0_albedoTint = { 1.0, 1.0, 1.0 };

	float c_depthBlendScalar = 1.0;

	Vector3 c_L0_emissiveTint = { 0.0, 0.0, 0.0 };

	float c_subsurfaceMaterialID = 0.0;

	Vector3 c_L0_perfSpecColor = { 0.0379723, 0.0379723, 0.0379723 };

	float c_L0_perfGloss = 1.0;

	Vector3 c_L1_albedoTint = { 0.0, 0.0, 0.0 };

	float c_L1_perfGloss = 0.0;

	Vector3 c_L1_emissiveTint = { 0.0, 0.0, 0.0 };
	Vector3 c_L1_perfSpecColor = { 0.0, 0.0, 0.0 };

	float c_splineMinPixelPercent = 0.0;

	Vector2 c_L0_anisoSpecCosSinTheta = { 1.0, 0.0 };
	Vector2 c_L1_anisoSpecCosSinTheta = { 1.0, 0.0 };

	float c_L0_anisoSpecStretchAmount = 0.0;
	float c_L1_anisoSpecStretchAmount = 0.0;

	float c_L0_emissiveHeightFalloff = 0.0;
	float c_L1_emissiveHeightFalloff = 0.0;

	float c_L1_transmittanceIntensityScale = 0.0;
	float c_L1_transmittanceAmount = 0.0;
	float c_L1_transmittanceDistortionAmount = 0.0;

	float c_L1_scatterDistanceScale = 0.0;
	Vector3 c_L1_scatterAmount = { 0.0, 0.0, 0.0 };
	float c_L1_scatterRatio = 0.0;

	float ScatterAmountAndRatio1[4];
	float CharacterBoostTintAndFogScale[4];
	float EdgeDetectOutlineColorAndAlpha[4];
	float EdgeDetectOutlineWidth;
	float EdgeDetectOutlineFalloffExp;
	float CharacterBoostScale;
	float CharacterBoostBias;
	float AlphaErosionHardnessAndc_iridescentViewFacingColor[4];
	float IridescentViewFacingFalloffAndBufferPadding[4];
};

struct MaterialCPUHeader
{
	RPakPtr  m_nData;
	uint32_t m_nDataSize;
	uint32_t m_nVersionMaybe;
};

struct StarpakStreamEntry
{
	uint64_t Offset;
	uint64_t Size;
};

// SHADERS
// --- shdr ---
struct ShaderHeader
{
	uint32_t NameIndex;
	uint32_t NameOffset;
	uint64_t DataSize;
	uint64_t Padding2;

	uint32_t Index1;
	uint32_t Offset1;

	uint32_t Index2;
	uint32_t Offset2;
};

struct RShaderImage
{
	uint32_t DataIndex;
	uint32_t DataOffset;
	uint32_t DataSize;
	uint32_t Size2;
	uint32_t DataIndex2;
	uint32_t DataOffset2;
};

// --- shds ---
struct ShaderSetHeader {
	uint64_t VTablePadding;
	uint32_t NameIndex;
	uint32_t NameOffset;
	uint8_t Unknown1[0x8];
	uint16_t Count1;
	uint16_t TextureInputCount;
	uint16_t Count3;
	uint8_t Byte1;
	uint8_t Byte2;

	uint8_t Unknown2[0x10];

	uint64_t OldVertexShaderHash;
	uint64_t OldPixelShaderHash;

	// only used for version 12+
	uint64_t VertexShaderHash;
	uint64_t PixelShaderHash;
	uint64_t PixelShaderHashTF;

};

struct ShaderSetHeaderTF {
	uint64_t VTablePadding;
	uint32_t NameIndex;
	uint32_t NameOffset;
	uint8_t Unknown1[0x8];
	uint16_t Count1;
	uint16_t TextureInputCount;
	uint16_t Count3;
	uint8_t Byte1;
	uint8_t Byte2;

	uint8_t Unknown2[0x28];

	// only used for version 12+
	uint64_t VertexShaderHash;
	uint64_t PixelShaderHash;
};


struct ShaderDataHeader
{
	uint32_t ByteCodeIndex;
	uint32_t ByteCodeOffset;
	uint32_t DataSize;
};

// DX Shader Types
enum ShaderType : uint16_t
{
	ComputeShader = 0x4353,
	DomainShader = 0x4453,
	GeometryShader = 0x4753,
	HullShader = 0x4853,
	VertexShader = 0xFFFE,
	PixelShader = 0xFFFF,
};

// shader "DXBC" header
struct DXBCHeader
{
	char FourCC[4];
	uint32_t Checksum[4];
	uint32_t One;
	uint32_t DataSize;
	uint32_t ChunkCount;
};

struct RDefHeader
{
	uint32_t Magic; // RDEF
	uint32_t DataSize;
	uint32_t ConstBufferCount;
	uint32_t ConstBufferOffset;
	uint32_t ResBindingCount;
	uint32_t ResBindingOffset;
	uint8_t  MinorVersion;
	uint8_t  MajorVersion;
	ShaderType ShaderType;
	uint32_t Flags;
	uint32_t CompilerStringOffset;
};

struct RDefConstBuffer
{
	uint32_t NameOffset;

	uint32_t VariableCount;
	uint32_t VariableOffset;
	uint32_t DataSize;
	uint32_t Flags;
	uint32_t BufferType;
};

struct RDefCBufVar
{
	uint32_t NameOffset;
	uint32_t CBufStartOffset;
	uint32_t Size;
	uint32_t Flags;
	uint32_t TypeOffset;
	uint32_t DefaultOffset; // offset to the default value of this var
	uint32_t unk[4];
};

struct RDefCBufVarType
{
	uint16_t Class;
	uint16_t Type;
	uint16_t MatrixRows;
	uint16_t MatrixColumns;
	uint16_t ArraySize;
	uint16_t StructMemberCount;
	uint16_t FirstMemberOffset;
};

struct RDefResBinding
{
	uint32_t NameOffset;
	D3D_SHADER_INPUT_TYPE InputType;
	D3D_RESOURCE_RETURN_TYPE ReturnType;
	uint32_t ViewDimension;
	uint32_t SampleCount;
	uint32_t BindPoint;
	uint32_t BindCount;
	D3D_SHADER_INPUT_FLAGS InputFlags;
};

struct ShaderVar
{
	string Name;
	D3D_SHADER_VARIABLE_TYPE Type;
	int Size;
};

struct ShaderResBinding
{
	string Name;
	D3D_SHADER_INPUT_TYPE Type;
};

// uimg - ui image atlas
struct UIAtlasHeader
{
	uint64_t Unk0;
	uint16_t Width; // full dimensions of the atlas texture
	uint16_t Height;
	uint16_t TexturesCount; // number of textures
	uint16_t UnkE;
	uint32_t TextureOffsetsIndex; // texture "offsets" (idrk what this actually does tbh)
	uint32_t TextureOffsetsOffset;
	uint32_t TextureDimsIndex; // texture dimensions
	uint32_t TextureDimsOffset;
	uint32_t Unk20;
	uint32_t Unk24;
	uint32_t TextureHashesIndex; // texture hashes
	uint32_t TextureHashesOffset;
	uint32_t TextureNamesIndex; // texture paths ( not always present )
	uint32_t TextureNamesOffset;
	uint64_t TextureGuid; // asset guid for the texture that contains our images
};

struct UIAtlasUV
{
	float uv0x; // top left corner
	float uv0y;

	float uv1x;
	float uv1y;
};

struct UIAtlasOffset
{
	// these don't seem to matter all that much as long as they are a valid float number
	float f0 = 0.f;
	float f1 = 0.f;

	// endX and endY define where the edge of the image is, with 1.f being the full length of the image and 0.5f being half of the image
	float endX = 1.f;
	float endY = 1.f;

	// startX and startY define where the top left corner is in proportion to the full image dimensions
	float startX = 0.f;
	float startY = 0.f;

	// changing these 2 values causes the image to be distorted on each axis
	float unkX = 1.f;
	float unkY = 1.f;
};

struct UIAtlasImage // uiai wen
{
	string Path;
	uint32_t Hash;
	uint32_t PathTableOffset;

	UIAtlasOffset offsets; // idk anymore
	UIAtlasUV uvs;

	uint16_t Width;
	uint16_t Height;

	uint16_t PosX; // top left corner's absolute position in the atlas texture
	uint16_t PosY;
};

// --- rson ---
#define RSON_STRING 0x2
#define RSON_OBJECT 0x8
#define RSON_BOOLEAN 0x10
#define RSON_INTEGER 0x20
#define RSON_ARRAY 0x1000

struct RSONHeader
{
	int type;
	int nodeCount;
	RPakPtr pNodes;
};

struct RSONNode
{
	RPakPtr pName;
	int type;
	int valueCount;
	RPakPtr pValues;
};
#pragma pack(pop)


// Validate all game structures
// APEX
ASSERT_SIZE(TextureHeader, 0x38);
ASSERT_SIZE(DataTableHeader, 0x28);
ASSERT_SIZE(DataTableColumn, 0x18);
ASSERT_SIZE(SubtitleHeader, 0x18);
ASSERT_SIZE(SettingsHeader, 0x48);
ASSERT_SIZE(PatchHeader, 0x18);
ASSERT_SIZE(UIIAHeader, 0x40);
ASSERT_SIZE(SettingsKeyValue, 0x8);
ASSERT_SIZE(SettingsKeyValuePair, 0x10);
ASSERT_SIZE(ModelHeaderS68, 0x68);
ASSERT_SIZE(ModelHeaderS80, 0x80);
ASSERT_SIZE(AnimHeader, 0x30);
ASSERT_SIZE(AnimRigHeader, 0x28);
ASSERT_SIZE(RAnimHeader, 0x40);

struct RUIImageTile
{
	uint32_t Offset : 24;
	uint32_t Opcode : 8;
};