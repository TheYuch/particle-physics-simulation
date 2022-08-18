#pragma once

#include <TString.h>
#include <map>

namespace Constants {
   const TString DefaultFileName = "sim#RUN#";

   enum class RootDataType {Calo, Sipm};
   const std::map<RootDataType, TString> RootDataTypeNames = {{RootDataType::Calo, "Calo"}, {RootDataType::Sipm, "Sipm"}}; // TODO: keep this in sync with enum class RootDataType
}