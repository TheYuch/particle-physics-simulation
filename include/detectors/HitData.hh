#pragma once

namespace Test
{

struct HitData
{
public: // extra, as public is default for struct
    HitData();
    HitData(const HitData& other);
    ~HitData();

    virtual const HitData& operator=(const HitData& other) =;
    virtual void Update(HitData* data) = 0;
    virtual void Print() = 0;
};

}