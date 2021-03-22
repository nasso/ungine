/*
** EPITECH PROJECT, 2021
** un
** File description:
** System
*/

#ifndef FE472698_E495_4B4C_95C4_2F2A646A84F5
#define FE472698_E495_4B4C_95C4_2F2A646A84F5

#include "World.hpp"
#include <functional>

namespace un {
namespace ecs {

    class System {
    public:
        System(std::function<void()> run);
        System(std::function<void(World&)> run);

        void run(World&);

    private:
        std::function<void(World&)> m_run;
    };

}
}

#endif /* FE472698_E495_4B4C_95C4_2F2A646A84F5 */