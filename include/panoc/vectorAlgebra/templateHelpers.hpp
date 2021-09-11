#pragma once

namespace pnc {
    template <typename TVector, typename TMaybeData>
    struct IsDataOfVectorKind {
        static constexpr bool value = std::is_same<typename TVector::data_type, TMaybeData>::value;
    };
}
