#pragma once

#include "decima/serializable/object/resource/resource.hpp"
#include "decima/serializable/array.hpp"
#include "decima/serializable/guid.hpp"
#include "decima/shared.hpp"

namespace Decima {
    enum class VertexElementType : std::uint8_t {
        Pos = 0,
        TangentBFlip = 1,
        Tangent = 2,
        Binormal = 3,
        Normal = 4,
        Color = 5,
        UV0 = 6,
        UV1 = 7,
        UV2 = 8,
        UV3 = 9,
        UV4 = 10,
        UV5 = 11,
        UV6 = 12,
        MotionVec = 13,
        Vec4Byte0 = 14,
        Vec4Byte1 = 15,
        BlendWeights = 16,
        BlendIndices = 17,
        BlendWeights2 = 18,
        BlendIndices2 = 19,
        BlendWeights3 = 20,
        BlendIndices3 = 21,
        PivotPoint = 22,
        AltPos = 23,
        AltTangent = 24,
        AltBinormal = 25,
        AltNormal = 26,
        AltColor = 27,
        AltUV0 = 28,
        Invalid = 29
    };

    enum class VertexElementStorageType : std::uint8_t {
        Undefined = 0,
        SignedShortNormalized = 1,
        Float = 2,
        HalfFloat = 3,
        UnsignedByteNormalized = 4,
        SignedShort = 5,
        XYZ10W2Normalized = 6,
        UnsignedByte = 7,
        UnsignedShort = 8,
        UnsignedShortNormalized = 9,
        UnsignedNormalized8sRGB = 10,
        XYZ10W2UnsignedNormalized = 11
    };

    class VertexStreamData {
    public:
        void parse(ash::buffer& buffer);
        void draw();

    public:
        std::uint8_t offset;
        VertexElementStorageType storage_type;
        std::uint8_t slots_used;
        VertexElementType element_type;
    };

    class VertexStreamInfo {
    public:
        void parse(ash::buffer& buffer);
        void draw();

    public:
        std::uint32_t flags;
        std::uint32_t stride;
        Array<VertexStreamData> descriptors;
        GUID resource_uuid;
    };

    class VertexArrayResource : public Resource {
    public:
        void parse(ArchiveManager& manager, ash::buffer& buffer, CoreFile* core_file) override;
        void draw() override;

    public:
        std::uint32_t vertex_count;
        std::uint32_t vertex_stream_count;
        std::uint8_t is_streaming;
        std::vector<VertexStreamInfo> vertex_stream_info;
    };
}

template <>
inline std::string Decima::to_string(const VertexElementType& value) {
    switch (value) {
    case VertexElementType::Invalid:
        return "Invalid";
    case VertexElementType::Pos:
        return "Position"; 
    case VertexElementType::TangentBFlip:
        return "Tangent binormal flip";
    case VertexElementType::Tangent:
        return "Tangent";
    case VertexElementType::Binormal:
        return "Binormal";
    case VertexElementType::Normal:
        return "Normal";
    case VertexElementType::Color:
        return "Color";
    case VertexElementType::UV0:
    case VertexElementType::UV1:
    case VertexElementType::UV2:
    case VertexElementType::UV3:
    case VertexElementType::UV4:
    case VertexElementType::UV5:
    case VertexElementType::UV6:
        return "UV";
    case VertexElementType::MotionVec:
        return "Motion vector";
    case VertexElementType::Vec4Byte0:
    case VertexElementType::Vec4Byte1:
        return "Vec4";
    case VertexElementType::BlendWeights:
    case VertexElementType::BlendWeights2:
    case VertexElementType::BlendWeights3:
        return "Blend weights";
    case VertexElementType::BlendIndices:
    case VertexElementType::BlendIndices3:
    case VertexElementType::BlendIndices2:
        return "Blend indices";
    case VertexElementType::PivotPoint:
        return "Pivot point";
    case VertexElementType::AltPos:
        return "Alternate position";
    case VertexElementType::AltTangent:
        return "Alternate tangent";
    case VertexElementType::AltBinormal:
        return "Alternate binormal";
    case VertexElementType::AltNormal:
        return "Alternate normal";
    case VertexElementType::AltColor:
        return "Alternate color";
    case VertexElementType::AltUV0:
        return "Alternate UV";
    default:
        return "Unknown: " + std::to_string(static_cast<std::size_t>(value));
    }
}

template <>
inline std::string Decima::to_string(const VertexElementStorageType& value) {
    switch (value) {
    case VertexElementStorageType::Undefined:
        return "Undefined";
    case VertexElementStorageType::UnsignedByteNormalized:
        return "Unsigned Int8 (normalized)";
    case VertexElementStorageType::UnsignedByte:
        return "Unsigned Int8";
    case VertexElementStorageType::SignedShortNormalized:
        return "Signed Int16 (normalized)";
    case VertexElementStorageType::SignedShort:
        return "Signed Int16";
    case VertexElementStorageType::UnsignedShort:
        return "Unsigned Int16";
    case VertexElementStorageType::UnsignedShortNormalized:
        return "Unsigned Int16 (normalized)";
    case VertexElementStorageType::Float:
        return "Float32";
    case VertexElementStorageType::HalfFloat:
        return "Float16";
    case VertexElementStorageType::XYZ10W2Normalized:
        return "Signed XYZ10W2 (normalized)";
    case VertexElementStorageType::XYZ10W2UnsignedNormalized:
        return "Unsigned XYZ10W2 (normalized)";
    case VertexElementStorageType::UnsignedNormalized8sRGB:
        return "Unsigned Int8 sRGB (normalized)";
    default:
        return "Unknown: " + std::to_string(static_cast<std::size_t>(value));
    }
}
