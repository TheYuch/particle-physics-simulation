#pragma once

#include <TString.h>

namespace Constants {
   const TString DefaultFileName = "sim#RUN#";

   enum class RootDataTypes {Calo, Sipm};
   static const char* RootDataTypeNames[2] = {"Calo", "Sipm"}; // TODO: keep this in sync with enum class RootDataTypes
}