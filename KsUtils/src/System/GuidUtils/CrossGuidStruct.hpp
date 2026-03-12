#ifndef CROSS_GUID_STRUCT_HPP
#define CROSS_GUID_STRUCT_HPP

template <typename GuidPlatformPolicy>
class CrossGuidStruct : public GuidPlatformPolicy
{
public:
    using GuidPlatformPolicy::GuidPlatformPolicy;

    static void GenerateGuid(GuidPlatformPolicy& guid)
    {
        GuidPlatformPolicy::Generate(guid);
    }
};


#endif // !CROSS_GUID_STRUCT_HPP
