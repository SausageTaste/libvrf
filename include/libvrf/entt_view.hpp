#pragma once

#include "libvrf/vector.hpp"


class DtSimObject;


namespace kcei {

    class EntityView {

    public:
        EntityView() = default;
        explicit EntityView(const DtSimObject* entity);

        DtSimObject* operator->();
        const DtSimObject* operator->() const;

        const DtSimObject* get() const;

        std::string name() const;
        std::string uuid() const;

        bool is_valid() const;
        bool is_friendly_force() const;
        bool is_opposing_force() const;

        DVec3 geocen_pos() const;
        DVec3 geocen_front() const;
        DVec3 geocen_up() const;
        DVec3 geocen_right() const;

        DVec3 geocen_vel() const;
        DVec3 geocen_acc() const;

    private:
        DtSimObject& self();
        const DtSimObject& self() const;

        DtSimObject* entity_ = nullptr;
    };

}  // namespace kcei
