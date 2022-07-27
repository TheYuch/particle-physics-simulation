#pragma once

namespace Test
{

struct HitData
{
public: // extra, as public is default for struct
    HitData();
    virtual ~HitData() = 0;

    virtual const HitData& operator=(const HitData& other) = 0;
    virtual void Update(HitData* data) = 0;
    virtual void Print() = 0;
};

}