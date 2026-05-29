#include "libvrf/entt_view.hpp"

#include <terrainCS/coordSystem.h>
#include <vrfcore/simulationServices.h>
#include <vrfobjcore/physicalWorld.h>


namespace {

    const DtPhysicalWorld* get_phys_world(const DtSimObject& entity) {
        auto sim_services = entity.simulationServices();
        if (!sim_services)
            return nullptr;

        return sim_services->physicalWorld();
    }

    const Coordinate_System* get_coord_sys(const DtSimObject& entity) {
        return entity.coordinateSystem();
    }

    // Get a state component
    template <typename T>
    const T* get_scpnt(const kcei::EntityView& entity) {
        try {
            return entity->getStateComponent<T>();
        } catch (const makVrf::DtUnknownStateComponent& e) {
            return nullptr;
        }
    }

    const makVrf::DtEntityLocationStateComponent* get_entt_loc_state(
        const kcei::EntityView& entity
    ) {
        return ::get_scpnt<makVrf::DtEntityLocationStateComponent>(entity);
    }

}  // namespace


namespace kcei {

    EntityView::EntityView(const DtSimObject* entity)
        : entity_(const_cast<DtSimObject*>(entity)) {}

    DtSimObject* EntityView::operator->() { return entity_; }

    const DtSimObject* EntityView::operator->() const { return entity_; }

    const DtSimObject* EntityView::get() const { return entity_; }

    std::string EntityView::name() const {
        return entity_->objectName().stdString();
    }

    std::string EntityView::uuid() const { return entity_->uuid().string(); }

    bool EntityView::is_valid() const {
        if (!entity_)
            return false;
        if (!entity_->isValid())
            return false;
        return true;
    }

    bool EntityView::is_friendly_force() const {
        if (!this->is_valid())
            return false;
        return entity_->forceType() == DtForceType::DtForceFriendly;
    }

    bool EntityView::is_opposing_force() const {
        if (!this->is_valid())
            return false;
        return entity_->forceType() == DtForceType::DtForceOpposing;
    }

    DVec3 EntityView::geocen_pos() const {
        auto phys_world = ::get_phys_world(this->self());
        auto coord_sys = ::get_coord_sys(this->self());

        DVec3 geodetic;
        coord_sys->localToGeodetic(entity_->localPosition(), geodetic);

        DVec3 database;
        phys_world->geodeticToDatabase(geodetic, database);

        DVec3 geocen;
        phys_world->databaseToGeocentric(database, geocen);

        return geocen;
    }

    DVec3 EntityView::geocen_front() const {
        DVec3 output;
        const auto mat = entity_->worldOrientationMatrix();
        DtDcmVecMul(mat, DVec3{ 1, 0, 0 }, output);
        return output.normalize();
    }

    // VRFUsersGuide.pdf 33.4.2 Vector3D part says the vector in North-East-Down
    // coordinate system, so we need to flip the Z axis.
    DVec3 EntityView::geocen_up() const {
        DVec3 output;
        const auto mat = entity_->worldOrientationMatrix();
        DtDcmVecMul(mat, DVec3{ 0, 0, -1 }, output);
        return output.normalize();
    }

    DVec3 EntityView::geocen_right() const {
        DVec3 output;
        const auto mat = entity_->worldOrientationMatrix();
        DtDcmVecMul(mat, DVec3{ 0, 1, 0 }, output);
        return output.normalize();
    }

    DVec3 EntityView::geocen_vel() const {
        if (const auto state_cpnt = ::get_entt_loc_state(*this))
            return state_cpnt->worldVelocity();
        else
            return DVec3{};
    }

    DVec3 EntityView::geocen_acc() const {
        if (const auto state_cpnt = ::get_entt_loc_state(*this))
            return state_cpnt->worldAcceleration();
        else
            return DVec3{};
    }

    DtSimObject& EntityView::self() { return *entity_; }

    const DtSimObject& EntityView::self() const { return *entity_; }

}  // namespace kcei
