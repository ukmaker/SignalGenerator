#ifndef VIEWSTACK_H
#define VIEWSTACK_H

#ifndef VIEWSTACK_TOTAL_VIEWS
#define VIEWSTACK_TOTAL_VIEWS 6
#endif

#include "View.h"

class ViewStack {

public:

  ViewStack() : _nViews(0), _activeView(0) {}

  View *selectView(int v) {
    if(v < 0) {
        v = (_nViews - 1);
    } else if(v >= _nViews) {
        v = 0;
    }
    _activeView = v;
    return currentView();
  }

  View *nextView() {
    return selectView(_activeView + 1);
  }

  View *prevView() {
    return selectView(_activeView - 1);
  }

  View *currentView() {
    return _views[_activeView];
  }

  int views() {
    return _nViews;
  }

  int viewIndex() {
    return _activeView;
  }

  View *view(int v) {
    return _views[v];
  }

  void addView(View *view) {
    if(_nViews < VIEWSTACK_TOTAL_VIEWS) {
      _views[_nViews] = view;
      _nViews++;
    }
  }


protected:

 View *_views[VIEWSTACK_TOTAL_VIEWS];
 int _nViews;
 int _activeView;



};

#endif
