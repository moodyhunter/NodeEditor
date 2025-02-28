#pragma once

#include <QtGlobal>

#if (QT_VERSION < QT_VERSION_CHECK(5, 14, 0))

// As of 5.14 there is a specialization std::hash<QString>

#include <QtCore/QString>
#include <QtCore/QVariant>
#include <functional>

namespace std
{
    template<>
    struct hash<QString>
    {
        inline std::size_t operator()(QString const &s) const
        {
            return qHash(s);
        }
    };
} // namespace std

#endif
