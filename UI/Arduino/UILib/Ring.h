#ifndef RING_H
#define RING_H

#ifndef RING_SIZE
#define RING_SIZE 6
#endif

template <typename T, uint8_t S>
class Ring {

public:

  Ring() : _size(0), _current(0) {}

   T *select(int v) {
    if(v < 0) {
        v = (_size - 1);
    } else if(v >= _size) {
        v = 0;
    }
    _current = v;
    return current();
  }

  T *next() {
    return select(_current + 1);
  }

  T *prev() {
    return select(_current - 1);
  }

  T *current() {
    return _refs[_current];
  }

  int size() {
    return _size;
  }

  int index() {
    return _current;
  }

  T *view(int v) {
    return _refs[v];
  }

  void add(T *ref) {
    if(_size < S) {
      _refs[_size] = ref;
      _size++;
    }
  }


protected:

 T* _refs[S];
 int _size;
 int _current;



};

#endif
