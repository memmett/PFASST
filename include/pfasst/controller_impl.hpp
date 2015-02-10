#include "controller.hpp"

#include "config.hpp"


namespace pfasst
{
  template<typename time>
  Controller<time>::Controller()
    :   step(0)
      , iteration(0)
      , max_iterations(0)
      , t(0.0)
      , dt(0.0)
      , tend(0.0)
  {}

  template<typename time>
  Controller<time>::~Controller()
  {}

  template<typename time>
  void Controller<time>::set_options(bool all_sweepers)
  {
    this->tend = config::get_value<double>("tend", this->tend);
    this->dt = config::get_value<double>("dt", this->dt);
    this->max_iterations = config::get_value<size_t>("num_iters", this->max_iterations);

    // XXX: add some nice "nsteps" logic here

    if (all_sweepers) {
      for (auto l = coarsest(); l <= finest(); ++l) {
        l.current()->set_options();
      }
    }
  }

  template<typename time>
  void Controller<time>::setup()
  {
    for (auto l = coarsest(); l <= finest(); ++l) {
      l.current()->set_controller(this);
      l.current()->setup();
    }
  }

  template<typename time>
  void Controller<time>::set_duration(time t0, time tend, time dt, size_t niters)
  {
    this->t = t0;
    this->tend = tend;
    this->dt = dt;
    this->step = 0;
    this->iteration = 0;
    this->max_iterations = niters;
  }

  template<typename time>
  void Controller<time>::add_level(shared_ptr<ISweeper<time>> swpr,
                                   shared_ptr<ITransfer<time>> trnsfr,
                                   bool coarse)
  {
    if (coarse) {
      levels.push_front(swpr);
      transfer.push_front(trnsfr);
    } else {
      levels.push_back(swpr);
      transfer.push_back(trnsfr);
    }
  }

  template<typename time>
  size_t Controller<time>::nlevels()
  {
    return levels.size();
  }

  //! @{
  template<typename time>
  size_t Controller<time>::get_step()
  {
    return step;
  }

  template<typename time>
  void Controller<time>::set_step(size_t n)
  {
    t += (n - step) * dt;
    step = n;
  }

  template<typename time>
  time Controller<time>::get_time_step()
  {
    return dt;
  }

  template<typename time>
  time Controller<time>::get_time()
  {
    return t;
  }

  template<typename time>
  void Controller<time>::advance_time(size_t nsteps)
  {
    step += nsteps;
    t += nsteps * dt;
  }

  template<typename time>
  time Controller<time>::get_end_time()
  {
    return tend;
  }

  template<typename time>
  size_t Controller<time>::get_iteration()
  {
    return iteration;
  }

  template<typename time>
  void Controller<time>::set_iteration(size_t iter)
  {
    this->iteration = iter;
  }

  template<typename time>
  void Controller<time>::advance_iteration()
  {
    iteration++;
  }

  template<typename time>
  size_t Controller<time>::get_max_iterations()
  {
    return max_iterations;
  }

  template<typename time>
  typename Controller<time>::LevelIter Controller<time>::finest()
  {
    return Controller<time>::LevelIter(nlevels() - 1, this);
  }

  template<typename time>
  typename Controller<time>::LevelIter Controller<time>::coarsest()
  {
    return Controller<time>::LevelIter(0, this);
  }


  template<typename time>
  Controller<time>::LevelIter::LevelIter(int level, Controller* ts)
    :   ts(ts)
      , level(level)
  {}

  template<typename time>
  typename Controller<time>::LevelIter Controller<time>::LevelIter::operator++()
  {
    level++;
    return *this;
  }

  template<typename time>
  bool Controller<time>::LevelIter::operator==(typename Controller<time>::LevelIter i)
  {
    return level == i.level;
  }

  template<typename time>
  bool Controller<time>::LevelIter::operator!=(typename Controller<time>::LevelIter i)
  {
    return level != i.level;
  }

  template<typename time>
  typename Controller<time>::LevelIter Controller<time>::LevelIter::operator--()
  {
    level--;
    return *this;
  }

  template<typename time>
  typename Controller<time>::LevelIter Controller<time>::LevelIter::operator-(difference_type i)
  {
    return LevelIter(level - i, ts);
  }

  template<typename time>
  typename Controller<time>::LevelIter Controller<time>::LevelIter::operator+(difference_type i)
  {
    return LevelIter(level + i, ts);
  }

  template<typename time>
  bool Controller<time>::LevelIter::operator<=(typename Controller<time>::LevelIter i)
  {
    return level <= i.level;
  }

  template<typename time>
  bool Controller<time>::LevelIter::operator>=(typename Controller<time>::LevelIter i)
  {
    return level >= i.level;
  }

  template<typename time>
  bool Controller<time>::LevelIter::operator<(typename Controller<time>::LevelIter i)
  {
    return level < i.level;
  }

  template<typename time>
  bool Controller<time>::LevelIter::operator>(typename Controller<time>::LevelIter i)
  {
    return level > i.level;
  }
}  // ::pfasst
