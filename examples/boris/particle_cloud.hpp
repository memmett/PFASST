#ifndef _EXAMPLES__BORIS__PARTICLE_CLOUD_HPP_
#define _EXAMPLES__BORIS__PARTICLE_CLOUD_HPP_

#include <memory>
#include <vector>
using namespace std;

#include <pfasst/easylogging++.h>

#include <pfasst/encap/encapsulation.hpp>

#include "particle.hpp"


namespace examples
{
  namespace boris
  {
    template<typename precision>
    using ParticleCloudComponent = vector<vector<precision>>;

    template<typename precision>
    using AttributeValues = ParticleComponent<precision>;


    template<typename precision>
    class ParticleCloud
      : public pfasst::encap::Encapsulation<precision>
    {
      private:
        size_t _dim;
        size_t _num_particles;
        ParticleCloudComponent<precision> _positions;
        ParticleCloudComponent<precision> _velocities;
        AttributeValues<precision> _charges;
        AttributeValues<precision> _masses;

        precision _default_charge;
        precision _default_mass;

      public:
        explicit ParticleCloud(const size_t num_particles = 0,
                               const size_t dim = 3,
                               const precision default_charge = precision(1.0),
                               const precision default_mass = precision(1.0));
        virtual ~ParticleCloud();

        virtual void zero() override;
        virtual void copy(shared_ptr<pfasst::encap::Encapsulation<precision>> other);

        void extend(const size_t new_size);
        void erase(const size_t index);
        void push_back(const shared_ptr<Particle<precision>>& particle);
        void insert(const size_t pos, const shared_ptr<Particle<precision>>& paticle);

        size_t size() const;
        size_t dim() const;
        ParticleCloudComponent<precision>& positions();
        ParticleCloudComponent<precision>& velocities();
        vector<precision>& charges();
        vector<precision>& masses();

        ParticleComponent<precision> center_of_mass() const;
        shared_ptr<Particle<precision>> operator[](const size_t index) const;
        shared_ptr<Particle<precision>> at(const size_t index) const;
        // !! EXPENSIVE !!
        vector<shared_ptr<Particle<precision>>> particles() const;
    };


    template<typename precision>
    class ParticleCloudFactory
      : public pfasst::encap::EncapFactory<precision>
    {
      private:
        size_t _num_particles;
        size_t _dim;
        precision _default_charge;
        precision _default_mass;

      public:
        ParticleCloudFactory(const size_t num_particles, const size_t dim, const precision default_charge,
                             const precision default_mass);
        size_t num_particles() const;
        size_t dim() const;
        virtual shared_ptr<pfasst::encap::Encapsulation<precision>> create(const pfasst::encap::EncapType);
    };
  }
}


#include "particle_cloud_impl.hpp"

#endif  // _EXAMPLES__BORIS__PARTICLE_CLOUD_HPP_
