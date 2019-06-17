#ifndef STRIDANI_AKCI_H
#define STRIDANI_AKCI_H

template<typename T>
class StridaniAkci {
private:
  T *const _instance;
  void (T::*const _akce1)();
  void (T::*const _akce2)();

  bool _spustitAkci1 = true;

public:
  StridaniAkci(T *const instance, void (T::*const akce1)(), void (T::*const akce2)(), bool spustitOkamzite)
    : _instance(instance),
      _akce1(akce1),
      _akce2(akce2) {
    if (spustitOkamzite) {
      vystridat();
    }
  }

  void vystridat() {
    if (_spustitAkci1) {
      (_instance->*_akce1)();
    }
    else {
      (_instance->*_akce2)();
    }

    _spustitAkci1 = !_spustitAkci1;
  }

  void resetovat() {
    _spustitAkci1 = true;
  }
};

#endif
