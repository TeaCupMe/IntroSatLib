#ifndef ISLIRPROTOCOL_H_
#define ISLIRPROTOCOL_H_

#include <stdint.h>

namespace IntroSatLib
{
    namespace IRProtocol
    {
        /**
         * \~russian
         * @brief Временные параметры ИК-протокола.
         *
         * Структура задаёт длительности маркеров и пробелов (в условных единицах),
         * а также допустимую погрешность и максимальные длительности.
         */
        struct ProtocolTimings {
            uint16_t markStart = 30;   ///< \~russian Длительность стартового маркера
            uint16_t spaceStart = 10;  ///< \~russian Длительность стартового пробела
            uint16_t mark0 = 10;       ///< \~russian Длительность маркера для бита 0
            uint16_t mark1 = 20;       ///< \~russian Длительность маркера для бита 1
            uint16_t space0 = 10;      ///< \~russian Длительность пробела для бита 0
            uint16_t space1 = 10;      ///< \~russian Длительность пробела для бита 1
            uint16_t errorScale = 5;   ///< \~russian Допустимое отклонение длительности (scale factor)
            uint16_t maxSpaceWidth = 100; ///< \~russian Максимальная длительность пробела
            uint16_t maxMarkWidth = 40;   ///< \~russian Максимальная длительность маркера
        };
    }
}

#endif